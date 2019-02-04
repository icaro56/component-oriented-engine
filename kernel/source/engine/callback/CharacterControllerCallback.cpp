#include "engine/callback/CharacterControllerCallback.h"

#include "PxRigidActor.h"
#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "physics/CharacterController.h"
#include "utils/KernelConverter.h"
#include "gameobject/LayerMask.h"

CharacterControllerCallback::CharacterControllerCallback()
{

}

CharacterControllerCallback::~CharacterControllerCallback()
{

}

void CharacterControllerCallback::onShapeHit(const physx::PxControllerShapeHit& hit)
{
	if (hit.actor)
	{
		Collider* collider = reinterpret_cast<Collider*>(hit.shape->userData);

		if (collider)
		{
			GameObject* gameObject = collider->getGameObject();

			ControllerColliderHit colliderHit;

			colliderHit.setGameObject(gameObject);
			colliderHit.setMoveDirection(Convert::toKernel(hit.dir));
			colliderHit.setMoveLength(hit.length);
			colliderHit.setNormal(Convert::toKernel(hit.worldNormal));
			colliderHit.setPoint(Convert::toKernel((hit.worldPos)));

			CharacterController* controller = reinterpret_cast<CharacterController*>(hit.controller->getUserData());
			
			if (controller)
			{
				controller->informOnControllerColliderHit(colliderHit);
			}
		
		}
	}
}

void CharacterControllerCallback::onControllerHit(const physx::PxControllersHit& hit)
{
	if (hit.controller)
	{
		CharacterController* controller = reinterpret_cast<CharacterController*>(hit.controller->getUserData());
		CharacterController* otherController = reinterpret_cast<CharacterController*>(hit.other->getUserData());

		if (controller && otherController)
		{
			ControllerControllerHit controllerHit, controllerHit2;

			controllerHit.setMoveDirection(Convert::toKernel(hit.dir));
			controllerHit.setMoveLength(hit.length);
			controllerHit.setNormal(Convert::toKernel(hit.worldNormal));
			controllerHit.setPoint(Convert::toKernel(hit.worldPos));

			controllerHit2.setMoveDirection(Convert::toKernel(hit.dir));
			controllerHit2.setMoveLength(hit.length);
			controllerHit2.setNormal(Convert::toKernel(hit.worldNormal));
			controllerHit2.setPoint(Convert::toKernel(hit.worldPos));


			controllerHit.setController(controller);
			controllerHit.setOtherController(otherController);

			controllerHit2.setController(otherController);
			controllerHit2.setOtherController(controller);

			controller->informOnControllerControllerHit(controllerHit);
			otherController->informOnControllerControllerHit(controllerHit2);
		}
	}
}

physx::PxQueryHitType::Enum	CharacterControllerCallback::preFilter(const physx::PxFilterData& filterData, 
																   const physx::PxShape* shape, 
																   const physx::PxRigidActor* actor, 
																   physx::PxHitFlags& queryFlags)
{
	Collider* collider = reinterpret_cast<Collider*>(shape->userData);

	if (collider)
	{
		const physx::PxU32* flags = LayerMask::groupCollisionFlags();
		unsigned idLayerCollisions =  flags[filterData.word1];

		bool col = (idLayerCollisions & collider->getLayer());

		/*if ( (filterData.word0 & collider->getFilterMask() ) && 
			 (collider->getFilterGroup() & filterData.word1) )*/
		if (col)
		{
			return physx::PxQueryHitType::eBLOCK;
		}
	}
	else
	{
		return physx::PxQueryHitType::eNONE;
	}
}

physx::PxQueryHitType::Enum	CharacterControllerCallback::postFilter(const physx::PxFilterData& filterData, 
																	const physx::PxQueryHit& hit)
{
	return physx::PxQueryHitType::eBLOCK;
}

bool CharacterControllerCallback::filter(const physx::PxController& a, const physx::PxController& b)
{

	CharacterController* controller = reinterpret_cast<CharacterController*>(a.getUserData());
	CharacterController* otherController = reinterpret_cast<CharacterController*>(b.getUserData());

	if (controller && otherController)
	{
		const physx::PxU32* flags = LayerMask::groupCollisionFlags();
		unsigned idLayerCollisions =  flags[controller->getLayerId()];
		
		bool col = (idLayerCollisions & otherController->getLayer());

		/*if((controller->getFilterData()->word0 & otherController->getFilterData()->word1) && 
		   (otherController->getFilterData()->word0 & controller->getFilterData()->word1))*/
		if (col)
		{
			return true;
		}
	}

	return false;
}