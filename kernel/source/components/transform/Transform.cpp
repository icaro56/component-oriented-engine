#include "transform/Transform.h"

#include "OgreSceneNode.h"
#include "OgreMovableObject.h"

#include "engine/Engine.h"
#include "engine/Scene.h"
#include "gameobject/GameObject.h"
#include "physics/Rigidbody.h"
#include "physics/Collider.h"
#include "physics/CharacterController.h"
#include "utils/KernelConverter.h"

#include "math/KernelMath.h"


Transform::Transform()
	: Component("Transform", "Transform"),
	  mParent(nullptr),
	  mSceneNode(nullptr),
	  changed(false),
	  isRootScene(false),
	  enabled(true)
{

}
	
Transform::~Transform()
{
	if (mSceneNode && !isRootScene)
	{
		mSceneNode->detachAllObjects();
		getGameObject()->getParentScene()->getOgreScene()->destroySceneNode(mSceneNode);
		mSceneNode = nullptr;
	}
}

void Transform::init(const std::string& name)
{
	if(!initialized)
	{
		if (!mSceneNode)
		{
			if (!isRootScene)
			{
				mSceneNode = getGameObject()->getParentScene()->getOgreScene()->getRootSceneNode()->createChildSceneNode(name + "_Transform");
				
				setParent(getGameObject()->getParentScene()->getRootTransform());
			}
			else
			{
				mSceneNode = getGameObject()->getParentScene()->getOgreScene()->getRootSceneNode();
			}
		}

		initialized = true;
	}
}

void Transform::setEnabled(bool enable)
{
	enabled = enable;
	if (initialized)
	{
		mSceneNode->setVisible(enable);
	}
}

bool Transform::isEnabled()
{
	return enabled;
}

void Transform::setPosition(const Vector3 &pos)
{
	if (initialized)
	{
		mSceneNode->_setDerivedPosition(Convert::toOgre(pos));

		if (rigidbody())
		{
			rigidbody()->setPosition(pos);
		}
		
		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setFootPosition(pos);
		}

		changed = true;
	}
}

void Transform::_updatePosition(const Vector3 &pos)
{
	mSceneNode->_setDerivedPosition(Convert::toOgre(pos));
	changed = true;
}

Vector3 Transform::getPosition()
{
	return Convert::toKernel(mSceneNode->_getDerivedPosition());
}

void Transform::setLocalPosition(const Vector3 &pos)
{
	if (initialized)
	{
		mSceneNode->setPosition(Convert::toOgre(pos));

		if (rigidbody())
		{
			rigidbody()->setPosition(getPosition());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setFootPosition(getPosition());
		}

		changed = true;
	}
}

Vector3 Transform::getLocalPosition()
{
	return Convert::toKernel(mSceneNode->getPosition());
}

void Transform::setLocalRotation(const Quaternion& quat)
{
	if (initialized)
	{
		mSceneNode->setOrientation(Convert::toOgre(quat));

		if (rigidbody())
		{
			rigidbody()->setRotation(getRotation());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(getRotation());
		}

		changed = true;
	}
}

Quaternion Transform::getLocalRotation()
{
	return Convert::toKernel(mSceneNode->getOrientation());
}

void Transform::setRotation(const Quaternion& quat)
{
	if (initialized)
	{
		mSceneNode->_setDerivedOrientation(Convert::toOgre(quat));

		if (rigidbody())
		{
			rigidbody()->setRotation(quat);
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(quat);
		}

		changed = true;
	}
}

void Transform::_updateRotation(const Quaternion& quat)
{
	mSceneNode->_setDerivedOrientation(Convert::toOgre(quat));

	changed = true;
}

Quaternion Transform::getRotation()
{
	return Convert::toKernel(mSceneNode->_getDerivedOrientation());
}

void Transform::setLocalScale(const Vector3& s, bool scaleChildren)
{
	if (initialized)
	{
		mSceneNode->setScale(Convert::toOgre(s));

		if (rigidbody())
		{
			rigidbody()->setScale(s);
		}

		if (collider())
		{
			collider()->setScale(s);
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setScale(s);
		}


		if (getChildCount())
		{
			//rescalar componentes dos filhos que não são escalados automaticamente
			
			TransformChildren::iterator it = children.begin();
			for (; it != children.end(); ++it)
			{
				Transform* aux = it->second;

				if (aux->rigidbody())
				{
					aux->rigidbody()->setScale(s);
				}

				if (aux->collider())
				{
					aux->collider()->setScale(s);
				}

				CharacterController* controller2 = aux->getGameObject()->getComponent<CharacterController>();
				if (controller2)
				{
					controller2->setScale(s);
				}
			}
		}

		changed = true;
	}
}

