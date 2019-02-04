#include "physics/BoxCollider.h"

#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxRigidActor.h"
#include "PxShape.h"

#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "transform/Transform.h"
#include "utils/KernelConverter.h"
#include "physics/Rigidbody.h"


BoxCollider::BoxCollider()
	: Collider("BoxCollider")
{

}

BoxCollider::BoxCollider(const ComponentDescMap &d)
	:  Collider("BoxCollider"),
	   mBoxColliderDescMap(d)
{

}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::init(const std::string& name)
{
	if (!initialized)
	{
		if (!rigidbody())
		{
			pxStaticActor->setName(name.c_str());
			pxShape = getGameObject()->getParentScene()->createBoxShape( pxStaticActor,
																	physx::PxVec3(1,1,1),
																	2.0f,
																    physx::PxTransform(physx::PxVec3(0,0,0)));
		}
		else
		{
			pxShape = getGameObject()->getParentScene()->createBoxShape( rigidbody()->getActor(),
																	physx::PxVec3(1,1,1),
																	2.0f,
																	physx::PxTransform(physx::PxVec3(0,0,0)));
		}

		initialized = true;

		if (mBoxColliderDescMap.size())
		{
			applyDescription(mBoxColliderDescMap);
		}
	}
}

void BoxCollider::setCenter(const Vector3& posCenter)
{
	if (initialized)
	{
		pxShape->setLocalPose(physx::PxTransform(Convert::toPx(posCenter)));
	}
	else
	{
		mBoxColliderDescMap[BCD_Center] = Convert::toString(posCenter);
	}
}

void BoxCollider::setSize(const Vector3& dim)
{
	if (initialized)
	{
		physx::PxBoxGeometry g;
		if (pxShape->getBoxGeometry(g))
		{
			Vector3 aux = dim *0.5f;
			g.halfExtents = Convert::toPx(aux);

			pxShape->setGeometry(g);
		}
	}
	else
	{
		mBoxColliderDescMap[BCD_SizeDim] = Convert::toString(dim);
	}
}

Vector3 BoxCollider::getSize()
{
	physx::PxBoxGeometry g;
	if (pxShape->getBoxGeometry(g))
	{
		Vector3 aux = Convert::toKernel(g.halfExtents * 2.0f);

		return aux;
	}

	return Vector3::ZERO;
}

void BoxCollider::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case BCD_SizeDim:
		setSize(Convert::parseVector3(value));
		break;

	case BCD_Center:
		setCenter(Convert::parseVector3(value));
		break;

	default:
		break;
	}
}

void BoxCollider::setScale(const Vector3& s)
{
	if (initialized)
	{
		physx::PxTransform localPose = pxShape->getLocalPose();
		Vector3 newDim = getSize() * 0.5f;

		//newDim *= s;

		//or

		physx::PxMat33 scaleMat = physx::PxMat33::createDiagonal(Convert::toPx(s)) * physx::PxMat33(localPose.q); // == (pose^-1 * scaling)^T
		newDim.x *= scaleMat.column0.magnitude();
		newDim.y *= scaleMat.column1.magnitude();
		newDim.z *= scaleMat.column2.magnitude();

		setSize(newDim);

		localPose.p = localPose.p.multiply(Convert::toPx(s));
		pxShape->setLocalPose(localPose);
	}
}
