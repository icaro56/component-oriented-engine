#include "physics/Vehicle.h"
#include "GameObject.h"
#include "transform\Transform.h"
#include "utils/KernelConverter.h"
#include "utils/Cooker.h"
#include "utils/DebugClass.h"


#include "utils\SnippetVehicleRaycast.h"
#include "engine\PhysicsEngine.h"
#include "vehicle\PxVehicleDrive4W.h"
#include "vehicle\PxVehicleUtil.h"
#include "engine/Engine.h"
#include "PxPhysics.h"
#include "engine/Scene.h"
#include "time\Time.h"

PxF32 gSteerVsForwardSpeedData[2*8]=
{
	0.0f,		0.75f,
	5.0f,		0.75f,
	30.0f,		0.125f,
	120.0f,		0.1f,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32,
	PX_MAX_F32, PX_MAX_F32
};
PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable(gSteerVsForwardSpeedData,4);

PxVehicleKeySmoothingData gKeySmoothingData=
{
	{
		6.0f,	//rise rate eANALOG_INPUT_ACCEL
		6.0f,	//rise rate eANALOG_INPUT_BRAKE		
		6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
		2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
		2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
	},
	{
		10.0f,	//fall rate eANALOG_INPUT_ACCEL
		10.0f,	//fall rate eANALOG_INPUT_BRAKE		
		10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
		5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
		5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
	}
};

PxVehiclePadSmoothingData gPadSmoothingData=
{
	{
		6.0f,	//rise rate eANALOG_INPUT_ACCEL
		6.0f,	//rise rate eANALOG_INPUT_BRAKE		
		6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
		2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
		2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
	},
	{
		10.0f,	//fall rate eANALOG_INPUT_ACCEL
		10.0f,	//fall rate eANALOG_INPUT_BRAKE		
		10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
		5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
		5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
	}
};

Vehicle::Vehicle()
	: Component("Vehicle", "Vehicle"),
	  gVehicle4W(nullptr),
	  gVehicleModeTimer(0.0f),
	  gVehicleOrderProgress(0),
	  gMimicKeyInputs(false),
	  gVehicleModeLifetime(4.0f),
	  gVehicleOrderComplete(false),
	  gIsVehicleInAir(true)
{

}

Vehicle::Vehicle(std::string idComponent)
	: Component(idComponent, "Vehicle"),
	gVehicle4W(nullptr),
	gVehicleModeTimer(0.0f),
	gVehicleOrderProgress(0),
	gMimicKeyInputs(false),
	gVehicleModeLifetime(4.0f),
	gVehicleOrderComplete(false),
	gIsVehicleInAir(true)
{

}

/*Vehicle::Vehicle(const VehicleDescMap &d)
	: Component("Vehicle", "Vehicle"),
	  mVehicleDescMap(d)
	
{

}*/

Vehicle::~Vehicle()
{
	if (gVehicle4W)
	{
		gVehicle4W->getRigidDynamicActor()->release();
		gVehicle4W->free();
		gVehicle4W = nullptr;
	}
}

