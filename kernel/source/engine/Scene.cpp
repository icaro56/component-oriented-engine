#include "engine/Scene.h"
#include "engine/Engine.h"
#include "engine/RenderingEngine.h"
#include "engine/PhysicsEngine.h"
#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "gameobject/LayerMask.h"
#include "components/physics/Collider.h"
#include "components/physics/Rigidbody.h"
#include "components/transform/Transform.h"
#include "time/Time.h"
#include "utils/KernelConverter.h"

#include "utils/Streams.h"
#include "utils/StringUtils.h"

#include "PxScene.h"
#include "PxPhysics.h"
#include "PxRigidStatic.h"
#include "PxActor.h"
#include "PxRigidDynamic.h"
#include "PxRigidBody.h"

#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxRigidBodyExt.h"
#include "extensions/PxSimpleFactory.h"
#include "characterkinematic/PxControllerManager.h"
#include "characterkinematic/PxCapsuleController.h"
#include "geometry/PxHeightField.h"

#include "transform/Transform.h"

#include "CollisionTools.h"
#include "OgreOverlaySystem.h"

#include "engine/buffers/RaycastHit.h"

Scene::Scene(const std::string& name, Ogre::SceneType typeMask)
	: mPxScene(nullptr),
	  mSceneMgr(nullptr),
	  rootGameObject(nullptr),
	  rootTransform(nullptr),
	  controllerManager(nullptr),
	  mRaySceneQuery(nullptr),
	  mVolQuery(nullptr)
{
	// create the scene manager
	mSceneMgr = Rendering->getRoot()->createSceneManager(typeMask, name);

	collisionTools = new MOC::CollisionTools(mSceneMgr);

	Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(pOverlaySystem);

	initRayQueries();

	// física
	physx::PxSceneDesc desc(Physics->getPhysics()->getTolerancesScale());
	desc.gravity = Convert::toPx(Physics->getGravity());
	desc.bounceThresholdVelocity = Physics->getBounceThreshold();
	desc.filterShader = mySimulationEventCallback.filter;
	desc.simulationEventCallback = &mySimulationEventCallback;
	desc.flags |= physx::PxSceneFlag::eENABLE_CCD 
		       | physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS 
			   | physx::PxSceneFlag::eENABLE_KINEMATIC_PAIRS
			   | physx::PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;
	
	//groupCollisionFlags precisa ser configurado antes de criar uma cena, logo, como usamos uma cena padrão, devemos
	// configurar os grupos que vão se colidir antes de chamar o método initRenderEngine
	/*desc.filterShaderData = LayerMask::groupCollisionFlags();
	desc.filterShaderDataSize = 32 * sizeof(physx::PxU32);
	//forma que simula physx 2.x, mas engessa a possibilidade de trocar a colisão entre grupos em tempo de execução.
	*/

	desc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
	if(!desc.cpuDispatcher)
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] PxDefaultCpuDispatcherCreate failed!");
		return;
	}

	if (!Physics->getPhysics())
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] Error: Cannot create scene because World is not initialised properly.");
		return;
	}

	mPxScene = Physics->getPhysics()->createScene(desc);
	
	if (!mPxScene)
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] create scene failed!");
		return;
	}

	mTimeAccumulator = 0.0f;
}

Scene::~Scene()
{
	if (collisionTools)
	{
		delete collisionTools;
		collisionTools = nullptr;
	}

	if (mVolQuery)
	{
		getOgreScene()->destroyQuery(mVolQuery);
	}

	//deletando todos os gameObject que estão nessa cena
	destroyAllGameObjects();

	destroyControllerManager();

	if (mPxScene)
	{
		mPxScene->release();
		mPxScene = nullptr;
	}

	if (mSceneMgr)
	{
		mSceneMgr->clearScene();
		Rendering->getRoot()->destroySceneManager(mSceneMgr);
		mSceneMgr = nullptr;
	}
}

std::string Scene::getName() const
{
	return mSceneMgr->getName();
}

Ogre::SceneManager* Scene::getOgreScene()
{
	return mSceneMgr;
}

physx::PxScene* Scene::getPhysxScene()
{
	return mPxScene;
}

void Scene::addGameObject(GameObject* gameObject)
{
	if (!gameObjects.count(gameObject->getName()))
	{
		gameObjects[gameObject->getName()] = gameObject;

		GameObjectManager::getInstance()->addObject(gameObject);
	}
}

void Scene::removeGameObject(GameObject* gameObject)
{
	if (gameObjects.count(gameObject->getName()))
	{
		gameObjects.erase(gameObject->getName());

		//GameObjectManager::getInstance()->removeObject(gameObject);
	}
}

void Scene::destroyGameObject(GameObject* gameObject)
{
	if (gameObjects.count(gameObject->getName()))
	{
		gameObjects.erase(gameObject->getName());
		GameObjectManager::getInstance()->removeObject(gameObject);

		delete gameObject;
		gameObject = 0;
	}
}

void Scene::destroyAllGameObjects()
{
	GameObjectMap::iterator iter;
	for(iter = gameObjects.begin(); iter != gameObjects.end();iter++) 
	{
		GameObject* gameobject = iter->second;

		if (gameobject)
		{
			GameObjectManager::getInstance()->removeObject(gameobject);
			delete gameobject;
			gameobject = 0;
		}
	}

	gameObjects.clear();
}

