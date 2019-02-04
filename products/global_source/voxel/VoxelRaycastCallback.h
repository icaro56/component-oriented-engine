#ifndef VOXEL_RAYCAST_CALLBACK_H
#define VOXEL_RAYCAST_CALLBACK_H

#include "polyvoxcore/SimpleVolume.h"
#include "polyvoxcore/Vector.h"
#include "polyvoxcore/MaterialDensityPair.h"

class VoxelRaycastCallback
{
public:
   VoxelRaycastCallback();

   bool operator()(const PolyVox::SimpleVolume<uint8_t>::Sampler& sampler);

   bool foundIntersection();
   uint32_t getTotalVoxelsTouched();
   PolyVox::Vector3DInt32 getLastPosition();

private:

   bool bFoundIntersection;
   uint32_t m_uTotalVoxelsTouched;
   PolyVox::Vector3DInt32 v3dLastPosition;
};

class VoxelRaycastCallbackDensity
{
public:
   VoxelRaycastCallbackDensity();

   bool operator()(const PolyVox::SimpleVolume< PolyVox::MaterialDensityPair88 >::Sampler& sampler);

   bool foundIntersection();
   uint32_t getTotalVoxelsTouched();
   PolyVox::Vector3DInt32 getLastPosition();

private:

   bool bFoundIntersection;
   uint32_t m_uTotalVoxelsTouched;
   PolyVox::Vector3DInt32 v3dLastPosition;
};

#endif