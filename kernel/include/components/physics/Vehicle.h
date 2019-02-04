#ifndef VEHICLE_HH
#define VEHICLE_HH

#include "KernelPrerequisites.h"
#include "../Component.h"

#include "math/Vector3.h"

#include "vehicle/PxVehicleDrive4W.h"
#include "vehicle/PxVehicleComponents.h"
#include "vehicle/PxVehicleUtilControl.h"
#include "utils\SnippetVehicleCreate.h"

namespace physx
{
	class PxConvexMesh;
	class PxVehicleDrive4W;
	class PxBatchQuery;
}

enum DriveMode
{
	eDRIVE_MODE_ACCEL_FORWARDS=0,
	eDRIVE_MODE_ACCEL_REVERSE,
	eDRIVE_MODE_HARD_TURN_LEFT,
	eDRIVE_MODE_HANDBRAKE_TURN_LEFT,
	eDRIVE_MODE_HARD_TURN_RIGHT,
	eDRIVE_MODE_HANDBRAKE_TURN_RIGHT,
	eDRIVE_MODE_BRAKE,
	eDRIVE_MODE_NONE
};

DriveMode gDriveModeOrder[] =
{
	eDRIVE_MODE_BRAKE,
	eDRIVE_MODE_ACCEL_FORWARDS,
	eDRIVE_MODE_BRAKE,
	eDRIVE_MODE_ACCEL_REVERSE,
	eDRIVE_MODE_BRAKE,
	eDRIVE_MODE_HARD_TURN_LEFT, 
	eDRIVE_MODE_BRAKE,
	eDRIVE_MODE_HARD_TURN_RIGHT,
	eDRIVE_MODE_ACCEL_FORWARDS,
	eDRIVE_MODE_HANDBRAKE_TURN_LEFT,
	eDRIVE_MODE_ACCEL_FORWARDS,
	eDRIVE_MODE_HANDBRAKE_TURN_RIGHT,
	eDRIVE_MODE_NONE
};

class KERNEL_VS_EXPORT Vehicle : public Component
{
public:
	/*enum VehicleDesc
	{
		CCD_ChassisMass,
		
		
		CCD_Size
	};

	typedef std::map<VehicleDesc, std::string> VehicleDescMap;*/

	 /**
      * @brief Construtor Default
    */
	Vehicle();

	/**
      * @brief Construtor 
      * @param idComponent Nome do Componente
     */
	Vehicle(std::string idComponent);

	/**
      * @brief Construtor 
      * @param &d Descritor da classe
     */
	//Vehicle(const VehicleDescMap &d);

	/**
      * @brief Destrutor Default 
    */
	~Vehicle();

	//reimplementando para sobrescrever implementação do pai.
	virtual void prerequisites();

	/**
      * @brief Inicializa o Component 
      * @param name Nome do GameObject
     */
	virtual void init(const std::string& name);
	virtual void nextInit();

	/**
      * @brief Ativa ou desativa o Componente 
      * @param e booleano de ativação
    */
	virtual void setEnabled(bool e);

	/**
      * @brief Acessa se o Componente esta ativado
    */
	virtual bool isEnabled();

	virtual void fixedUpdate();

	void setChassisMass(float mass);
	float getChassisMass();

	void setChassisConvexMesh(const std::string& chassisMeshName);
	void setWheelsConvexMesh(const StringVector& list);

private:

	virtual void applyDesc(int key, const std::string& value);

	VehicleDesc initVehicleDesc();
	void startBrakeMode();
	void incrementDrivingMode();
	void releaseAllControls();
	void startAccelerateForwardsMode();
	void startAccelerateReverseMode();
	void startTurnHardLeftMode();
	void startTurnHardRightMode();
	void startHandbrakeTurnLeftMode();
	void startHandbrakeTurnRightMode();
private:

	//VehicleDescMap mVehicleDescMap;

	physx::PxConvexMesh* pxChassisConvexMesh;
	std::vector<physx::PxConvexMesh*> pxWheelsConvexMeshList;

	//***********************************
	physx::PxVehicleDrive4W* gVehicle4W;
	Real gVehicleModeTimer;
	Real gVehicleModeLifetime;
	unsigned int gVehicleOrderProgress;
	bool gMimicKeyInputs;
	bool gVehicleOrderComplete;
	bool gIsVehicleInAir;
	physx::PxVehicleDrive4WRawInputData gVehicleInputData;
};

#endif