GameObject* Scene::findGameObject(const std::string& name)
{
	if (gameObjects.count(name))
	{
		return gameObjects.at(name);
	}

	return NULL;
}

void Scene::clear()
{
	gameObjects.clear();
}

void Scene::updateGameObjects()
{
	GameObjectMap::iterator iter;
	for(iter = gameObjects.begin(); iter != gameObjects.end(); iter++) 
	{
		iter->second->update();
	}
}

void Scene::lateUpdateGameObjects()
{
	GameObjectMap::iterator iter;
	for(iter = gameObjects.begin(); iter != gameObjects.end(); iter++) 
	{
		iter->second->lateUpdate();
	}
}

void Scene::fixedUpdateGameObject()
{
	GameObjectMap::iterator iter;
	for(iter = gameObjects.begin(); iter != gameObjects.end(); iter++) 
	{
		iter->second->fixedUpdate();
	}
}

void Scene::update(float dt)
{
	mySimulationEventCallback.update();

	simulate(dt);

	mPxScene->fetchResults(true);

	updateGameObjects();
	lateUpdateGameObjects();
}

bool Scene::simulate(float dt)
{
	mTimeAccumulator  += dt;
	float mStepSize = Time->getFixedDeltaTime();

	if(mTimeAccumulator < mStepSize)
		return false;

	mTimeAccumulator -= mStepSize;

	Time->updateFixedTime();

	fixedUpdateGameObject();

	mPxScene->simulate(mStepSize);

	return true;

}

physx::PxShape* Scene::createCapsuleShape(physx::PxRigidActor* actor, float radius, float height, 
										  float density, 
										  const physx::PxTransform &shapePose)
{
	return createCapsuleShape(actor, radius, height, density, *Physics->getDefaultMaterial(), shapePose);
}

physx::PxShape* Scene::createCapsuleShape(physx::PxRigidActor* actor, float radius, float height, 
										  float density, 
										  physx::PxMaterial &material, 
										  const physx::PxTransform &shapePose)
{
	



	return createShape(actor, physx::PxCapsuleGeometry(radius, height), material, shapePose, density);
}

physx::PxShape* Scene::createSphereShape( physx::PxRigidActor* actor,
								  float radius,
								  float density,
								  const physx::PxTransform &shapePose)
{
	return createSphereShape(actor, radius, density, *Physics->getDefaultMaterial(), shapePose);
}

physx::PxShape* Scene::createSphereShape( physx::PxRigidActor* actor,
								  float radius,
								  float density,
								  physx::PxMaterial &material,
								  const physx::PxTransform &shapePose)
{
	return createShape(actor, physx::PxSphereGeometry(radius), material, shapePose, density);
}

physx::PxShape* Scene::createBoxShape(physx::PxRigidActor* actor, 
									  const physx::PxVec3& dim, 
									  float density,
									  const physx::PxTransform &shapePose)
{
	return createBoxShape(actor, dim, density, *Physics->getDefaultMaterial(), shapePose);
}

physx::PxShape* Scene::createBoxShape(physx::PxRigidActor* actor, 
									  const physx::PxVec3& dim, 
									  float density,
									  physx::PxMaterial &material,
									  const physx::PxTransform &shapePose)
{
	return createShape(actor, physx::PxBoxGeometry(dim), material, shapePose);
}

physx::PxRigidStatic* Scene::createRigidStatic(const physx::PxTransform &actorPose)
{
	physx::PxRigidStatic *pxActor = Physics->getPhysics()->createRigidStatic(actorPose);
	mPxScene->addActor(*pxActor);
	return pxActor;
}

physx::PxRigidDynamic* Scene::createRigidDynamic(const physx::PxTransform &actorPose)
{
	physx::PxRigidDynamic *pxActor = Physics->getPhysics()->createRigidDynamic(actorPose);
	mPxScene->addActor(*pxActor);
	return pxActor;
}

physx::PxShape* Scene::createHeightFieldShape(physx::PxRigidActor* actor, physx::PxHeightField* heightField, const Vector3& scale)
{

	physx::PxHeightFieldGeometry hfGeom(heightField, physx::PxMeshGeometryFlags(), scale.y, scale.z, scale.x);

	return createShape(actor, hfGeom, *Physics->getDefaultMaterial());
}

