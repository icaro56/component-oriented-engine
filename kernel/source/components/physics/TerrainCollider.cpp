#include "physics/TerrainCollider.h"

#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "engine/Engine.h"
#include "PxRigidStatic.h"
#include "PxRigidActor.h"
#include "PxShape.h"
#include "PxPhysics.h"

#include "geometry/PxHeightFieldSample.h"
#include "geometry/PxHeightFieldDesc.h"
#include "geometry/PxHeightField.h"

#include "engine/PhysicsEngine.h"

//*******************************************************************************************************

TerrainCollider::TerrainCollider()
	: Collider("TerrainCollider"),
	  terrainData(nullptr),
	  heightField(nullptr)
{

}

TerrainCollider::~TerrainCollider()
{
	if (heightField)
	{
		heightField->release();
	}
}

void TerrainCollider::init(const std::string& name)
{
	if (!initialized)
	{
		if (!rigidbody() && terrainData)
		{
			//valor máximo de um signed short
			const physx::PxReal maxValue = 32767.0f;
			//const physx::PxReal maxValue = 16385.0f;
			//const physx::PxReal maxValue = 65536.0f;
			
			//calculando escala
			double scaleX, scaleY, scaleZ;

			scaleX = (double)terrainData->getWidth() / (double)(terrainData->getPageSize() - 1);
			scaleY = (double)(terrainData->getHeight() );
			scaleZ = (double)terrainData->getWidth() / (double)(terrainData->getPageSize() - 1);
			scaleY = (double)(scaleY / (maxValue));
			
			scale = Vector3(  scaleX, scaleY, scaleZ);

			//Descritor
			physx::PxHeightFieldDesc heightFieldDesc;
			heightFieldDesc.samples.stride = sizeof(physx::PxHeightFieldSample);

			//preenchendo samples
			physx::PxHeightFieldSample* samples = new physx::PxHeightFieldSample[terrainData->getTotalSize()];

			char* currentByte = (char*)samples;

			for (physx::PxU32 row = 0; row < terrainData->getPageSize(); ++row)
			{
				for (physx::PxU32 column = 0; column < terrainData->getPageSize(); ++column)
				{
					physx::PxHeightFieldSample* currentSample = (physx::PxHeightFieldSample*)currentByte;
					
					currentSample->height = (physx::PxI16)(terrainData->getDataAt(row + (column * terrainData->getPageSize())) * maxValue);
					
					currentSample->materialIndex0 = 0;
					currentSample->materialIndex1 = 0;
					currentSample->setTessFlag();
					currentByte += heightFieldDesc.samples.stride;
				}
			}

			//segunda parte descritor
			heightFieldDesc.format = physx::PxHeightFieldFormat::eS16_TM;
			heightFieldDesc.nbColumns = terrainData->getPageSize();
			heightFieldDesc.nbRows = terrainData->getPageSize();
			heightFieldDesc.convexEdgeThreshold = 0;
			heightFieldDesc.thickness = -1000; //-1000
			heightFieldDesc.samples.data = samples;

			heightField = Physics->getPhysics()->createHeightField(heightFieldDesc);

			pxShape = getGameObject()->getParentScene()->createHeightFieldShape(pxStaticActor, 
																				heightField,
																				scale);

			delete [] heightFieldDesc.samples.data;
			heightFieldDesc.samples.data = 0;


			initialized = true;
		}

		
	}
}

TerrainData* TerrainCollider::getTerrainData()
{
	return terrainData;
}

void TerrainCollider::setTerrainData(TerrainData* tdata)
{
	terrainData = tdata;
}

void TerrainCollider::setScale(const Vector3& s)
{
	physx::PxHeightFieldGeometry hf;
	pxShape->getHeightFieldGeometry(hf);
	
	hf.columnScale *= s.x;
	hf.heightScale *= s.y;
	hf.rowScale *= s.z;

	pxShape->setGeometry(hf);
}