Vector3 Transform::getLocalScale()
{
	return Convert::toKernel(mSceneNode->getScale());
}

void Transform::_attachObject(Ogre::MovableObject*  movable)
{
	if (initialized)
	{
		mSceneNode->attachObject(movable);
	}
}

void Transform::turnX(double degrees, Space ts)
{
	if (initialized)
	{
		mSceneNode->pitch(Convert::toOgre(Degree(degrees)), Ogre::Node::TransformSpace(ts));

		if (rigidbody())
		{
			rigidbody()->setRotation(getRotation());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(getRotation());
		}

		changed = true;
	}
}

//----------------------------------------------------------------------------
void Transform::turnY(double degrees, Space ts)
{
	if (initialized)
	{
		mSceneNode->yaw(Convert::toOgre(Degree(degrees)), Ogre::Node::TransformSpace(ts));

		if (rigidbody())
		{
			rigidbody()->setRotation(getRotation());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(getRotation());
		}

		changed = true;
	}
}

//----------------------------------------------------------------------------
void Transform::turnZ(double degrees, Space ts)
{
	if (initialized)
	{
		mSceneNode->roll(Convert::toOgre(Degree(degrees)), Ogre::Node::TransformSpace(ts));

		if (rigidbody())
		{
			rigidbody()->setRotation(getRotation());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(getRotation());
		}

		changed = true;
	}
}

void Transform::lookAt(Transform* target, const Vector3& worldUp, Space ts)
{
	lookAt(target->getPosition(), worldUp, ts);
}

void Transform::lookAt(const Vector3& worldPosition, const Vector3& worldUp, Space ts)
{
	if (initialized)
	{
		mSceneNode->setFixedYawAxis(true, Convert::toOgre(worldUp));
		mSceneNode->lookAt(Convert::toOgre(worldPosition), Ogre::Node::TransformSpace(ts));

		if (rigidbody())
		{
			rigidbody()->setRotation(getRotation());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(getRotation());
		}

		changed = true;
	}
}

//----------------------------------------------------------------------------
void Transform::resetOrientation()
{
	if (initialized)
	{
		mSceneNode->resetOrientation();

		if (rigidbody())
		{
			rigidbody()->setRotation(getRotation());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(getRotation());
		}

		changed = true;
	}
}

//----------------------------------------------------------------------------
float Transform::getPitch()
{
    Quaternion q = Convert::toKernel(mSceneNode->getOrientation());
    return q.getPitch().valueDegrees();
}

//----------------------------------------------------------------------------
float Transform::getYaw()
{
    Quaternion q = Convert::toKernel(mSceneNode->getOrientation());
    return q.getYaw().valueDegrees();
}

//----------------------------------------------------------------------------
float Transform::getRoll()
{
    Quaternion q = Convert::toKernel(mSceneNode->getOrientation());
    return q.getRoll().valueDegrees();
}

//----------------------------------------------------------------------------
float Transform::getWorldPitch()
{
    Quaternion q = Convert::toKernel(mSceneNode->_getDerivedOrientation());
    return q.getPitch().valueDegrees();
}
//----------------------------------------------------------------------------
float Transform::getWorldYaw()
{
    Quaternion q = Convert::toKernel(mSceneNode->_getDerivedOrientation());
    return q.getYaw().valueDegrees();
}
//----------------------------------------------------------------------------
float Transform::getWorldRoll()
{
    Quaternion q = Convert::toKernel(mSceneNode->_getDerivedOrientation());
    return q.getRoll().valueDegrees();
}

//----------------------------------------------------------------------------
void Transform::translate (const Vector3 &d, Space relativeTo)
{
	if (initialized)
	{
		mSceneNode->translate(Convert::toOgre(d), Ogre::Node::TransformSpace(relativeTo));

		if (rigidbody())
		{
			rigidbody()->setPosition(getPosition());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setFootPosition(getPosition());
		}

		changed = true;
	}
}

//----------------------------------------------------------------------------
void Transform::translate(double tx, double ty, double tz, Space ts)
{
	if (initialized)
	{
		mSceneNode->translate(tx, ty, tz, Ogre::Node::TransformSpace(ts));

		if (rigidbody())
		{
			rigidbody()->setPosition(getPosition());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setFootPosition(getPosition());
		}

		changed = true;
	}
}

void  Transform::translate(const Vector3& translation, Transform* relativeTo)
{
	if (initialized)
	{
		if (relativeTo)
		{
			mSceneNode->translate(Convert::toOgre(relativeTo->getPosition() - translation), Ogre::Node::TransformSpace(Space::SELF));
		}
		else
		{
			mSceneNode->translate(Convert::toOgre(translation), Ogre::Node::TransformSpace(Space::WORLD));
		}
		
		if (rigidbody())
		{
			rigidbody()->setPosition(getPosition());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setFootPosition(getPosition());
		}

		changed = true;
	}
}