void Vehicle::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void Vehicle::init(const std::string& name)
{
	if (!initialized)
	{
		if (!Physics->isStartedVehicleSDK())
		{
			Physics->initVehicleSDK(getGameObject()->getParentScene());
		}

		VehicleDesc vehicleDesc = initVehicleDesc();

		gVehicle4W = createVehicle4W(vehicleDesc, Physics->getPhysics(), Physics->getCooking());
		//20 80 20
		PxTransform startTransform(PxVec3(20, 80 + (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 20), PxQuat(PxIdentity));
		gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
		gVehicle4W->getRigidDynamicActor()->setName("CarroTeste");

		GEngine->getCurrentScene()->getPhysxScene()->addActor(*gVehicle4W->getRigidDynamicActor());

		//configurando veículo para iniciar em repouso na primeira marcha
		//e com marcha automática
		gVehicle4W->setToRestState();
		gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
		gVehicle4W->mDriveDynData.setUseAutoGears(true);

		gVehicleModeTimer = 0.0f;
		gVehicleOrderProgress = 0;
		startBrakeMode();

		initialized = true;

		/*if (mVehicleDescMap.size())
		{
			applyDescription();
		}*/
	}
}

void Vehicle::nextInit()
{
	
}

void Vehicle::setEnabled(bool e)
{

}

bool Vehicle::isEnabled()
{
	return true;
}

void Vehicle::applyDesc(int key, const std::string& value)
{
	/*switch (key)
	{

	case CCD_ChassisMass:
		setCenter(Convert::parseVector3(value));
		break;

	case CCD_Height:
		setHeight(Convert::parseReal(value));
		break;

	case CCD_Radius:
		setRadius(Convert::parseReal(value));
		break;

	case CCD_SlopeLimit:
		setSlopeLimit(Convert::parseReal(value));
		break;

	case CCD_StepOffset:
		setStepOffset(Convert::parseReal(value));
		break;

	default:
		break;
	}*/
}

void Vehicle::setChassisConvexMesh(const std::string& chassisMeshName)
{
	if ( (Ogre::ResourceGroupManager::getSingleton().resourceExists( "General", chassisMeshName ) ) ||
		 (Ogre::MeshManager::getSingletonPtr()->resourceExists(chassisMeshName)) )
	{
		Ogre::MeshPtr ptr = Ogre::MeshManager::getSingletonPtr()->load(chassisMeshName, "General");

		if (!ptr.isNull())
		{
			Cooker::Params params;
			pxChassisConvexMesh = Cooker::getSingleton().createPxConvexMesh(ptr, params);
		}
	}
}

void Vehicle::setWheelsConvexMesh(const StringVector& list)
{
	for(int i = 0; i < list.size(); ++i)
	{
		std::string chassisMeshName = list.at(i);

		if ( (Ogre::ResourceGroupManager::getSingleton().resourceExists( "General", chassisMeshName ) ) ||
		 (Ogre::MeshManager::getSingletonPtr()->resourceExists(chassisMeshName)) )
		{
			Ogre::MeshPtr ptr = Ogre::MeshManager::getSingletonPtr()->load(chassisMeshName, "General");

			if (!ptr.isNull())
			{
				Cooker::Params params;
				pxWheelsConvexMeshList.push_back(Cooker::getSingleton().createPxConvexMesh(ptr, params));
			}
		}
	}
}

VehicleDesc Vehicle::initVehicleDesc()
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const PxF32 chassisMass = 1500.0f;
	const PxVec3 chassisDims(2.5f,2.0f,5.0f);
	const PxVec3 chassisMOI
		((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass/12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass/12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass/12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 20.0f;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.4f;
	const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 4;

	VehicleDesc vehicleDesc;
	vehicleDesc.chassisMass = chassisMass;
	vehicleDesc.chassisDims = chassisDims;
	vehicleDesc.chassisMOI = chassisMOI;
	vehicleDesc.chassisCMOffset = chassisCMOffset;
	vehicleDesc.chassisMaterial = Physics->getDefaultMaterial();
	vehicleDesc.wheelMass = wheelMass;
	vehicleDesc.wheelRadius = wheelRadius;
	vehicleDesc.wheelWidth = wheelWidth;
	vehicleDesc.wheelMOI = wheelMOI;
	vehicleDesc.numWheels = nbWheels;
	vehicleDesc.wheelMaterial = Physics->getDefaultMaterial();

	return vehicleDesc;
}

void Vehicle::startBrakeMode()
{
	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalBrake(true);
	}
	else
	{
		gVehicleInputData.setAnalogBrake(1.0f);
	}
}

void Vehicle::startAccelerateForwardsMode()
{
	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
	}
}

void Vehicle::startAccelerateReverseMode()
{
	gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);

	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
	}
}

void Vehicle::startTurnHardLeftMode()
{
	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
		gVehicleInputData.setDigitalSteerLeft(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(true);
		gVehicleInputData.setAnalogSteer(-1.0f);
	}
}

void Vehicle::startTurnHardRightMode()
{
	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(true);
		gVehicleInputData.setDigitalSteerRight(true);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(1.0f);
		gVehicleInputData.setAnalogSteer(1.0f);
	}
}

