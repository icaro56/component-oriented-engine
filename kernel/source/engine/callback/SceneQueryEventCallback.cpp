#include "engine/callback/SceneQueryEventCallback.h"
#include "gameobject/LayerMask.h"
#include "physics/Collider.h"

SceneQueryEventCallback::SceneQueryEventCallback()
	: isBlock(true)
{

}

physx::PxQueryHitType::Enum	SceneQueryEventCallback::preFilter(const physx::PxFilterData& filterData, 
																   const physx::PxShape* shape, 
																   const physx::PxRigidActor* actor, 
																   physx::PxHitFlags& queryFlags)
{
	Collider* collider = reinterpret_cast<Collider*>(shape->userData);

	if (collider)
	{
		const physx::PxU32* flags = LayerMask::groupCollisionFlags();
		//unsigned idLayerCollisions =  flags[filterData.word1];
		unsigned idLayerCollisions =  flags[collider->getLayerId()];

		bool col = (idLayerCollisions & filterData.word0);

		if (col)
		{
			if (isBlock)
			{
				if (!collider->getRayPass())
					return physx::PxQueryHitType::eBLOCK;
				else
					return physx::PxQueryHitType::eTOUCH;
			}
			else
				return physx::PxQueryHitType::eTOUCH;
		}
	}
	else
	{
		return physx::PxQueryHitType::eNONE;
	}
}

physx::PxQueryHitType::Enum	SceneQueryEventCallback::postFilter(const physx::PxFilterData& filterData, 
																const physx::PxQueryHit& hit)
{
	Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);

	if (collider)
	{
		const physx::PxU32* flags = LayerMask::groupCollisionFlags();
		//unsigned idLayerCollisions =  flags[filterData.word1];
		unsigned idLayerCollisions =  flags[collider->getLayerId()];

		bool col = (idLayerCollisions & filterData.word0);

		if (col)
		{
			if (isBlock)
			{
				if (!collider->getRayPass())
					return physx::PxQueryHitType::eBLOCK;
				else
					return physx::PxQueryHitType::eTOUCH;
			}
			else
				return physx::PxQueryHitType::eTOUCH;
		}
	}
	else
	{
		return physx::PxQueryHitType::eNONE;
	}
}

void SceneQueryEventCallback::setMode(bool block)
{
	isBlock = block;
}