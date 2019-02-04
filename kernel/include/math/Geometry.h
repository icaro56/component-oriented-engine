#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Vector3.h"
#include "KernelPrerequisites.h"

enum class GeometryType
{
	GT_Box,
	GT_Sphere,
	GT_Capsule,
	GT_None
};

class KERNEL_VS_EXPORT Geometry
{
public:
	Geometry();
	virtual ~Geometry(){};

	GeometryType getType();

protected:
	GeometryType type;
};

class KERNEL_VS_EXPORT BoxGeometry : public Geometry
{
public:
	BoxGeometry();
	virtual ~BoxGeometry();

	Vector3 halfExtents;
};

class KERNEL_VS_EXPORT SphereGeometry : public Geometry
{
public:
	SphereGeometry();
	virtual ~SphereGeometry();

	float radius;
};

class KERNEL_VS_EXPORT CapsuleGeometry : public Geometry
{
public:
	CapsuleGeometry();
	virtual ~CapsuleGeometry();

	float radius;
	float halfHeight;
};


#endif