#include "physics/MeshCollider.h"

#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxRigidActor.h"
#include "PxShape.h"

#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "engine/Engine.h"
#include "engine/PhysicsEngine.h"
#include "transform/Transform.h"
#include "mesh/MeshFilter.h"
#include "mesh/Renderer.h"
#include "utils/KernelConverter.h"
#include "physics/Rigidbody.h"

MeshCollider::MeshCollider()
	: Collider("MeshCollider"),
	  convex(false)
{

}

MeshCollider::MeshCollider(const ComponentDescMap &d)
	:  Collider("MeshCollider"),
	mMeshColliderDescMap(d),
	convex(false)
{

}

MeshCollider::~MeshCollider()
{

}

void MeshCollider::init(const std::string& name)
{
	if (!initialized)
	{
		if (!rigidbody())
		{
			
			MeshFilter* meshfilter = getGameObject()->getComponent<MeshFilter>();
			if (renderer() && meshfilter)
			{
				if (isConvex())
				{
					pxShape = getGameObject()->getParentScene()->createConvexMeshShape(pxStaticActor, 
						renderer()->getOgreEntity(),
						2.0f, *Physics->getDefaultMaterial(),
						Vector3(1,1,1), physx::PxTransform::createIdentity());
				}
				else
				{
					pxShape = getGameObject()->getParentScene()->createTriangleMeshShape(pxStaticActor,
						renderer()->getOgreEntity(),
						Cooker::Params());
				}
			}
		}
		else
		{
			MeshFilter* meshfilter = getGameObject()->getComponent<MeshFilter>();
			if (renderer() && meshfilter)
			{
				if (isConvex())
				{
					pxShape = getGameObject()->getParentScene()->createConvexMeshShape(rigidbody()->getActor(), 
						renderer()->getOgreEntity(),
						2.0f, *Physics->getDefaultMaterial(),
						Vector3(1,1,1), physx::PxTransform::createIdentity());
				}
				else
				{
					pxShape = getGameObject()->getParentScene()->createTriangleMeshShape(rigidbody()->getActor(),
						renderer()->getOgreEntity(),
						Cooker::Params());
				}
				
			}
		}

		initialized = true;

		if (mMeshColliderDescMap.size())
		{
			applyDescription(mMeshColliderDescMap);
		}
	}
}

void MeshCollider::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	/*case BCD_SizeDim:
		setSize(Convert::parseVector3(value));
		break;*/

	default:
		break;
	}
}

void MeshCollider::setConvex(bool c)
{
	convex = c;
}

bool MeshCollider::isConvex()
{
	return convex;
}

void MeshCollider::setScale(const Vector3& s)
{
	if (initialized)
	{
		physx::PxVec3 lengthScale = Convert::toPx(s);

		physx::PxGeometryType::Enum t = pxShape->getGeometryType();
		physx::PxTransform localPose = pxShape->getLocalPose();

		switch (t)
		{
		case physx::PxGeometryType::Enum::eCONVEXMESH:
			{
				physx::PxConvexMeshGeometry convexMesh;
				pxShape->getConvexMeshGeometry(convexMesh);

				/*convexMesh.scale.scale.x *= lengthScale.x;
				convexMesh.scale.scale.y *= lengthScale.y;
				convexMesh.scale.scale.z *= lengthScale.z;*/

				physx::PxMat33 scaleMat = physx::PxMat33::createDiagonal(lengthScale) * physx::PxMat33(localPose.q); // == (pose^-1 * scaling)^T
				convexMesh.scale.scale.x *= scaleMat.column0.magnitude();
				convexMesh.scale.scale.y *= scaleMat.column1.magnitude();
				convexMesh.scale.scale.z *= scaleMat.column2.magnitude();

				pxShape->setGeometry(convexMesh);
			}
			break;

		case physx::PxGeometryType::Enum::eTRIANGLEMESH:
			{
				physx::PxTriangleMeshGeometry triMesh;
				pxShape->getTriangleMeshGeometry(triMesh);

				/*triMesh.scale.scale.x *= lengthScale.x;
				triMesh.scale.scale.y *= lengthScale.y;
				triMesh.scale.scale.z *= lengthScale.z;*/

				physx::PxMat33 scaleMat = physx::PxMat33::createDiagonal(lengthScale) * physx::PxMat33(localPose.q); // == (pose^-1 * scaling)^T
				triMesh.scale.scale.x *= scaleMat.column0.magnitude();
				triMesh.scale.scale.y *= scaleMat.column1.magnitude();
				triMesh.scale.scale.z *= scaleMat.column2.magnitude();

				pxShape->setGeometry(triMesh);
			}
			break;

		default:
			break;
		}

		localPose.p = localPose.p.multiply(Convert::toPx(s));
		pxShape->setLocalPose(localPose);
	}
}
