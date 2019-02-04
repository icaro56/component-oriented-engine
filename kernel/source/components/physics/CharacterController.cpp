#include "physics/CharacterController.h"

#include "gameobject/GameObject.h"
#include "transform/Transform.h"
#include "engine/Scene.h"
#include "engine/Engine.h"
#include "engine/PhysicsEngine.h"
#include "time/Time.h"
#include "characterkinematic/PxCapsuleController.h"
#include "utils/KernelConverter.h"
#include "PxRigidDynamic.h"
#include <cmath>
#include "foundation/PxMath.h"
#include "OgreMath.h"
#include "gameobject/LayerMask.h"

#include "moc_CharacterController.cpp"

////////////////////////////////////// CharacterControllerHit  //////////////////////////////////////////
ControllerColliderHit::ControllerColliderHit()
	:   gameObject(nullptr),
		collider(nullptr),
		controller(nullptr),
		moveDirection(Vector3::ZERO),
		moveLength(0),
		normal(Vector3::ZERO),
		point(Vector3::ZERO),
		rigidbody(nullptr),
		transform(nullptr)
{

}

ControllerColliderHit::~ControllerColliderHit()
{

}

void ControllerColliderHit::setGameObject(GameObject* g)
{
	gameObject = g;

	if (gameObject)
	{
		transform = gameObject->transform;
		rigidbody = gameObject->rigidbody;
		collider = gameObject->collider;
	}
}

void ControllerColliderHit::setController(CharacterController* c)
{
	controller = c;
}

Collider* ControllerColliderHit::getCollider()
{
	return collider;
}

CharacterController* ControllerColliderHit::getController()
{
	return controller;
}

GameObject* ControllerColliderHit::getGameObject()
{
	return gameObject;
}

Vector3 ControllerColliderHit::getMoveDirection()
{
	return moveDirection;
}

float ControllerColliderHit::getMoveLength()
{
	return moveLength;
}

Vector3 ControllerColliderHit::getNormal()
{
	return normal;
}

Vector3 ControllerColliderHit::getPoint()
{
	return point;
}

Rigidbody* ControllerColliderHit::getRigidbody()
{
	return rigidbody;
}

Transform* ControllerColliderHit::getTransform()
{
	return transform;
}

void ControllerColliderHit::setMoveDirection(const Vector3& d)
{
	moveDirection = d;
}

void ControllerColliderHit::setMoveLength(float l)
{
	moveLength = l;
}

void ControllerColliderHit::setNormal(const Vector3& d)
{
	normal = d;
}