physx::PxShape* Scene::createShape(physx::PxRigidActor* actor, 
								   physx::PxGeometry &geometry, 
								   physx::PxMaterial &material, 
								   const physx::PxTransform &shapePose,
								   float density)
{
	physx::PxShape* shape = actor->createShape(geometry, material);

	if (shape)
	{
		shape->setLocalPose(shapePose);
	}

	if (actor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
	{
		physx::PxRigidBody *aux = static_cast< physx::PxRigidBody* >(actor);
		physx::PxRigidBodyExt::updateMassAndInertia(*aux, density);
	}

	return shape;
}

void Scene::createPlane(const physx::PxVec3& normal, float distance)
{
	/*physx::PxTransform t = physx::PxTransform(physx::PxVec3(0,0,0));
	physx::PxRigidStatic* plane = createRigidStatic(t);
	createShape(plane, physx::PxPlaneGeometry(), *GEngine->getDefaultMaterial());*/

	physx::PxRigidStatic* plane = physx::PxCreatePlane(*Physics->getPhysics(), 
									physx::PxPlane(physx::PxVec3(0,1,0), 0), 
									*Physics->getDefaultMaterial());

	plane->setName("Plane_Test");

	mySimulationEventCallback.setupFiltering(plane, 1 << 0, 0, true);

	mPxScene->addActor(*plane);
}

bool Scene::raycast(const Vector3& origin, const Vector3& direction, float distance, unsigned layerMask)
{
	physx::PxRaycastBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	return mPxScene->raycast(	Convert::toPx(origin), 
								Convert::toPx(direction), 
								distance, 
								hit, 
								physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
								filterData, 
								&mySceneQueryEventCallback);
}

bool Scene::raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hitInfo,float distance, unsigned layerMask)
{
	physx::PxRaycastBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);


	bool result = mPxScene->raycast(	Convert::toPx(origin), 
										Convert::toPx(direction), 
										distance, 
										hit, 
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

	if (hit.hasBlock)
	{
		Collider* collider = reinterpret_cast<Collider*>(hit.block.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		hitInfo.setBarycentricCoordinate(Vector2(hit.block.u, hit.block.v));
		hitInfo.setDistance(hit.block.distance);
		hitInfo.setNormal(Convert::toKernel(hit.block.normal));
		hitInfo.setPoint(Convert::toKernel(hit.block.position));
		hitInfo.setTriangleIndex(hit.block.faceIndex);
	}

	return result;
}

bool Scene::raycast(const Ray& ray, float distance, unsigned layerMask)
{
	physx::PxRaycastBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	return mPxScene->raycast(	Convert::toPx(ray.getOrigin()), 
								Convert::toPx(ray.getDirection()), 
								distance, 
								hit, 
								physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
								filterData, 
								&mySceneQueryEventCallback);
}

bool Scene::raycast(const Ray& ray, RaycastHit& hitInfo, float distance, unsigned layerMask)
{
	physx::PxRaycastBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	bool result = mPxScene->raycast(	Convert::toPx(ray.getOrigin()), 
										Convert::toPx(ray.getDirection()), 
										distance, 
										hit, 
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

	if (hit.hasBlock)
	{
		Collider* collider = reinterpret_cast<Collider*>(hit.block.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		hitInfo.setBarycentricCoordinate(Vector2(hit.block.u, hit.block.v));
		hitInfo.setDistance(hit.block.distance);
		hitInfo.setNormal(Convert::toKernel(hit.block.normal));
		hitInfo.setPoint(Convert::toKernel(hit.block.position));
		hitInfo.setTriangleIndex(hit.block.faceIndex);
	}


	return result;
}

RaycastHitList Scene::raycastAll(const Vector3& origin, const Vector3& direction, float distance, unsigned layerMask)
{
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxRaycastHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here


	bool result = mPxScene->raycast(	Convert::toPx(origin), 
										Convert::toPx(direction), 
										distance, 
										buf, 
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

	RaycastHitList list;

	for (physx::PxU32 i = 0; i < buf.nbTouches; ++i)
	{
		RaycastHit hitInfo;
		physx::PxRaycastHit hit = buf.touches[i];

		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);

		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
		hitInfo.setDistance(hit.distance);
		hitInfo.setNormal(Convert::toKernel(hit.normal));
		hitInfo.setPoint(Convert::toKernel(hit.position));
		hitInfo.setTriangleIndex(hit.faceIndex);

		list.push_back(hitInfo);
	}

	return list;
}

RaycastHitList Scene::raycastAll(const Ray& ray, float distance, unsigned layerMask)
{
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxRaycastHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here


	bool result = mPxScene->raycast(	Convert::toPx(ray.getOrigin()), 
										Convert::toPx(ray.getDirection()), 
										distance, 
										buf, 
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

	RaycastHitList list;

	for (physx::PxU32 i = 0; i < buf.nbTouches; ++i)
	{
		RaycastHit hitInfo;
		physx::PxRaycastHit hit = buf.touches[i];

		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);
		
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
		hitInfo.setDistance(hit.distance);
		hitInfo.setNormal(Convert::toKernel(hit.normal));
		hitInfo.setPoint(Convert::toKernel(hit.position));
		hitInfo.setTriangleIndex(hit.faceIndex);

		list.push_back(hitInfo);
	}

	return list;
}

bool Scene::sweep(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, RaycastHit& hitInfo, float distance, unsigned layerMask)
{
	if (geo)
	{
		physx::PxSweepBuffer hit;
		physx::PxQueryFilterData filterData;

		filterData.data.word0 = layerMask;
		filterData.flags |= physx::PxQueryFlag::ePREFILTER;

		mySceneQueryEventCallback.setMode(true);

		physx::PxGeometry* pxGeo = nullptr;

		GeometryType type = geo->getType();
		switch (type)
		{
		case GeometryType::GT_Box:
		{
			BoxGeometry* box = dynamic_cast<BoxGeometry*>(geo);
			pxGeo =  new physx::PxBoxGeometry(Convert::toPx(box->halfExtents));
			break;
		}
		case GeometryType::GT_Sphere:
		{
			SphereGeometry* sphere = (SphereGeometry*)geo;
			pxGeo = &physx::PxSphereGeometry(sphere->radius);
			break;
		}
		case GeometryType::GT_Capsule:
		{
			CapsuleGeometry* capsule = (CapsuleGeometry*)geo;
			pxGeo = &physx::PxCapsuleGeometry(capsule->radius, capsule->halfHeight);
			break;
		}
			
		default:
			return false;
			break;
		}


		bool result = mPxScene->sweep(*pxGeo, Convert::toPxTransform(pos, ori), Convert::toPx(direction), distance, hit,
			physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, filterData, &mySceneQueryEventCallback);

		if (result && hit.hasBlock)
		{
			Collider* collider = reinterpret_cast<Collider*>(hit.block.shape->userData);
			if (collider)
			{
				hitInfo.setCollider(collider);

				Rigidbody* r = collider->rigidbody();
				hitInfo.setRigidbody(r);

				Transform* t = collider->transform();
				hitInfo.setTransform(t);
			}

			//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
			hitInfo.setDistance(hit.block.distance);
			hitInfo.setNormal(Convert::toKernel(hit.block.normal));
			hitInfo.setPoint(Convert::toKernel(hit.block.position));
			hitInfo.setTriangleIndex(hit.block.faceIndex);
		}

		return result;
	}
	
	return false;
}

RaycastHitList Scene::sweepAll(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, float distance, unsigned layerMask)
{
	RaycastHitList list;
	if (geo)
	{
		const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
		physx::PxSweepHit hitBuffer[bufferSize];  // [out] User provided buffer for results
		physx::PxSweepBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here

		physx::PxQueryFilterData filterData;

		filterData.data.word0 = layerMask;
		filterData.flags |= physx::PxQueryFlag::ePREFILTER;

		mySceneQueryEventCallback.setMode(false);

		physx::PxGeometry* pxGeo = nullptr;

		GeometryType type = geo->getType();
		switch (type)
		{
			case GeometryType::GT_Box:
			{
				BoxGeometry* box = (BoxGeometry*)geo;
				pxGeo = &physx::PxBoxGeometry(Convert::toPx(box->halfExtents));
				break;
			}
			case GeometryType::GT_Sphere:
			{
				SphereGeometry* sphere = (SphereGeometry*)geo;
				pxGeo = &physx::PxSphereGeometry(sphere->radius);
				break;
			}
			case GeometryType::GT_Capsule:
			{
				CapsuleGeometry* capsule = (CapsuleGeometry*)geo;
				pxGeo = &physx::PxCapsuleGeometry(capsule->radius, capsule->halfHeight);
				break;
			}
			default:
				return list;
				break;
		}

		if (geo)
		{
			delete geo;
			geo = nullptr;
		}

		bool result = mPxScene->sweep(*pxGeo, Convert::toPxTransform(pos, ori), Convert::toPx(direction), distance, buf,
			physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, filterData, &mySceneQueryEventCallback);

		for(physx::PxU32 i = 0; i < buf.nbTouches; ++i)
		{
			RaycastHit hitInfo;
			physx::PxSweepHit hit = buf.touches[i];
			Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);
			if (collider)
			{
				hitInfo.setCollider(collider);

				Rigidbody* r = collider->rigidbody();
				hitInfo.setRigidbody(r);

				Transform* t = collider->transform();
				hitInfo.setTransform(t);
			}

			//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
			hitInfo.setDistance(hit.distance);
			hitInfo.setNormal(Convert::toKernel(hit.normal));
			hitInfo.setPoint(Convert::toKernel(hit.position));
			hitInfo.setTriangleIndex(hit.faceIndex);

			list.push_back(hitInfo);
		}

		return list;
	}

	return list;
}

bool Scene::sphereCast(const Vector3& origin, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance, unsigned layerMask)
{
	physx::PxSweepBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->sweep(	pxGeo, 
									physx::PxTransform(Convert::toPx(origin)), 
									Convert::toPx(direction), 
									distance, 
									hit,
									physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
									filterData, 
									&mySceneQueryEventCallback);

	if (result && hit.hasBlock)
	{
		Collider* collider = reinterpret_cast<Collider*>(hit.block.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
		hitInfo.setDistance(hit.block.distance);
		hitInfo.setNormal(Convert::toKernel(hit.block.normal));
		hitInfo.setPoint(Convert::toKernel(hit.block.position));
		hitInfo.setTriangleIndex(hit.block.faceIndex);
	}

	return result;
}

bool Scene::sphereCast(const Ray& ray, float radius, RaycastHit& hitInfo, float distance, unsigned layerMask)
{
	return sphereCast(ray.getOrigin(), radius, ray.getDirection(), hitInfo, distance, layerMask);
}

bool  Scene::sphereCast(const Ray& ray, float radius, float distance, unsigned layerMask)
{
	physx::PxSweepBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->sweep(	pxGeo, 
		physx::PxTransform(Convert::toPx(ray.getOrigin())), 
		Convert::toPx(ray.getDirection()), 
		distance, 
		hit,
		physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
		filterData, 
		&mySceneQueryEventCallback);

	return result;
}

RaycastHitList Scene::sphereCastAll(const Ray& ray, float radius, float distance, unsigned layerMask)
{
	RaycastHitList list;

	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxSweepHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxSweepBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here

	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->sweep(pxGeo, physx::PxTransform(Convert::toPx(ray.getOrigin())), 
															Convert::toPx(ray.getDirection()), 
															distance, 
															buf,
															physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
															filterData, 
															&mySceneQueryEventCallback);

	for(physx::PxU32 i = 0; i < buf.nbTouches; ++i)
	{
		RaycastHit hitInfo;
		physx::PxSweepHit hit = buf.touches[i];
		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
		hitInfo.setDistance(hit.distance);
		hitInfo.setNormal(Convert::toKernel(hit.normal));
		hitInfo.setPoint(Convert::toKernel(hit.position));
		hitInfo.setTriangleIndex(hit.faceIndex);

		list.push_back(hitInfo);
	}

	return list;
}

RaycastHitList Scene::sphereCastAll(const Vector3& origin, float radius, const Vector3& direction, float distance, unsigned layerMask)
{
	Ray ray(origin, direction);
	return sphereCastAll(ray, radius, distance, layerMask);
}

bool Scene::capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance, unsigned layerMask)
{
	physx::PxSweepBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->sweep(	pxGeo, 
									physx::PxTransform(Convert::toPx(point1)), 
									Convert::toPx(direction), 
									distance, 
									hit,
									physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
									filterData, 
									&mySceneQueryEventCallback);

	if ( result && hit.hasBlock)
	{
		Collider* collider = reinterpret_cast<Collider*>(hit.block.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
		hitInfo.setDistance(hit.block.distance);
		hitInfo.setNormal(Convert::toKernel(hit.block.normal));
		hitInfo.setPoint(Convert::toKernel(hit.block.position));
		hitInfo.setTriangleIndex(hit.block.faceIndex);

		return result;
	}
	else
	{
		bool result2 = mPxScene->sweep(	pxGeo, 
										physx::PxTransform(Convert::toPx(point2)), 
										Convert::toPx(direction), 
										distance, 
										hit,
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

		if ( result2 && hit.hasBlock)
		{
			Collider* collider = reinterpret_cast<Collider*>(hit.block.shape->userData);
			if (collider)
			{
				hitInfo.setCollider(collider);

				Rigidbody* r = collider->rigidbody();
				hitInfo.setRigidbody(r);

				Transform* t = collider->transform();
				hitInfo.setTransform(t);
			}

			//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
			hitInfo.setDistance(hit.block.distance);
			hitInfo.setNormal(Convert::toKernel(hit.block.normal));
			hitInfo.setPoint(Convert::toKernel(hit.block.position));
			hitInfo.setTriangleIndex(hit.block.faceIndex);

			return result2;
		}
	}

	return false;
}

bool Scene::capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance, unsigned layerMask)
{
	physx::PxSweepBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->sweep(	pxGeo, 
									physx::PxTransform(Convert::toPx(point1)), 
									Convert::toPx(direction), 
									distance, 
									hit,
									physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
									filterData, 
									&mySceneQueryEventCallback);

	if ( result)
	{
		return result;
	}
	else
	{
		bool result2 = mPxScene->sweep(	pxGeo, 
										physx::PxTransform(Convert::toPx(point2)), 
										Convert::toPx(direction), 
										distance, 
										hit,
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

		if ( result2)
		{
			return result2;
		}
	}

	return false;
}

RaycastHitList Scene::capsuleCastAll(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance, unsigned layerMask)
{
	RaycastHitList list;

	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxSweepHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxSweepBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here
	physx::PxSweepBuffer buf2(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here

	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->sweep(	pxGeo, physx::PxTransform(Convert::toPx(point1)), 
									Convert::toPx(direction), 
									distance, 
									buf,
									physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
									filterData, 
									&mySceneQueryEventCallback);

	for(physx::PxU32 i = 0; i < buf.nbTouches; ++i)
	{
		RaycastHit hitInfo;
		physx::PxSweepHit hit = buf.touches[i];
		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
		hitInfo.setDistance(hit.distance);
		hitInfo.setNormal(Convert::toKernel(hit.normal));
		hitInfo.setPoint(Convert::toKernel(hit.position));
		hitInfo.setTriangleIndex(hit.faceIndex);

		list.push_back(hitInfo);
	}

	bool result2 = mPxScene->sweep(	pxGeo, physx::PxTransform(Convert::toPx(point2)), 
									Convert::toPx(direction), 
									distance, 
									buf2,
									physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
									filterData, 
									&mySceneQueryEventCallback);

	for(physx::PxU32 i = 0; i < buf2.nbTouches; ++i)
	{
		RaycastHit hitInfo;
		physx::PxSweepHit hit = buf2.touches[i];
		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		//hitInfo.setBarycentricCoordinate(Vector2(hit.u, hit.v));
		hitInfo.setDistance(hit.distance);
		hitInfo.setNormal(Convert::toKernel(hit.normal));
		hitInfo.setPoint(Convert::toKernel(hit.position));
		hitInfo.setTriangleIndex(hit.faceIndex);

		list.push_back(hitInfo);
	}

	return list;
}

ColliderVector Scene::overlapSphere(const Vector3& position, float radius, unsigned layerMask)
{
	ColliderVector list;

	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxOverlapHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxOverlapBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here

	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->overlap(pxGeo,
									physx::PxTransform(Convert::toPx(position)),
									buf,
									filterData,
									&mySceneQueryEventCallback);

	for(physx::PxU32 i = 0; i < buf.nbTouches; ++i)
	{
		physx::PxOverlapHit hit = buf.touches[i];
		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);
		if (collider)
		{
			list.push_back(collider);
		}
	}

	return list;
}

bool Scene::checkSphere(const Vector3& position, float radius, unsigned layerMask)
{
	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxOverlapHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxOverlapBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here

	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->overlap(pxGeo,
		physx::PxTransform(Convert::toPx(position)),
		buf,
		filterData,
		&mySceneQueryEventCallback);

	return result;
}

bool Scene::checkCapsule(const Vector3& start, const Vector3& end, float radius, unsigned layerMask)
{
	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxOverlapHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxOverlapBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here

	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	physx::PxGeometry pxGeo = physx::PxSphereGeometry(radius);

	bool result = mPxScene->overlap(pxGeo,
		physx::PxTransform(Convert::toPx(start)),
		buf,
		filterData,
		&mySceneQueryEventCallback);

	if (result)
		return result;

	bool result2 = mPxScene->overlap(pxGeo,
		physx::PxTransform(Convert::toPx(end)),
		buf,
		filterData,
		&mySceneQueryEventCallback);

	if (result2)
		return result2;

	return false;
}

ColliderVector Scene::overlap(Geometry* geo,const Vector3& pos, const Quaternion& ori, unsigned layerMask)
{
	ColliderVector list;

	const physx::PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	physx::PxOverlapHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	physx::PxOverlapBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits will be stored here

	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(false);

	physx::PxGeometry* pxGeo = nullptr;

	GeometryType type = geo->getType();
	switch (type)
	{
	case GeometryType::GT_Box:
		{
			BoxGeometry* box = (BoxGeometry*)geo;
			pxGeo = &physx::PxBoxGeometry(Convert::toPx(box->halfExtents));
			break;
		}
	case GeometryType::GT_Sphere:
		{
			SphereGeometry* sphere = (SphereGeometry*)geo;
			pxGeo = &physx::PxSphereGeometry(sphere->radius);
			break;
		}
	case GeometryType::GT_Capsule:
		{
			CapsuleGeometry* capsule = (CapsuleGeometry*)geo;
			pxGeo = &physx::PxCapsuleGeometry(capsule->radius, capsule->halfHeight);
			break;
		}
	default:
		return list;
		break;
	}

	if (geo)
	{
		delete geo;
		geo = nullptr;
	}

	bool result = mPxScene->overlap(*pxGeo,
									physx::PxTransform(Convert::toPx(pos)),
									buf,
									filterData,
									&mySceneQueryEventCallback);

	for(physx::PxU32 i = 0; i < buf.nbTouches; ++i)
	{
		physx::PxOverlapHit hit = buf.touches[i];
		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);
		if (collider)
		{
			list.push_back(collider);
		}
	}

	return list;
}

bool Scene::lineCast(const Vector3& start, const Vector3& end, RaycastHit& hitInfo, unsigned layerMask)
{
	physx::PxRaycastBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	Vector3 dir = end - start;
	float distance = dir.normalise();

	bool result = mPxScene->raycast(	Convert::toPx(start), 
										Convert::toPx(dir), 
										distance, 
										hit, 
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

	if (hit.hasBlock)
	{
		Collider* collider = reinterpret_cast<Collider*>(hit.block.shape->userData);
		if (collider)
		{
			hitInfo.setCollider(collider);

			Rigidbody* r = collider->rigidbody();
			hitInfo.setRigidbody(r);

			Transform* t = collider->transform();
			hitInfo.setTransform(t);
		}

		hitInfo.setBarycentricCoordinate(Vector2(hit.block.u, hit.block.v));
		hitInfo.setDistance(hit.block.distance);
		hitInfo.setNormal(Convert::toKernel(hit.block.normal));
		hitInfo.setPoint(Convert::toKernel(hit.block.position));
		hitInfo.setTriangleIndex(hit.block.faceIndex);
	}

	return result;
}

bool Scene::lineCast(const Vector3& start, const Vector3& end, unsigned layerMask)
{
	physx::PxRaycastBuffer hit;
	physx::PxQueryFilterData filterData;

	filterData.data.word0 = layerMask;
	filterData.flags |= physx::PxQueryFlag::ePREFILTER;

	mySceneQueryEventCallback.setMode(true);

	Vector3 dir = end - start;
	float distance = dir.normalise();

	bool result = mPxScene->raycast(	Convert::toPx(start), 
										Convert::toPx(dir), 
										distance, 
										hit, 
										physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eUV, 
										filterData, 
										&mySceneQueryEventCallback);

	return result;
}

physx::PxShape* Scene::createConvexMeshShape(physx::PxRigidActor* actor, Ogre::Entity *entity, float density, physx::PxMaterial &material, 
											   const Vector3 &scale, const physx::PxTransform &shapePos)
{
	physx::PxTransform shapeOffset = physx::PxTransform::createIdentity();

	Cooker::Params params;
	params.scale(scale);

	if (entity->getBoundingBox().getSize().x * scale.x > 0.4f &&
		entity->getBoundingBox().getSize().y * scale.y > 0.4f &&
		entity->getBoundingBox().getSize().z * scale.z > 0.4f)
	{
		physx::PxConvexMeshGeometry geo(Cooker::getSingleton().createPxConvexMesh(entity->getMesh(), params));
		return createShape(actor, geo, material, shapePos, density); 
	}

	//mesh is too small for convex mesh
	shapeOffset.p = Convert::toPx(entity->getBoundingBox().getCenter());
	physx::PxBoxGeometry boxGeometry = physx::PxBoxGeometry(Convert::toPx(entity->getBoundingBox().getHalfSize() * Convert::toOgre(scale)));
	physx::PxVec3 dim = boxGeometry.halfExtents;
	
	return createBoxShape(actor, dim, density, material, shapePos); 
}

physx::PxShape* Scene::createTriangleMeshShape(physx::PxRigidActor* actor, Ogre::Entity *entity, Cooker::Params &cookerParams)
{
	Cooker::AddedMaterials addedMaterials;
	cookerParams.materials(Physics->getOgreMaterialNames());
	physx::PxTriangleMeshGeometry geometry = physx::PxTriangleMeshGeometry(Cooker::getSingleton().createPxTriangleMesh(entity->getMesh(), cookerParams, &addedMaterials));
	physx::PxShape* shape;

	if (addedMaterials.materialCount > 0)
		shape = actor->createShape(geometry, addedMaterials.materials, addedMaterials.materialCount);
	else
		shape = createShape(actor, geometry, *Physics->getDefaultMaterial());

	return shape;
}

Transform* Scene::getRootTransform()
{
	return rootTransform;
}

void Scene::createRootTransform()
{
	rootGameObject = new GameObject("RGO_scene_" + getName(), this, true);
	rootTransform = rootGameObject->transform;
}

SimulationEventCallback& Scene::getSimulationEventCallback()
{
	return mySimulationEventCallback;
}

CharacterControllerCallback& Scene::getCharacterControllerCallback()
{
	return myCharacterControllerCallback;
}

SceneQueryEventCallback& Scene::getSceneQueryEventCallback()
{
	return mySceneQueryEventCallback;
}

void Scene::initControllerManager()
{
	if (!controllerManager)
	{
		controllerManager = PxCreateControllerManager(*mPxScene);
	}
}

void Scene::destroyControllerManager()
{
	if (controllerManager)
	{
		controllerManager->release();
		controllerManager = nullptr;
	}
}

physx::PxController* Scene::createCharacterController(const physx::PxCapsuleControllerDesc& capsuleDesc)
{
	if (!controllerManager)
		initControllerManager();

	return controllerManager->createController(capsuleDesc);
}

physx::PxController* Scene::createCharacterController(const physx::PxBoxControllerDesc& boxDesc)
{
	if (!controllerManager)
		initControllerManager();

	return controllerManager->createController(boxDesc);
}

void Scene::setShadowType(ShadowTechnique type, int textureSize)
{
	if ((type == SHADOW_STENCIL) || (type == SHADOW_STENCIL_ADDITIVE))
	{
		mSceneMgr->getRenderQueue()->
			getQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1)->
			setShadowsEnabled(false);
	}
	else
	{
		mSceneMgr->getRenderQueue()->
			getQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1)->
			setShadowsEnabled(true);
		mSceneMgr->setShadowTextureSize(textureSize);
	}
	mSceneMgr->setShadowTechnique((Ogre::ShadowTechnique)type);
}

//-----------------------------------------------------------------------------
void Scene::setShadowDistance(float distance)
{
	mSceneMgr->setShadowFarDistance(distance);
}

//-----------------------------------------------------------------------------
void Scene::setShadowColor(double r, double g, double b)
{
	mSceneMgr->setShadowColour(Ogre::ColourValue(r,g,b));
}

//-----------------------------------------------------------------------------
void Scene::setSkybox(const std::string &texture, double distance)
{
	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().
		create("DefaultSkybox", "General");
	mat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	mat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	Ogre::TextureUnitState* tus = mat->getTechnique(0)->getPass(0)->
		createTextureUnitState();
	tus->setCubicTextureName(texture);
	tus->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	mSceneMgr->setSkyBox(true, "DefaultSkybox", distance);
}

//-----------------------------------------------------------------------------
void Scene::setSkyDome(const std::string &texture, bool enable,
								 double curvature, double tilling, double distance)
{
	Ogre::MaterialManager::getSingleton().remove("skyMat");
	// Set up a material for the skydome
	Ogre::MaterialPtr skyMat =
		Ogre::MaterialManager::getSingleton().create("skyMat",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	// Perform no dynamic lighting on the sky
	skyMat->setLightingEnabled(false);
	// Use a cloudy sky
	Ogre::TextureUnitState* t = skyMat->getTechnique(0)->
		getPass(0)->createTextureUnitState(texture);
	// Scroll the clouds
	t->setScrollAnimation(0.015, 0);

	mSceneMgr->setSkyDome(enable, "skyMat", curvature, tilling, distance);
}

//-----------------------------------------------------------------------------
void Scene::setFog(double colorR, double colorG, double colorB,
							 double distNear, double distFar)
{
	mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(colorR,colorG,colorB),
		0, distNear, distFar);
}

//-----------------------------------------------------------------------------
void Scene::disableFog()
{
	mSceneMgr->setFog(Ogre::FOG_NONE);
}

//-----------------------------------------------------------------------------
void Scene::setAmbientLight(double r, double g, double b)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(r,g,b));
}

