#include "engine/buffers/Collision.h"

#include "gameobject/GameObject.h"

//Collision class
Collision::Collision()
	: collider(nullptr),
	gameObject(nullptr),
	rigidbody(nullptr),
	transform(nullptr)
{

}

Collision::~Collision()
{

}

Collider* Collision::getCollider()
{
	return collider;
}

GameObject* Collision::getGameObject()
{
	return gameObject;
}

Vector3 Collision::getRelativeVelocity()
{
	return relativeVelocity;
}

Rigidbody* Collision::getRigidBody()
{
	return rigidbody;
}

Transform* Collision::getTransform()
{
	return transform;
}

void Collision::setGameObject(GameObject* g)
{
	gameObject = g;

	if (gameObject)
	{
		transform = gameObject->transform;
		rigidbody = gameObject->rigidbody;
		collider = gameObject->collider;
	}
}

void Collision::setRelativeVelocity(const Vector3& vel)
{
	relativeVelocity = vel;
}