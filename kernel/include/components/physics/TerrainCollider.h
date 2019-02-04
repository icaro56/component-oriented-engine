#ifndef TERRAIN_COLLIDER_H
#define TERRAIN_COLLIDER_H

#include "KernelPrerequisites.h"
#include "Collider.h"

#include "engine/buffers/TerrainData.h"

#include "math/Vector3.h"

namespace physx
{
	class PxHeightField;
}

class KERNEL_VS_EXPORT TerrainCollider : public Collider
{
public:
	TerrainCollider();
	virtual ~TerrainCollider();

	virtual void init(const std::string& name);

	TerrainData* getTerrainData();
	void setTerrainData(TerrainData* tdata);

	void setScale(const Vector3& s);

private:
	TerrainData* terrainData;
	Vector3 scale;

	physx::PxHeightField* heightField;
};


#endif // !TERRAIN_COLLIDER_H