void Vehicle::startHandbrakeTurnLeftMode()
{
	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalSteerLeft(true);
		gVehicleInputData.setDigitalHandbrake(true);
	}
	else
	{
		gVehicleInputData.setAnalogSteer(-1.0f);
		gVehicleInputData.setAnalogHandbrake(1.0f);
	}
}

void Vehicle::startHandbrakeTurnRightMode()
{
	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalSteerRight(true);
		gVehicleInputData.setDigitalHandbrake(true);
	}
	else
	{
		gVehicleInputData.setAnalogSteer(1.0f);
		gVehicleInputData.setAnalogHandbrake(1.0f);
	}
}

void Vehicle::fixedUpdate()
{
	if (initialized)
	{
		incrementDrivingMode();

		//Update the control inputs for the vehicle.
		if(gMimicKeyInputs)
		{
			physx::PxVehicleDrive4WSmoothDigitalRawInputsAndSetAnalogInputs(gKeySmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, Time->getFixedDeltaTime(), gIsVehicleInAir, *gVehicle4W);
		}
		else
		{
			physx::PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, Time->getFixedDeltaTime(), gIsVehicleInAir, *gVehicle4W);
		}

		//Work out if the vehicle is in the air.
		gIsVehicleInAir = Physics->updateVehicles(gVehicle4W);
	}
	
}

void Vehicle::incrementDrivingMode()
{
	const Real timestep = Time->getFixedDeltaTime();

	gVehicleModeTimer += timestep;
	if(gVehicleModeTimer > gVehicleModeLifetime)
	{
		//If the mode just completed was eDRIVE_MODE_ACCEL_REVERSE then switch back to forward gears.
		if(eDRIVE_MODE_ACCEL_REVERSE == gDriveModeOrder[gVehicleOrderProgress])
		{
			gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eFIRST);
		}

		//Increment to next driving mode.
		gVehicleModeTimer = 0.0f;
		gVehicleOrderProgress++;
		releaseAllControls();

		//If we are at the end of the list of driving modes then start again.
		if(eDRIVE_MODE_NONE == gDriveModeOrder[gVehicleOrderProgress])
		{
			gVehicleOrderProgress = 0;
			gVehicleOrderComplete = true;
		}

		//Start driving in the selected mode.
		DriveMode eDriveMode = gDriveModeOrder[gVehicleOrderProgress];
		switch(eDriveMode)
		{
		case eDRIVE_MODE_ACCEL_FORWARDS:
			startAccelerateForwardsMode();
			break;
		case eDRIVE_MODE_ACCEL_REVERSE:
			startAccelerateReverseMode();
			break;
		case eDRIVE_MODE_HARD_TURN_LEFT:
			startTurnHardLeftMode();
			break;
		case eDRIVE_MODE_HANDBRAKE_TURN_LEFT:
			startHandbrakeTurnLeftMode();
			break;
		case eDRIVE_MODE_HARD_TURN_RIGHT:
			startTurnHardRightMode();
			break;
		case eDRIVE_MODE_HANDBRAKE_TURN_RIGHT:
			startHandbrakeTurnRightMode();
			break;
		case eDRIVE_MODE_BRAKE:
			startBrakeMode();
			break;
		case eDRIVE_MODE_NONE:
			break;
		};

		//If the mode about to start is eDRIVE_MODE_ACCEL_REVERSE then switch to reverse gears.
		if(eDRIVE_MODE_ACCEL_REVERSE == gDriveModeOrder[gVehicleOrderProgress])
		{
			gVehicle4W->mDriveDynData.forceGearChange(PxVehicleGearsData::eREVERSE);
		}
	}
}

void Vehicle::releaseAllControls()
{
	if(gMimicKeyInputs)
	{
		gVehicleInputData.setDigitalAccel(false);
		gVehicleInputData.setDigitalSteerLeft(false);
		gVehicleInputData.setDigitalSteerRight(false);
		gVehicleInputData.setDigitalBrake(false);
		gVehicleInputData.setDigitalHandbrake(false);
	}
	else
	{
		gVehicleInputData.setAnalogAccel(0.0f);
		gVehicleInputData.setAnalogSteer(0.0f);
		gVehicleInputData.setAnalogBrake(0.0f);
		gVehicleInputData.setAnalogHandbrake(0.0f);
	}
}