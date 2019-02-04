#include "math/Ray.h"

Ray::Ray()
	:origin(Vector3::ZERO), 
	 direction(Vector3::UNIT_Z)
{

}

Ray::Ray(const Vector3& or, const Vector3& dir)
	:origin(or),
	 direction(dir)
{

}

Ray::~Ray()
{

}

void Ray::setOrigin(const Vector3& or)
{
	origin = or;
}

const Vector3& Ray::getOrigin() const
{
	return origin;
}

void Ray::setDirection(const Vector3& dir)
{
	direction = dir;
}

const Vector3& Ray::getDirection() const
{
	return direction;
}

Vector3 Ray::getPoint(Real t) const
{
	return Vector3(origin + (direction * t));
}