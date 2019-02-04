#include "engine/callback/SimulationEventCallback.h"
#include "PxRigidActor.h"

#include "physics/Collider.h"
#include "physics/Rigidbody.h"
#include "utils/KernelConverter.h"
#include "gameobject/GameObjectManager.h"
#include "gameobject/GameObject.h"
#include "gameobject/LayerMask.h"

#include "utils/SnippetVehicleRaycast.h"

#include "math/Vector3.h"

SimulationEventCallback::SimulationEventCallback()
	: generateDebugContacts(true)
{

}

void SimulationEventCallback::setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterLayer, physx::PxU32 filterLayerID, bool reportCollision)
{
	physx::PxFilterData filterData;
	filterData.word0 = filterLayer; 
	filterData.word1 = filterLayerID;
	filterData.word2 = reportCollision;	

	//código temporário para veículo
	setupDrivableSurface(filterData);

	const physx::PxU32 numShapes = actor->getNbShapes();

	physx::PxShape **shapes = new physx::PxShape*[numShapes];
	actor->getShapes(shapes, numShapes);

	for(physx::PxU32 i = 0; i < numShapes; i++)
	{
		physx::PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
		shape->setQueryFilterData(filterData);

	}

	delete [] shapes;
}

void SimulationEventCallback::setupFiltering(physx::PxShape* shape, physx::PxU32 filterLayer, physx::PxU32 filterLayerID, bool reportCollision)
{
	physx::PxFilterData filterData;
	filterData.word0 = filterLayer;
	filterData.word1 = filterLayerID;
	filterData.word2 = reportCollision;	

	//código temporário para veículo
	setupDrivableSurface(filterData);

	if (shape)
	{
		shape->setSimulationFilterData(filterData);
		shape->setQueryFilterData(filterData);
	}
}

void SimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{	
	if (pairHeader.flags & (physx::PxContactPairHeaderFlag::eDELETED_ACTOR_0 | physx::PxContactPairHeaderFlag::eDELETED_ACTOR_1))
		return;

	std::vector<physx::PxContactPairPoint> contactPoints;
	Collision collision, collision2;

	for(physx::PxU32 i=0; i < nbPairs; i++)
	{
		physx::PxU32 contactCount = pairs[i].contactCount;
		if(contactCount)
		{
			contactPoints.resize(contactCount);
			pairs[i].extractContacts(&contactPoints[0], contactCount);

			for(physx::PxU32 j=0;j<contactCount;j++)
			{
				contactPositions.push_back(contactPoints[j].position);
				contactImpulses.push_back(contactPoints[j].impulse);
				contactNormals.push_back(contactPoints[j].normal);

				ContactPoint contactPoint, contactPoint2;
				contactPoint.point = Convert::toOgre(contactPoints[j].position);
				contactPoint.normal = Convert::toOgre(contactPoints[j].normal);
				contactPoint.impulse = Convert::toOgre(contactPoints[j].impulse);
				contactPoint.thisCollider = reinterpret_cast<Collider*>(pairs[i].shapes[0]->userData);
				contactPoint.otherCollider = reinterpret_cast<Collider*>(pairs[i].shapes[1]->userData);

				contactPoint2.point = contactPoint.point;
				contactPoint2.normal = contactPoint.normal;
				contactPoint2.impulse = contactPoint.impulse;
				contactPoint2.thisCollider = contactPoint.otherCollider;
				contactPoint2.otherCollider = contactPoint.thisCollider;

				collision.contactPoints.push_back(contactPoint);
				collision2.contactPoints.push_back(contactPoint2);
			}
		}

		const physx::PxContactPair& cp = pairs[i];

		if (cp.events & (physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | 
						 physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | 
						 physx::PxPairFlag::eNOTIFY_TOUCH_LOST))
		{
			Collider* collider = reinterpret_cast<Collider*>(cp.shapes[0]->userData);
			Collider* collider2 = reinterpret_cast<Collider*>(cp.shapes[1]->userData);

			GameObject* gameobject1 = nullptr;
			GameObject* gameobject2 = nullptr;

			/*gameobject1 = GameObjectManager::getInstance()->find(pairHeader.actors[0]->getName());
			gameobject2 = GameObjectManager::getInstance()->find(pairHeader.actors[1]->getName());*/

			if (collider)
				gameobject1 = collider->getGameObject();

			if (collider2)
				gameobject2 = collider2->getGameObject();

			collision.setGameObject(gameobject2);
			collision2.setGameObject(gameobject1);

			if (gameobject1 && gameobject2)
			{
				if (gameobject1->rigidbody && gameobject2->rigidbody)
				{
					Vector3 vel = gameobject1->rigidbody->getVelocity() + gameobject2->rigidbody->getVelocity();
					collision.setRelativeVelocity(vel);
					collision2.setRelativeVelocity(vel);
				}
			}
		}
		
		if(cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (collision2.getCollider())
				collision2.getCollider()->informOnCollisionEnter(collision);

			if (collision.getCollider())
				collision.getCollider()->informOnCollisionEnter(collision2);
		}
		else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			if (collision2.getCollider())
				collision2.getCollider()->informOnCollisionStay(collision);

			if (collision.getCollider())
				collision.getCollider()->informOnCollisionStay(collision2);
		}
		else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (collision2.getCollider())
				collision2.getCollider()->informOnCollisionExit(collision);

			if (collision.getCollider())
				collision.getCollider()->informOnCollisionExit(collision2);
		}

	}
}

