#ifndef CHARACTER_CONTROLLER_CALLBACK
#define CHARACTER_CONTROLLER_CALLBACK

#include "characterkinematic/PxController.h" // for PxUserControllerHitReport
#include "characterkinematic/PxControllerBehavior.h"
#include "PxQueryFiltering.h"
#include "PxFiltering.h"

class CharacterControllerCallback : public physx::PxUserControllerHitReport,
									public physx::PxControllerBehaviorCallback,
									public physx::PxQueryFilterCallback,
									public physx::PxControllerFilterCallback
{
public:
	CharacterControllerCallback();
	~CharacterControllerCallback();

	// Implements PxUserControllerHitReport
	virtual void							onShapeHit(const physx::PxControllerShapeHit& hit);
	virtual void							onControllerHit(const physx::PxControllersHit& hit);
	virtual void							onObstacleHit(const physx::PxControllerObstacleHit& hit)	{}

	// Implements PxControllerBehaviorCallback
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags(const physx::PxShape&, const physx::PxActor&)	{ return physx::PxControllerBehaviorFlags(0);	}
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags(const physx::PxController&)				{ return physx::PxControllerBehaviorFlags(0);	}
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags(const physx::PxObstacle&)					{ return physx::PxControllerBehaviorFlags(0);	}

	// Implements PxSceneQueryFilterCallback
	virtual physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags);
	
	virtual	physx::PxQueryHitType::Enum	postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit);

	// Implements PxControllerFilterCallback
	virtual bool filter(const physx::PxController& a, const physx::PxController& b);

private:
};

#endif