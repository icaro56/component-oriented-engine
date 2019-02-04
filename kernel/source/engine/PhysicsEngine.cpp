#include "engine/PhysicsEngine.h"

#include "engine/Engine.h"
#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "foundation/PxFoundation.h"
#include "physxprofilesdk/PxProfileZoneManager.h"
#include "cooking/PxCooking.h"
#include "PxMaterial.h"
#include "gameobject/LayerMask.h"
#include "engine/Scene.h"

#include "vehicle/PxVehicleSDK.h"
#include "vehicle/PxVehicleDrive4W.h"
#include "vehicle/PxVehicleWheels.h"
#include "vehicle\PxVehicleUtil.h"

#include "utils\SnippetVehicleRaycast.h"
#include "utils\SnippetVehicleTireFriction.h"
#include "utils\KernelConverter.h"
#include "time/Time.h"

PhysicsEngine* PhysicsEngine::instance = nullptr;

PhysicsEngine* PhysicsEngine::getInstance()
{
	if (!instance)
	{
		instance = new PhysicsEngine();
	}

	return instance;
}

void PhysicsEngine::init()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if(!mFoundation)
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] Error: Foundation failed.");
		return;
	}

	mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
	if(!mProfileZoneManager)
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] Error: ProfileZoneManager failed.");
		return;
	}

	bool recordMemoryAllocations = true;
	physx::PxTolerancesScale scale = physx::PxTolerancesScale();

	mPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, 
		*mFoundation,
		scale,
		recordMemoryAllocations, 
		mProfileZoneManager );
	if(!mPhysicsSDK)
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] Error: ProfileZoneManager failed.");
		return;
	}


	physx::PxCookingParams params(scale);
	params.meshWeldTolerance = 0.001f;
	params.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES | physx::PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES | physx::PxMeshPreprocessingFlag::eREMOVE_DUPLICATED_TRIANGLES);
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, params);
	if (!mCooking)
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] Error: Creating Cooking failed.");
		return;
	}

	if (!PxInitExtensions(*mPhysicsSDK))
	{
		Ogre::LogManager::getSingleton().logMessage("[PhysX] Error: InitExtensions failed.");
		return;
	}


	if (mPhysicsSDK->getPvdConnectionManager())
	{
		mPhysicsSDK->getVisualDebugger()->setVisualizeConstraints(true);
		mPhysicsSDK->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		mPhysicsSDK->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		mConnection = physx::PxVisualDebuggerExt::createConnection(mPhysicsSDK->getPvdConnectionManager(), "localhost", 5425, 10);
	}

	//create default material
	mDefaultMaterial = mPhysicsSDK->createMaterial(0.5f, 0.5f, 0.6f);

	Ogre::LogManager::getSingleton().logMessage("[PhysX] SDK created.");
}

void PhysicsEngine::destroy()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

PhysicsEngine::PhysicsEngine()
	:mPhysicsSDK(nullptr),
	 mFoundation(nullptr),
	 mProfileZoneManager(nullptr),
	 mCooking(nullptr),
	 mDefaultMaterial(nullptr),
	 mConnection(nullptr),
	 defaultGravity(Vector3(0, -9.81f, 0)),
	 defaultBounceThreshold(0.2f * defaultGravity.length()),
	 defaultMaxAngularVelocity(7.0f),
	 initiazeVehicleSDK(false),
	 gVehicleSceneQueryData(nullptr),
	 gBatchQuery(nullptr),
	 gFrictionPairs(nullptr)
{
}

PhysicsEngine::~PhysicsEngine()
{
	destroyVehicleSDK();

	mDefaultMaterial->release();

	if (mCooking)
	{
		mCooking->release();
		mCooking = nullptr;
	}

	if (mConnection)
	{
		mConnection->release();
		mConnection = nullptr;
	}

	if (mPhysicsSDK)
	{
		mPhysicsSDK->release();
		mPhysicsSDK = nullptr;
	}

	if (mProfileZoneManager)
	{
		mProfileZoneManager->release();
		mProfileZoneManager = nullptr;
	}

	PxCloseExtensions();

	if (mFoundation)
	{
		mFoundation->release();
		mFoundation = nullptr;
	}
}