void Transform::translate(float x, float y, float z, Transform* relativeTo)
{
	translate(Vector3(x, y, z), relativeTo);
}

void Transform::setForward(const Vector3& dir)
{
	if (initialized)
	{
		mSceneNode->setDirection(Convert::toOgre(dir));

		if (rigidbody())
		{
			rigidbody()->setRotation(getRotation());
		}

		CharacterController* controller = getGameObject()->getComponent<CharacterController>();
		if (controller)
		{
			controller->setRotation(getRotation());
		}

		changed = true;
	}
}

Vector3 Transform::getForward()
{
	Quaternion q = getRotation();
	Vector3 v = q * Vector3::NEGATIVE_UNIT_Z;

	return v;
}

Vector3 Transform::getRight()
{
	Quaternion q = getRotation();
	Vector3 v = q * Vector3::UNIT_X;

	return v;
}

Vector3 Transform::getUp()
{
	Quaternion q = getRotation();
	Vector3 v = q * Vector3::UNIT_Y;

	return v;
}

void Transform::setInheritOrientation(bool op)
{
	if (initialized)
	{
		mSceneNode->setInheritOrientation(op);
	}
}

void Transform::setInheritScale(bool op)
{
	if (initialized)
	{
		mSceneNode->setInheritScale(op);
	}
}

void Transform::setFixedYawAxis(bool op)
{
	if (initialized)
	{
		mSceneNode->setFixedYawAxis(op);
	}
}

void Transform::attachChild(Transform *node)
{
	if (initialized)
	{
		Ogre::Node *parent = node->_getOgreSceneNode()->getParent();

		// detach from current parent first
		if (parent != NULL)
		{
			parent->removeChild(node->_getOgreSceneNode());
		}

		mSceneNode->addChild(node->_getOgreSceneNode());

		node->setParent(this);
	}
}

//----------------------------------------------------------------------------
void Transform::detach(bool attachToRoot)
{
	if (initialized)
	{
		Ogre::Node *parent = mSceneNode->getParent();

		if (parent != NULL)
		{
			parent->removeChild(mSceneNode);
		}

		if (attachToRoot)
			getGameObject()->getParentScene()->getOgreScene()->getRootSceneNode()->addChild(mSceneNode);

		if (mParent)
			mParent = NULL;
	}
}
//----------------------------------------------------------------------------
bool Transform::hasParent()
{
    Ogre::Node *parent = mSceneNode->getParent();
    return (parent == NULL) ? false : true;
}

//----------------------------------------------------------------------------
Transform* Transform::getParent()
{
    return mParent;
}

//----------------------------------------------------------------------------
void Transform::setParent(Transform* n)
{
	if (initialized)
	{
		mParent = n;
	}
}

//----------------------------------------------------------------------------
Ogre::SceneNode *Transform::_getOgreSceneNode()
{
    return mSceneNode;
}

void Transform::_setOgreSceneNode(Ogre::SceneNode* sceneNode)
{
	mSceneNode = sceneNode;
}

int Transform::getChildCount()
{
	if (initialized)
	{
		return children.size();
		//return mSceneNode->numChildren();
	}

	return 0;
}

Transform::TransformChildren& Transform::getChildren()
{
	return children;
}

void Transform::setEulerAngles(Vector3& angle)
{
	if (initialized)
	{
		Matrix3 mx;
		mx.FromEulerAnglesYXZ(Degree(angle.y), Degree(angle.x), Degree(angle.z));

		Quaternion quat(mx);
		setRotation(quat);

		changed = true;
	}
}

Vector3 Transform::getEulerAngles()
{
	if (initialized)
	{
		Matrix3 mx2;
		getRotation().ToRotationMatrix(mx2);

		Radian x, y, z;
		mx2.ToEulerAnglesYXZ(y, x, z);

		Vector3 vect(x.valueAngleUnits(), y.valueAngleUnits(), z.valueAngleUnits());

		return vect;
	}

	return Vector3::ZERO;
}

bool Transform::hasChanged()
{
	return changed;
}

void Transform::restoreHasChanged()
{
	changed = false;
}

void Transform::setLocalEulerAngles(Vector3& angle)
{
	if (initialized)
	{
		Matrix3 mx;
		mx.FromEulerAnglesYXZ(Degree(angle.y), Degree(angle.x), Degree(angle.z));

		Quaternion quat(mx);
		setLocalRotation(quat);

		changed = true;
	}
}

Vector3 Transform::getLocalEulerAngles()
{
	if (initialized)
	{
		Matrix3 mx2;
		getLocalRotation().ToRotationMatrix(mx2);

		Radian x, y, z;
		mx2.ToEulerAnglesYXZ(y, x, z);

		Vector3 vect(x.valueAngleUnits(), y.valueAngleUnits(), z.valueAngleUnits());

		return vect;
	}

	return Vector3::ZERO;
}

