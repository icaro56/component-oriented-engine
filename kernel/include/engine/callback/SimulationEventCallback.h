#ifndef SIMULATION_EVENT_CALLBACK_H
#define SIMULATION_EVENT_CALLBACK_H

#include "PxSimulationEventCallback.h"
#include "KernelPrerequisites.h"
#include <vector>

class SimulationEventCallback : public physx::PxSimulationEventCallback
{
public:
	SimulationEventCallback();

	// Implements PxSimulationEventCallback:
	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
	virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) {}
	virtual void onWake(physx::PxActor** , physx::PxU32 ) {}
	virtual void onSleep(physx::PxActor** , physx::PxU32 ){}
	//

	/*!
		\brief Função que configura actor para colisão
		\param actor
		\param filterGroup grupo do actor
		\param filterMask reportar colisão com esses grupos informados.
	*/
	void setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterLayer, physx::PxU32 filterLayerID, bool reportCollision);
	void setupFiltering(physx::PxShape* shape, physx::PxU32 filterLayer, physx::PxU32 filterLayerID, bool reportCollision);

	// Implements PxSimulationFilterShader
	static physx::PxFilterFlags filter(	physx::PxFilterObjectAttributes attributes0, 
										physx::PxFilterData filterData0, 
										physx::PxFilterObjectAttributes attributes1, 
										physx::PxFilterData filterData1,
										physx::PxPairFlags& pairFlags,
										const void* constantBlock,
										physx::PxU32 constantBlockSize);

	void update();

	const std::vector<physx::PxVec3> & getContactPositions();
	const std::vector<physx::PxVec3> & getContactImpulses();
	const std::vector<physx::PxVec3> & getContactNormals();

private:
	std::vector<physx::PxVec3> contactPositions;
	std::vector<physx::PxVec3> contactImpulses;
	std::vector<physx::PxVec3> contactNormals;

	bool generateDebugContacts;
};

#endif