void ControllerColliderHit::setPoint(const Vector3& d)
{
	point = d;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////// ControllerControllerHit  //////////////////////////////////////////
ControllerControllerHit::ControllerControllerHit()
	:   otherController(nullptr),
	controller(nullptr),
	moveDirection(Vector3::ZERO),
	moveLength(0),
	normal(Vector3::ZERO),
	point(Vector3::ZERO)
{

}

ControllerControllerHit::~ControllerControllerHit()
{

}

void ControllerControllerHit::setController(CharacterController* c)
{
	controller = c;
}

void ControllerControllerHit::setOtherController(CharacterController* c)
{
	otherController = c;
}

CharacterController* ControllerControllerHit::getController()
{
	return controller;
}

CharacterController* ControllerControllerHit::getOtherController()
{
	return otherController;
}

Vector3 ControllerControllerHit::getMoveDirection()
{
	return moveDirection;
}

float ControllerControllerHit::getMoveLength()
{
	return moveLength;
}

Vector3 ControllerControllerHit::getNormal()
{
	return normal;
}

Vector3 ControllerControllerHit::getPoint()
{
	return point;
}

void ControllerControllerHit::setMoveDirection(const Vector3& d)
{
	moveDirection = d;
}

void ControllerControllerHit::setMoveLength(float l)
{
	moveLength = l;
}

void ControllerControllerHit::setNormal(const Vector3& d)
{
	normal = d;
}

void ControllerControllerHit::setPoint(const Vector3& d)
{
	point = d;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

CharacterController::CharacterController()
	: Collider("CharacterController", "CharacterController"),
	controller(nullptr),
	mIsGrounded(false),
	mVelocity(Vector3::ZERO),
	minMoveDistance(0),
	mFilterData(nullptr)
{

}

CharacterController::CharacterController(std::string idComponent)
	: Collider(idComponent, "CharacterController"),
	controller(nullptr),
	mIsGrounded(false),
	mVelocity(Vector3::ZERO),
	minMoveDistance(0),
	mFilterData(nullptr)
{

}

CharacterController::CharacterController(const ComponentDescMap &d)
	: Collider("CharacterController", "CharacterController"),
	  mCharacterControllerDescMap(d),
	  mIsGrounded(false),
	  mVelocity(Vector3::ZERO),
	  minMoveDistance(0),
	  mFilterData(nullptr)
{

}

CharacterController::~CharacterController()
{

}

void CharacterController::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void CharacterController::init(const std::string& name)
{
	if (!initialized)
	{
		physx::PxCapsuleControllerDesc capsuleDesc;
		capsuleDesc.material = Physics->getDefaultMaterial();
		capsuleDesc.position = Convert::toPx(transform()->getPosition(), true);
		capsuleDesc.height = 3.0f;
		capsuleDesc.radius = 0.5f;
		//capsuleDesc.slopeLimit		= 0.0f;
		capsuleDesc.contactOffset		= 0.1f;
		capsuleDesc.stepOffset		= 0.02f;
		capsuleDesc.reportCallback	= &getGameObject()->getParentScene()->getCharacterControllerCallback();
		capsuleDesc.behaviorCallback	= &getGameObject()->getParentScene()->getCharacterControllerCallback();
		capsuleDesc.userData = this;

		mFilterCallback = &getGameObject()->getParentScene()->getCharacterControllerCallback();
		mCCTFilterCallback = &getGameObject()->getParentScene()->getCharacterControllerCallback();

		controller = getGameObject()->getParentScene()->createCharacterController(capsuleDesc);
		controller->getActor()->setName(name.c_str());

		getControllerShape();

		initialized = true;

		if (mCharacterControllerDescMap.size())
		{
			applyDescription(mCharacterControllerDescMap);
		}
	}
}

void CharacterController::nextInit()
{
	Collider::nextInit();

	if (!mFilterData)
	{
		mFilterData = new physx::PxFilterData(pxShape->getSimulationFilterData());
	}
	else
	{
		mFilterData->word0 = pxShape->getSimulationFilterData().word0;
		mFilterData->word1 = pxShape->getSimulationFilterData().word1;
		mFilterData->word2 = pxShape->getSimulationFilterData().word2;
		mFilterData->word3 = pxShape->getSimulationFilterData().word3;
	}

	setSceneQuery(false);
}

void CharacterController::setEnabled(bool e)
{
	//Collider::setEnabled(e);

	if (initialized)
	{
		if (wasSceneQuery)
			setSceneQuery(e);
	}
	else
	{
		mColliderDescMap[CD_Enabled] = Convert::toString(e);
	}
}

bool CharacterController::isEnabled()
{
	return Collider::isEnabled();
}

void CharacterController::getControllerShape()
{
	physx::PxRigidDynamic* actor = controller->getActor();
	if(actor)
	{
		if(actor->getNbShapes())
		{
			physx::PxShape* ctrlShape;
			actor->getShapes(&ctrlShape,1);
			pxShape = ctrlShape;
		}
		else
			std::cerr << "character actor has no shape";
	}
	else
		std::cerr << "character could not create actor";
}

void CharacterController::setCenter(const Vector3& c)
{
	if (initialized)
	{
		controller->setPosition(Convert::toPx(c, true));
	}
	else
	{
		mCharacterControllerDescMap[CCD_Center] = Convert::toString(c);
	}
}

Vector3 CharacterController::getCenter()
{
	if (initialized)
	{
		return Convert::toKernel(controller->getPosition());
	}

	return Vector3::ZERO;
}

void CharacterController::applyDesc(int key, const std::string& value)
{
	switch (key)
	{

	case CCD_Center:
		setCenter(Convert::parseVector3(value));
		break;

	case CCD_Height:
		setHeight(Convert::parseReal(value));
		break;

	case CCD_Radius:
		setRadius(Convert::parseReal(value));
		break;

	case CCD_SlopeLimit:
		setSlopeLimit(Convert::parseReal(value));
		break;

	case CCD_StepOffset:
		setStepOffset(Convert::parseReal(value));
		break;

	default:
		break;
	}
}

CollisionFlags CharacterController::move(const Vector3& motion, bool useCenterPosition)
{
	if (initialized)
	{
		float dt = Time->getDeltaTime();
		physx::PxExtendedVec3 oldPosition = controller->getFootPosition();

		const physx::PxControllerFilters filters(mFilterData, mFilterCallback, mCCTFilterCallback);

		unsigned auxflags = controller->move(Convert::toPx(motion), minMoveDistance, dt, filters);

		mVelocity = Convert::toKernel(oldPosition - controller->getFootPosition());

		collisionFlags = CollisionFlags(auxflags);

		if (collisionFlags & CollisionFlags::Below)
		{
			mIsGrounded = true;
		}
		else
		{
			mIsGrounded = false;
		}

		if (useCenterPosition)
			transform()->_updatePosition(Convert::toKernel(controller->getPosition()));
		else
			transform()->_updatePosition(Convert::toKernel(controller->getFootPosition()));
		//transform()->setRotation(Convert::toOgre(pose.q));

		return collisionFlags;
	}

	return CollisionFlags::None;
}

void CharacterController::setHeight(float h)
{
	if (initialized)
	{
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(controller);
		
		capsuleController->setHeight(h);
	}
	else
	{
		mCharacterControllerDescMap[CCD_Height] = Convert::toString(h);
	}
}

float CharacterController::getHeight()
{
	if (initialized)
	{
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(controller);

		return capsuleController->getHeight();
	}

	return 3.0f;
}

void CharacterController::setRadius(float r)
{
	if (initialized)
	{
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(controller);

		capsuleController->setRadius(r);
	}
	else
	{
		mCharacterControllerDescMap[CCD_Radius] = Convert::toString(r);
	}
}

float CharacterController::getRadius()
{
	if (initialized)
	{
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(controller);

		return capsuleController->getRadius();
	}

	return 0.5f;
}

bool CharacterController::isGrounded()
{
	return mIsGrounded;
}

void CharacterController::setSlopeLimit(float grau)
{
	if (initialized)
	{
		controller->setSlopeLimit(cosf(Ogre::Math::DegreesToRadians(grau)));
	}
	else
	{
		mCharacterControllerDescMap[CCD_SlopeLimit] = Convert::toString(grau);
	}
}

float CharacterController::getSlopeLimit()
{
	if (initialized)
	{
		return acosf(controller->getSlopeLimit());
	}

	return 0.0f;
}

void CharacterController::setStepOffset(float offset)
{
	if (initialized)
	{
		controller->setStepOffset(offset);
	}
	else
	{
		mCharacterControllerDescMap[CCD_StepOffset] = Convert::toString(offset);
	}
}

float CharacterController::getStepOffset()
{
	if (initialized)
	{
		return controller->getStepOffset();
	}

	return 0.0f;
}

Vector3 CharacterController::getVelocity()
{
	return mVelocity;
}

void CharacterController::informOnControllerColliderHit(ControllerColliderHit& hit)
{
	emit onControllerColliderHit(hit);
}

void CharacterController::informOnControllerControllerHit(ControllerControllerHit& hit)
{
	emit onControllerControllerHit(hit);
}

void CharacterController::setReportCollision(bool filterMask)
{
	Collider::setReportCollision(filterMask);

	mFilterData->word2 = pxShape->getSimulationFilterData().word2;
}

void CharacterController::setLayerId(unsigned layer)
{
	Collider::setLayerId(layer);

	mFilterData->word0 = pxShape->getSimulationFilterData().word0;
	mFilterData->word1 = pxShape->getSimulationFilterData().word1;
}

const physx::PxFilterData* CharacterController::getFilterData()
{
	return mFilterData;
}

void CharacterController::setScale(const Vector3& s)
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

void CharacterController::setRotation(const Quaternion& q)
{
	if (initialized)
	{
		physx::PxTransform p = controller->getActor()->getGlobalPose();
		p.q = Convert::toPx(q);
		controller->getActor()->setGlobalPose(p);
	}
	
}

Quaternion CharacterController::getRotation()
{
	if (initialized)
	{
		return Convert::toKernel(controller->getActor()->getGlobalPose().q);
	}

	return Quaternion::IDENTITY;
}

void CharacterController::setPosition(const Vector3& p)
{
	if (initialized)
	{
		controller->setPosition(Convert::toPx(p, true));
	}
}

Vector3 CharacterController::getPosition()
{
	if (initialized)
	{
		return Convert::toKernel(controller->getPosition());
	}

	return Vector3::ZERO;
}

void CharacterController::setFootPosition(const Vector3& p)
{
	if (initialized)
	{
		controller->setFootPosition(Convert::toPx(p, true));
	}
}

Vector3 CharacterController::getFootPosition()
{
	if (initialized)
	{
		return Convert::toKernel(controller->getFootPosition());
	}

	return Vector3::ZERO;
}