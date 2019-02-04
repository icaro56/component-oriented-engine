#include "math/Geometry.h"

Geometry::Geometry()
	: type(GeometryType::GT_None)
{

}

GeometryType Geometry::getType()
{
	return type;
}

BoxGeometry::BoxGeometry()
	: halfExtents(Vector3::ZERO)
{
	type = GeometryType::GT_Box;
}

BoxGeometry::~BoxGeometry()
{

}

SphereGeometry::SphereGeometry()
	: radius(0.0f)
{
	type = GeometryType::GT_Sphere;
}

SphereGeometry::~SphereGeometry()
{

}

CapsuleGeometry::CapsuleGeometry()
	: radius(0.0f),
	  halfHeight(0.0f)
{
	type = GeometryType::GT_Capsule;
}

CapsuleGeometry::~CapsuleGeometry()
{

}