physx::PxPhysics* PhysicsEngine::getPhysics()
{
	return mPhysicsSDK;
}

physx::PxCooking* PhysicsEngine::getCooking()
{
	return mCooking;
}

bool PhysicsEngine::initVehicleSDK(Scene* scene)
{
	//inicializando o SDK dos veículos
	PxInitVehicleSDK(*mPhysicsSDK);

	//configurando os vetores base
	physx::PxVec3 up(0, 1, 0);
	physx::PxVec3 forward(0, 0, 1);
	physx::PxVehicleSetBasisVectors(up, forward);

	//Configurando o modo de atualização do veículo para ser imediata as trocas de velocidade
	physx::PxVehicleSetUpdateMode(physx::PxVehicleUpdateMode::eVELOCITY_CHANGE);

	//Criando os scenequeries para veículos
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, gDefaultAllocatorCallback);
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, scene->getPhysxScene());

	//Criando tabela de contato entre cada pneu e superfície
	gFrictionPairs = createFrictionPairs(mDefaultMaterial);


		

	initiazeVehicleSDK = true;

	return initiazeVehicleSDK;
}

bool PhysicsEngine::destroyVehicleSDK()
{
	if (initiazeVehicleSDK)
	{
		if (gBatchQuery)
		{
			//gBatchQuery->release();
			gBatchQuery = nullptr;
		}

		if (gVehicleSceneQueryData)
		{
			gVehicleSceneQueryData->free(gDefaultAllocatorCallback);
			gVehicleSceneQueryData = nullptr;
		}

		if (gFrictionPairs)
		{
			gFrictionPairs->release();
			gFrictionPairs = nullptr;
		}


		physx::PxCloseVehicleSDK();
	}


	initiazeVehicleSDK = false;

	return initiazeVehicleSDK;
}

void PhysicsEngine::registerOgreMaterialName(const std::string &matName, physx::PxMaterial *pxMat)
{
	auto i = mMaterialBindings.find(matName);
	if (i != mMaterialBindings.end()) 
		i->second = pxMat;
	else 
	{
		std::pair<std::string, physx::PxMaterial*> par;
		par = std::make_pair(matName, pxMat);
		mMaterialBindings.insert(par);
	}
}

physx::PxMaterial* PhysicsEngine::getOgreMaterialByName(const std::string &matName)
{
	auto i  = mMaterialBindings.find(matName);
	if (i != mMaterialBindings.end()) 
		return i->second;

	return mDefaultMaterial;
}

physx::PxMaterial* PhysicsEngine::getDefaultMaterial()
{
	return mDefaultMaterial;
}

unsigned PhysicsEngine::AllLayers()
{
	return 0xffffffff;
}

unsigned PhysicsEngine::DefaultRaycastLayers()
{
	return LayerMask::nameToLayer("Default");
}

unsigned PhysicsEngine::IgnoreRaycastLayer()
{
	return LayerMask::nameToLayer("IgnoreRaycast");
}

void PhysicsEngine::setBounceThreshold(float b)
{
	defaultBounceThreshold = b;
}

float PhysicsEngine::getBounceThreshold()
{
	return defaultBounceThreshold;
}

void PhysicsEngine::setGravity(const Vector3& g)
{
	defaultGravity = g;
}

Vector3 PhysicsEngine::getGravity()
{
	return defaultGravity;
}

void PhysicsEngine::setMaxAngularVelocity(float m)
{
	defaultMaxAngularVelocity = m;
}

float PhysicsEngine::getMaxAngularVelocity()
{
	return defaultMaxAngularVelocity;
}

bool PhysicsEngine::raycast(const Vector3& origin, const Vector3& direction, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycast(origin, direction, distance, layerMask);
}

bool PhysicsEngine::raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hitInfo, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycast(origin, direction, hitInfo, distance, layerMask);
}

