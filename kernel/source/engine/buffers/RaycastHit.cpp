#include "engine/buffers/RaycastHit.h"

#include "components/physics/Collider.h"
#include "components/physics/Rigidbody.h"
#include "components/transform/Transform.h"


RaycastHit::RaycastHit()
	:   barycentricCoordinate(Vector2::ZERO),
		collider(nullptr),
		distance(0.0f),
		normal(Vector3::ZERO),
		point(Vector3::ZERO),
		rigidbody(nullptr),
		transform(nullptr),
		triangleIndex(0)
{

}

RaycastHit::~RaycastHit()
{

}

Vector2 RaycastHit::getBarycentricCoordinate()
{
	return barycentricCoordinate;
}

Collider* RaycastHit::getCollider()
{
	return collider;
}

float RaycastHit::getDistance()
{
	return distance;
}

Vector3 RaycastHit::getNormal()
{
	return normal;
}

Vector3 RaycastHit::getPoint()
{
	return point;
}

Rigidbody* RaycastHit::getRigidbody()
{
	return rigidbody;
}

Transform* RaycastHit::getTransform()
{
	return transform;
}

int RaycastHit::getTriangleIndex()
{
	return triangleIndex;
}

void RaycastHit::setCollider(Collider* c)
{
	collider = c;
}

void RaycastHit::setRigidbody(Rigidbody* r)
{
	rigidbody = r;
}

void RaycastHit::setTransform(Transform* t)
{
	transform = t;
}

void RaycastHit::setBarycentricCoordinate(const Vector2& v)
{
	barycentricCoordinate = v;
}

void RaycastHit::setDistance(float d)
{
	distance = d;
}

void RaycastHit::setNormal(const Vector3& n)
{
	normal = n;
}

void RaycastHit::setPoint(const Vector3& p)
{
	point = p;
}

void RaycastHit::setTriangleIndex(int i)
{
	triangleIndex = i;
}