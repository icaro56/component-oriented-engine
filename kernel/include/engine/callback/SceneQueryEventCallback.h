#ifndef SCENE_QUERY_EVENT_CALLBACK_H
#define SCENE_QUERY_EVENT_CALLBACK_H

#include "PxQueryFiltering.h"

class SceneQueryEventCallback : public physx::PxQueryFilterCallback
{
public:
	SceneQueryEventCallback();

	// Implements PxSceneQueryFilterCallback
	virtual physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags);

	virtual	physx::PxQueryHitType::Enum	postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit);

	void setMode(bool block);

private:
	bool isBlock;
};

#endif