bool PhysicsEngine::raycast(const Ray& ray, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycast(ray, distance, layerMask);
}

bool PhysicsEngine::raycast(const Ray& ray, RaycastHit& hitInfo, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	
	return scene->raycast(ray, hitInfo, distance, layerMask);
}

RaycastHitList PhysicsEngine::raycastAll(const Vector3& origin, const Vector3& direction, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycastAll(origin, direction, distance, layerMask);
}

RaycastHitList PhysicsEngine::raycastAll(const Ray& ray, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycastAll(ray, distance, layerMask);
}

bool PhysicsEngine::sweep(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, RaycastHit& hitInfo, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->sweep(geo, pos, ori, direction, hitInfo, distance, layerMask);
}

RaycastHitList PhysicsEngine::sweepAll(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->sweepAll(geo, pos, ori, direction, distance, layerMask);
}

bool PhysicsEngine::sphereCast(const Vector3& origin, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->sphereCast(origin, radius, direction, hitInfo, distance, layerMask);
}

bool PhysicsEngine::sphereCast(const Ray& ray, float radius, RaycastHit& hitInfo, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->sphereCast(ray, radius, hitInfo, distance, layerMask);
}

bool PhysicsEngine::sphereCast(const Ray& ray, float radius, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->sphereCast(ray, radius, distance, layerMask);
}

RaycastHitList PhysicsEngine::sphereCastAll(const Ray& ray, float radius, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->sphereCastAll(ray, radius, distance, layerMask);
}

RaycastHitList PhysicsEngine::sphereCastAll(const Vector3& origin, float radius, const Vector3& direction, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->sphereCastAll(origin, radius, direction, distance, layerMask);
}

bool PhysicsEngine::capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->capsuleCast(point1, point2, radius, direction, hitInfo, distance, layerMask);
}

bool PhysicsEngine::capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->capsuleCast(point1, point2, radius, direction, distance, layerMask);
}

RaycastHitList PhysicsEngine::capsuleCastAll(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->capsuleCastAll(point1, point2, radius, direction, distance, layerMask);
}

ColliderVector PhysicsEngine::overlapSphere(const Vector3& position, float radius, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->overlapSphere(position, radius, layerMask);
}

bool PhysicsEngine::checkSphere(const Vector3& position, float radius, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->checkSphere(position, radius, layerMask);
}

bool PhysicsEngine::checkCapsule(const Vector3& start, const Vector3& end, float radius, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->checkCapsule(start, end, radius, layerMask);
}

ColliderVector PhysicsEngine::overlap(Geometry* geo,const Vector3& pos, const Quaternion& ori, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->overlap(geo, pos, ori, layerMask);
}

bool PhysicsEngine::lineCast(const Vector3& start, const Vector3& end, RaycastHit& hitInfo, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->lineCast(start, end, hitInfo, layerMask);
}

bool PhysicsEngine::lineCast(const Vector3& start, const Vector3& end, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->lineCast(start, end, layerMask);
}

bool PhysicsEngine::updateVehicles(physx::PxVehicleDrive4W* gVehicle4W)
{
	//Raycasts.
	physx::PxVehicleWheels* vehicles[1] = {gVehicle4W};
	physx::PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
	const physx::PxU32 raycastResultsSize = gVehicleSceneQueryData->getRaycastQueryResultBufferSize();
	PxVehicleSuspensionRaycasts(gBatchQuery, 1, vehicles, raycastResultsSize, raycastResults);

	//Vehicle update.
	const physx::PxVec3 grav = Convert::toPx(getGravity());
	physx::PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
	physx::PxVehicleWheelQueryResult vehicleQueryResults[1] = {{wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()}};
	PxVehicleUpdates(Time->getFixedDeltaTime(), grav, *gFrictionPairs, 1, vehicles, vehicleQueryResults);

	return (gVehicle4W->getRigidDynamicActor()->isSleeping() ? false : physx::PxVehicleIsInAir(vehicleQueryResults[0]));
}