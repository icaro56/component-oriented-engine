#ifndef RAY_H
#define RAY_H

#include "KernelPrerequisites.h"
#include "Vector3.h"

class KERNEL_VS_EXPORT Ray
{
public:
	Ray();
	Ray(const Vector3& or, const Vector3& dir);
	~Ray();

	void setOrigin(const Vector3& or);
	const Vector3& getOrigin() const;

	void setDirection(const Vector3& dir);
	const Vector3& getDirection() const;

	Vector3 getPoint(Real t) const;

	/** Function for writing to a stream.
    */
    inline KERNEL_VS_EXPORT friend std::ostream& operator <<
        ( std::ostream& o, const Ray& v )
    {
        o << "RayOrigin(" << v.origin.x << ", " << v.origin.y << ", " << v.origin.z << "), " << "RayDirection(" << v.direction.x << ", " << v.direction.y << ", " << v.direction.z << ")";
        return o;
    }

private:
	Vector3 origin;
	Vector3 direction;
};

#endif