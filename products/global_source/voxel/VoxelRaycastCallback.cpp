#include "VoxelRaycastCallback.h"

VoxelRaycastCallback::VoxelRaycastCallback()
	: bFoundIntersection(false),
	  m_uTotalVoxelsTouched(0)
{
   v3dLastPosition = PolyVox::Vector3DInt32(0,0,0);
}

bool VoxelRaycastCallback::operator()(const PolyVox::SimpleVolume<uint8_t>::Sampler& sampler)
{
	m_uTotalVoxelsTouched++;
	
    // Store this so we can retrieve it later
    v3dLastPosition = sampler.getPosition();

    if(sampler.getVoxel() > 0) // You can replace this with your own condition if you want to.
    {
		bFoundIntersection = true;
		// We've hit something, so stop traversing.
		return false;
    }

    // By default we continue traversing.
    return true;
}

bool VoxelRaycastCallback::foundIntersection()
{
	return bFoundIntersection;
}

uint32_t VoxelRaycastCallback::getTotalVoxelsTouched()
{
	return m_uTotalVoxelsTouched;
}

PolyVox::Vector3DInt32 VoxelRaycastCallback::getLastPosition()
{
	return v3dLastPosition;
}


// Density

VoxelRaycastCallbackDensity::VoxelRaycastCallbackDensity()
	: bFoundIntersection(false),
	  m_uTotalVoxelsTouched(0)
{
   v3dLastPosition = PolyVox::Vector3DInt32(0,0,0);
}

bool VoxelRaycastCallbackDensity::operator()(const PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>::Sampler& sampler)
{
	m_uTotalVoxelsTouched++;
	
    // Store this so we can retrieve it later
    v3dLastPosition = sampler.getPosition();

	if(sampler.getVoxel().getMaterial() > 0) // You can replace this with your own condition if you want to.
    {
		bFoundIntersection = true;
		// We've hit something, so stop traversing.
		return false;
    }

    // By default we continue traversing.
    return true;
}

bool VoxelRaycastCallbackDensity::foundIntersection()
{
	return bFoundIntersection;
}

uint32_t VoxelRaycastCallbackDensity::getTotalVoxelsTouched()
{
	return m_uTotalVoxelsTouched;
}

PolyVox::Vector3DInt32 VoxelRaycastCallbackDensity::getLastPosition()
{
	return v3dLastPosition;
}