///////////////////////////////////////////////////////////////////////////////

void SimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for(physx::PxU32 i=0; i < count; i++)
	{
		// ignore pairs when shapes have been deleted
		if (pairs[i].flags & (physx::PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;

		Collider* triggerCollider = reinterpret_cast<Collider*>(pairs[i].triggerShape->userData);
		Collider* collider = reinterpret_cast<Collider*>(pairs[i].otherShape->userData);

		if (pairs[i].status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (triggerCollider && collider)
			{
				triggerCollider->informOnTriggerEnter(collider);
				collider->informOnTriggerEnter(triggerCollider);
			}
		}
		else if (pairs[i].status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (triggerCollider && collider)
			{
				triggerCollider->informOnTriggerExit(collider);
				collider->informOnTriggerExit(triggerCollider);
			}
		}

	}
}



void SimulationEventCallback::update()
{
	contactImpulses.clear();
	contactPositions.clear();
	contactNormals.clear();
}

const std::vector<physx::PxVec3> & SimulationEventCallback::getContactPositions()
{
	return contactPositions;
}

const std::vector<physx::PxVec3> & SimulationEventCallback::getContactImpulses()
{
	return contactImpulses;
}

const std::vector<physx::PxVec3> & SimulationEventCallback::getContactNormals()
{
	return contactNormals;
}

physx::PxFilterFlags SimulationEventCallback::filter(physx::PxFilterObjectAttributes attributes0, 
													 physx::PxFilterData filterData0, 
													 physx::PxFilterObjectAttributes attributes1, 
													 physx::PxFilterData filterData1, 
													 physx::PxPairFlags& pairFlags, 
													 const void* constantBlock, 
													 physx::PxU32 constantBlockSize)
{
	/* forma antiga que simula a forma de interação fixa entre grupos da physx 2.x
	physx::PxU32 ShapeGroup0 = filterData0.word2 & 0xffffffff;
	physx::PxU32 ShapeGroup1 = filterData1.word2 & 0xffffffff;
	physx::PxU32* groupCollisionFlags = (physx::PxU32*)constantBlock;

	if ((groupCollisionFlags[ShapeGroup0] & (1 << ShapeGroup1)) == 0)
		return physx::PxFilterFlag::eSUPPRESS;*/

	const physx::PxU32* flags = LayerMask::groupCollisionFlags();
	unsigned idLayerCollisions =  flags[filterData0.word1];

	bool col = (idLayerCollisions & filterData1.word0);
	
	//bool col = ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1));

	if ( !col)
	{
		return physx::PxFilterFlag::eSUPPRESS;
	}

	//se é para reportar colisão. Ou seja, os dois Objetos precisam estar com a flag report ativa
	if ( (filterData0.word2) && (filterData1.word2))
	{
		// let triggers through
		if(physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlag::eDEFAULT;
		}

		// generate contacts for all that were not filtered above
		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

		// trigger the contact callback for pairs (A,B) where
		// the filtermask of A contains the ID of B and vice versa.
		if(col)
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND		 | 
				physx::PxPairFlag::eNOTIFY_TOUCH_LOST		 | 
				physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS   |
				physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}
	}
	
	return physx::PxFilterFlag::eDEFAULT;
}


