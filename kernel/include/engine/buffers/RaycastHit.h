#ifndef RAYCAST_HIT_H
#define RAYCAST_HIT_H

#include "KernelPrerequisites.h"
#include "math/Vector2.h"
#include "math/Vector3.h"

class Collider;
class Rigidbody;
class Transform;
class RaycastHit;

typedef std::vector<RaycastHit> RaycastHitList;

/*!
	\brief Classe usada para retorno de dados dos raycasts.
	RaycastRendering simples usa so transform, point e distance
	VolumecastRendering usa apenas transform,
	Física usa geralmente todos os atributos, podendo não usar barycentric coordinate
*/
class KERNEL_VS_EXPORT RaycastHit
{
	friend class Scene;
	friend class Rigidbody;
public:
	RaycastHit();
	~RaycastHit();

	Vector2 getBarycentricCoordinate();
	Collider* getCollider();
	float getDistance();
	Vector3 getNormal();
	Vector3 getPoint();
	Rigidbody* getRigidbody();
	Transform* getTransform();
	int getTriangleIndex();

private:
	void setCollider(Collider* c);
	void setRigidbody(Rigidbody* r);
	void setTransform(Transform* t);
	void setBarycentricCoordinate(const Vector2& v);
	void setDistance(float d);
	void setNormal(const Vector3& n);
	void setPoint(const Vector3& p);
	void setTriangleIndex(int i);

private:
	Vector2 barycentricCoordinate;
	Collider* collider;
	float distance;
	Vector3 normal;
	Vector3 point;
	Rigidbody* rigidbody;
	Transform* transform;
	int triangleIndex;
};


#endif