Vector3 Transform::getLossyScale()
{
	if (initialized)
	{
		return Convert::toKernel(mSceneNode->_getDerivedScale());
	}

	return Vector3(1,1,1);
}

void Transform::iAmRootScene(bool active)
{
	isRootScene = active;
}

Transform* Transform::getSubRoot()
{
	if (initialized)
	{
		if (getParent() == getRoot())
		{
			return this;
		}
		else
		{
			Transform* aux = this;
			while (aux->getParent())
			{
				if (aux->getParent() != getRoot())
					aux = aux->getParent();
				else
					return aux;
			}

			return aux;
		}
	}

	return this;
}

Transform* Transform::getRoot()
{
	return getGameObject()->getParentScene()->getRootTransform();
}

bool Transform::operator==(const Transform* t) const
{
	return (this->getGameObject()->getName() == t->getGameObject()->getName());
}

void Transform::detachChildren()
{
	if (initialized)
	{
		mSceneNode->detachAllObjects();
		getGameObject()->getParentScene()->getOgreScene()->destroySceneNode(mSceneNode);
		mSceneNode = 0;

		children.clear();
	}
}

Transform* Transform::find(const std::string& name)
{
	if (initialized)
	{
		if (children.count(name))
		{
			return children[name];
		}
	}

	return NULL;
}

Transform* Transform::getChild(int index)
{
	if (initialized)
	{
		if (index < children.size())
		{
			unsigned count;
			
			for (TransformChildren::iterator it = children.begin(); it != children.end(); ++it, ++count)
			{
				if (index == count-1)
				{
					return it->second;
				}
			}
		}
	}

	return NULL;
}

Vector3 Transform::inverseTransformDirection(const Vector3& direction)
{
	if (initialized)
	{
		Vector3 dir = direction;
		dir.normalise();
		/*Vector3 src = getForward();
		Quaternion quat = src.getRotationTo(dir);

		return (mSceneNode->convertWorldToLocalOrientation(quat) * dir);*/

		return getLocalRotation() * dir;
	}

	return Vector3::NEGATIVE_UNIT_Z;
}

Vector3 Transform::inverseTransformPoint(const Vector3& position)
{
	if (initialized)
	{
		return Convert::toKernel(mSceneNode->convertWorldToLocalPosition(Convert::toOgre(position)));
	}

	return Vector3::ZERO;
}

Vector3 Transform::transformDirection(const Vector3& direction)
{
	if (initialized)
	{
		Vector3 dir = direction;
		dir.normalise();
		//Vector3 src = Vector3::NEGATIVE_UNIT_Z;
		//Quaternion quat = src.getRotationTo(dir);
		//Quaternion worldquat = mSceneNode->convertLocalToWorldOrientation(quat);
		//src = worldquat * dir;

		//return (src);
		return getRotation() * dir;
	}

	return Vector3::NEGATIVE_UNIT_Z;
}

Vector3 Transform::transformPoint(const Vector3& position)
{
	if (initialized)
	{
		return Convert::toKernel(mSceneNode->convertLocalToWorldPosition(Convert::toOgre(position)));
	}

	return Vector3::ZERO;
}

bool Transform::isChildOf(Transform* parent)
{
	return (getParent() == parent);
}

void Transform::rotate(const Vector3& eulerAngles, Space relativeTo /*= Space::SELF*/)
{
	if (initialized)
	{
		Matrix3 mx;
		mx.FromEulerAnglesYXZ(Degree(eulerAngles.y), Degree(eulerAngles.x), Degree(eulerAngles.z));

		Quaternion quat(mx);
		mSceneNode->rotate(Convert::toOgre(quat), Ogre::Node::TransformSpace(relativeTo));

		changed = true;
	}
}

void Transform::rotate(float xAngle, float yAngle, float zAngle, Space relativeTo /*= Space::SELF*/)
{
	rotate(Vector3(xAngle, yAngle, zAngle), relativeTo);
}

void Transform::rotate(const Vector3& axis, float angle, Space relativeTo /*= Space::SELF*/)
{
	if (initialized)
	{
		mSceneNode->rotate(Convert::toOgre(axis), Convert::toOgre(Degree(angle)), Ogre::Node::TransformSpace(relativeTo));

		changed = true;
	}
}

void Transform::rotateAround(const Vector3& point, const Vector3& axis, float angle)
{
	if (initialized)
	{
		Vector3 diff = point - getPosition();
		setPosition(point);
		rotate(axis, angle, Space::WORLD);
		translate(diff, Space::WORLD);

		changed = true;
	}
}