bool Scene::isColliding(const Ogre::Vector3 &oldPos, const Ogre::Vector3 &newPos, const float &radius, const unsigned &mask, const std::string &idName, const bool bounding)
{
	return collisionTools->collidesWithEntity(idName, Ogre::Vector3(oldPos.x, oldPos.y, oldPos.z), Ogre::Vector3(newPos.x, newPos.y,newPos.z), radius, 5, 1<<mask, bounding);
}

void Scene::initRayQueries()
{
	if (getOgreScene())
	{
		mRaySceneQuery = getOgreScene()->createRayQuery(Ogre::Ray());
		mVolQuery = getOgreScene()->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
	}
}

bool Scene::raycastRendering(const Ray& ray, unsigned layerMask)
{
	mRaySceneQuery->setQueryMask(layerMask);
	mVolQuery->setQueryMask(layerMask);

	Ogre::Entity *tmpE = NULL;
	Ogre::Vector3 result = Ogre::Vector3::ZERO;
	float distToColl;

	if (collisionTools->raycastFromCamera(Convert::toOgre(ray), result, tmpE, distToColl, layerMask))
	{
		Ogre::SceneNode* node = tmpE->getParentSceneNode();
		//return (char*)node->getName().c_str();
		return true;
	}

	return false;
}

bool Scene::raycastRendering(const Ray& ray, RaycastHit& hitInfo, unsigned layerMask)
{
	mRaySceneQuery->setQueryMask(layerMask);
	mVolQuery->setQueryMask(layerMask);

	Ogre::Entity *tmpE = NULL;
	Ogre::Vector3 result = Ogre::Vector3::ZERO;
	float distToColl;

	if (collisionTools->raycastFromCamera(Convert::toOgre(ray), result, tmpE, distToColl, layerMask))
	{
		Ogre::SceneNode* node = tmpE->getParentSceneNode();
		//return (char*)node->getName().c_str();

		hitInfo.setPoint(Convert::toKernel(result));
		hitInfo.setDistance(distToColl);
		
		std::string name = node->getName();
		name = StringUtil::split(name, "_Transform").at(0);

		GameObject* g = GameObjectManager::getInstance()->find(name);

		if (g)
			hitInfo.setTransform(g->transform);

		return true;
	}

	return false;
}

