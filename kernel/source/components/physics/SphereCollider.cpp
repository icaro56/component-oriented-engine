#include "physics/SphereCollider.h"


#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxRigidActor.h"
#include "PxShape.h"

#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "transform/Transform.h"
#include "utils/KernelConverter.h"
#include "physics/Rigidbody.h"


SphereCollider::SphereCollider()
	: Collider("SphereCollider")
{

}

SphereCollider::SphereCollider(const ComponentDescMap &d)
	:  Collider("SphereCollider"),
	mSphereColliderDescMap(d)
{

}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::init(const std::string& name)
{
	if (!initialized)
	{
		if (!rigidbody())
		{
			pxShape = getGameObject()->getParentScene()->createSphereShape(pxStaticActor,
				1.0f,
				2.0f,
				physx::PxTransform(physx::PxVec3(0,0,0)));

		}
		else
		{
			pxShape = getGameObject()->getParentScene()->createSphereShape( rigidbody()->getActor(),
				1.0f,
				2.0f,
				physx::PxTransform(physx::PxVec3(0,0,0)));
		}

		initialized = true;

		if (mSphereColliderDescMap.size())
		{
			applyDescription(mSphereColliderDescMap);
		}
	}
}

void SphereCollider::setCenter(const Vector3& posCenter)
{
	if (initialized)
	{
		pxShape->setLocalPose(physx::PxTransform(Convert::toPx(posCenter)));
	}
	else
	{
		mSphereColliderDescMap[SCD_Center] = Convert::toString(posCenter);
	}
}

void SphereCollider::setRadius(float r)
{
	if (initialized)
	{
		physx::PxSphereGeometry g;
		if (pxShape->getSphereGeometry(g))
		{
			g.radius = r;

			pxShape->setGeometry(g);
		}
	}
	else
	{
		mSphereColliderDescMap[SCD_Radius] = Convert::toString(r);
	}
}

float SphereCollider::getRadius()
{
	if (initialized)
	{
		physx::PxSphereGeometry g;
		if (pxShape->getSphereGeometry(g))
		{
			return g.radius;
		}
	}

	return 1.0f;
}

void SphereCollider::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case SCD_Radius:
		setRadius(Convert::parseReal(value));
		break;

	case SCD_Center:
		setCenter(Convert::parseVector3(value));
		break;

	default:
		break;
	}
}

#include <cmath>
void SphereCollider::setScale(const Vector3& s)
{
	if (initialized)
	{
		physx::PxTransform localPose = pxShape->getLocalPose();
		float r = getRadius();

		//código de escala tirado do exemplo da physx
		//r *= s.x;

		//código de escala tirado do link: https://xp-dev.com/sc/113239/187/%2Ftrunk%2FSource%20Code%2FDynamics%2FPhysics%2FPhysX%2FActors%2FpxShapeScaling.cpp
		r *= pow( abs(s.x * s.y * s.z), 1.0f/3.0f);

		setRadius(r);

		localPose.p = localPose.p.multiply(Convert::toPx(s));
		pxShape->setLocalPose(localPose);
	}
}
