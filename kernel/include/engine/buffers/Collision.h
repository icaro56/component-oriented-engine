#ifndef COLLISION_H
#define COLLISION_H

#include "KernelPrerequisites.h"
#include "math/Vector3.h"

class Collider;
class GameObject;
class Rigidbody;
class Transform;

class KERNEL_VS_EXPORT ContactPoint
{
public:
	ContactPoint()
	{
		otherCollider = nullptr;
		thisCollider = nullptr;
	}

	Ogre::Vector3 normal;
	Ogre::Vector3 point;
	Ogre::Vector3 impulse;
	Collider* otherCollider;
	Collider* thisCollider;
};

class KERNEL_VS_EXPORT Collision
{
	friend class SimulationEventCallback;
public:
	typedef std::vector<ContactPoint> ContactPoints;

	Collision();
	virtual ~Collision();

	//The Collider we hit
	Collider* getCollider();

	//The GameObject whose collider we are colliding with.
	GameObject* getGameObject();

	//The relative linear velocity of the two colliding objects.
	Vector3 getRelativeVelocity();

	//The Rigidbody we hit (Read Only). This is null if the object we hit is a collider with no rigidbody attached.
	Rigidbody* getRigidBody();

	Transform* getTransform();

	ContactPoints contactPoints;

private:
	void setGameObject(GameObject* g);
	void setRelativeVelocity(const Vector3& vel);

private:
	Collider* collider;
	GameObject* gameObject;
	Vector3 relativeVelocity;
	Rigidbody* rigidbody;
	Transform* transform;
};

#endif