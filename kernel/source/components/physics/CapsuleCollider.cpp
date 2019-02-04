#include "physics/CapsuleCollider.h"


#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxRigidActor.h"
#include "PxShape.h"

#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "transform/Transform.h"
#include "utils/KernelConverter.h"
#include "physics/Rigidbody.h"
#include <cmath>

CapsuleCollider::CapsuleCollider()
	: Collider("CapsuleCollider")
{
	myDir = Y_Axis;
}

CapsuleCollider::CapsuleCollider(const ComponentDescMap &d)
	:  Collider("CapsuleCollider"),
	mCapsuleColliderDescMap(d)
{
	myDir = Y_Axis;
}

CapsuleCollider::~CapsuleCollider()
{

}

void CapsuleCollider::init(const std::string& name)
{
	if (!initialized)
	{
		if (!rigidbody())
		{
			physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));
			pxShape = getGameObject()->getParentScene()->createCapsuleShape(pxStaticActor, 0.5f, 1.0f, 2.0f,
														               relativePose);

		}
		else
		{
			physx::PxTransform relativePose(physx::PxVec3(0,0,0) , physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));
			pxShape = getGameObject()->getParentScene()->createCapsuleShape( rigidbody()->getActor(),
				0.5f, 1.0f, 2.0f,
				relativePose);
		}

		initialized = true;

		if (mCapsuleColliderDescMap.size())
		{
			applyDescription(mCapsuleColliderDescMap);
		}
	}
}

void CapsuleCollider::setCenter(const Vector3& posCenter)
{
	if (initialized)
	{
		pxShape->setLocalPose(physx::PxTransform(Convert::toPx(posCenter)));
	}
	else
	{
		mCapsuleColliderDescMap[CCD_Center] = Convert::toString(posCenter);
	}
}

void CapsuleCollider::setRadius(float r)
{
	if (initialized)
	{
		physx::PxCapsuleGeometry g;
		if (pxShape->getCapsuleGeometry(g))
		{
			g.radius = r;

			pxShape->setGeometry(g);
		}
	}
	else
	{
		mCapsuleColliderDescMap[CCD_Radius] = Convert::toString(r);
	}
}

float CapsuleCollider::getRadius()
{
	if (initialized)
	{
		physx::PxCapsuleGeometry g;
		if (pxShape->getCapsuleGeometry(g))
		{
			return g.radius;
		}
	}

	return 0.5f;
}

void CapsuleCollider::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case CCD_Radius:
		setRadius(Convert::parseReal(value));
		break;

	case CCD_Center:
		setCenter(Convert::parseVector3(value));
		break;

	case CCD_Height:
		setHeight(Convert::parseReal(value));
		break;

	default:
		break;
	}
}

void CapsuleCollider::setHeight(float h)
{
	if (initialized)
	{
		physx::PxCapsuleGeometry g;
		if (pxShape->getCapsuleGeometry(g))
		{
			h = h - (2 * getRadius());
			g.halfHeight = (h * 0.5f);

			pxShape->setGeometry(g);
		}
	}
	else
	{
		mCapsuleColliderDescMap[CCD_Height] = Convert::toString(h);
	}
}

float CapsuleCollider::getHeight()
{
	if (initialized)
	{
		physx::PxCapsuleGeometry g;
		if (pxShape->getCapsuleGeometry(g))
		{
			float height = g.halfHeight * 2;
			height = height + (2 * getRadius());

			return height;
		}
	}

	return 3.0f;
}

void CapsuleCollider::setDirection(CapsuleDir d)
{
	myDir = d;

	if (initialized)
	{
		physx::PxTransform relativePose;
		
		switch (d)
		{
		case X_Axis:
			relativePose = physx::PxTransform(physx::PxQuat::createIdentity());
			break;

		case Y_Axis:
			relativePose = physx::PxTransform(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));
			break;

		case Z_Axis:
			relativePose = physx::PxTransform(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,1,0)));
			break;

		default:
			break;
		}

		pxShape->setLocalPose(relativePose);
	}
}


void CapsuleCollider::setScale(const Vector3& s)
{
	if (initialized)
	{
		physx::PxTransform localPose = pxShape->getLocalPose();
		
		float r = getRadius() /** s.x*/;
		float h = getHeight() /** s.x*/;

		physx::PxMat33 scaleMat = physx::PxMat33::createDiagonal(Convert::toPx(s)) * physx::PxMat33(localPose.q);
		h *= scaleMat.column0.magnitude();
		r *= sqrt(scaleMat.column1.magnitude() * scaleMat.column2.magnitude());

		setRadius(r);
		setHeight(h);
		
		localPose.p = localPose.p.multiply(Convert::toPx(s));
		pxShape->setLocalPose(localPose);
	}
}