RaycastHitList Scene::raycastRenderingAll(const Ray& ray, float distance, unsigned layerMask)
{
	mRaySceneQuery->setQueryMask(layerMask);
	mVolQuery->setQueryMask(layerMask);

	RaycastHitList list;

	mRaySceneQuery->setRay(Convert::toOgre(ray));
	mRaySceneQuery->setSortByDistance(true);

	// execute query
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr;

	for (itr = result.begin(); itr != result.end(); ++itr)
	{
		if (itr->movable && itr->movable->isVisible() && itr->distance <= distance)
		{
			std::string name = itr->movable->getName();
			name = StringUtil::split(name, "_Transform").at(0);

			GameObject* g = GameObjectManager::getInstance()->find(name);

			if (g)
			{
				 RaycastHit hitInfo;
				 hitInfo.setTransform(g->transform);
				 hitInfo.setPoint(ray.getPoint(itr->distance));
				 hitInfo.setDistance(distance);

				 list.push_back(hitInfo);
			}
		}
	}

	return list;
}

RaycastHitList Scene::volumecastRendering(const Ray& topLeft, const Ray& topRight, const Ray& bottomLeft , const Ray& bottomRight, unsigned layerMask)
{
	mRaySceneQuery->setQueryMask(layerMask);
	mVolQuery->setQueryMask(layerMask);

	RaycastHitList list;


	Ogre::PlaneBoundedVolume vol;

	vol.planes.push_back(Ogre::Plane(Convert::toOgre(topLeft.getPoint(3)), 
									 Convert::toOgre(topRight.getPoint(3)), 
									 Convert::toOgre(bottomRight.getPoint(3))));
	
	vol.planes.push_back(Ogre::Plane(Convert::toOgre(topLeft.getOrigin()), 
		Convert::toOgre(topLeft.getPoint(100)), 
		Convert::toOgre(topRight.getPoint(100))));
	
	vol.planes.push_back(Ogre::Plane(Convert::toOgre(topLeft.getOrigin()), 
									Convert::toOgre(bottomLeft.getPoint(100)), 
									Convert::toOgre(topLeft.getPoint(100))));
	
	vol.planes.push_back(Ogre::Plane(Convert::toOgre(bottomLeft.getOrigin()), 
									Convert::toOgre(bottomRight.getPoint(100)), 
									Convert::toOgre(bottomLeft.getPoint(100))));
	
	vol.planes.push_back(Ogre::Plane(Convert::toOgre(topRight.getOrigin()), 
									Convert::toOgre(topRight.getPoint(100)), 
									Convert::toOgre(bottomRight.getPoint(100))));

	Ogre::PlaneBoundedVolumeList volList;
	volList.push_back(vol);

	mVolQuery->setVolumes(volList);	
	Ogre::SceneQueryResult& result = mVolQuery->execute();

	Ogre::SceneQueryResultMovableList::iterator iter;
	for(iter = result.movables.begin(); iter != result.movables.end(); iter++)
	{
		Ogre::MovableObject* obj = *iter;
		if (obj->getMovableType() == "Entity" && obj->isVisible())
		{
			std::string name = obj->getName();
			name = StringUtil::split(name, "_Transform").at(0);

			GameObject* g = GameObjectManager::getInstance()->find(name);

			if (g)
			{
				RaycastHit hitInfo;

				hitInfo.setTransform(g->transform);

				list.push_back(hitInfo);
			}
		}
	}

	return list;
}