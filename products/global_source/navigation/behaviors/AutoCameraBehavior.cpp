#include "AutoCameraBehavior.h"

#include <time/Time.h>

#include "system/IniConfig.h"
#include "surface/SurfaceManager.h"

#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"

AutoCameraBehavior::AutoCameraBehavior()
	: mTargetParent(0),
	  axisZ(0)
{
	setCameraBehaviorType(CBT_AUTOMATIC);
}

AutoCameraBehavior::~AutoCameraBehavior()
{
	if (mTargetParent)
	{
		GameObjectManager::getInstance()->destroy(mTargetParent);
		mTargetParent = 0;
	}
}

void AutoCameraBehavior::update()
{
	Transform* transform = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	Vector3 oldPos = transform->getPosition();
	Ogre::Real localSpeed = mFastMove ? topSpeed * 4 : topSpeed;

	Transform* transformTarget = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));

	int z1 = 0;
	double dt = Time->getDeltaTime();

	if (pInput->getRenderView()->isActiveWindow() && pInput->getRenderView()->isOnRender())
	{
		if (pInput->mouseZDelta() < 0 )
		{
			z1 = 1;
		}
		else if (pInput->mouseZDelta() > 0)
		{
			z1 = -1;
		}
	}

	if (transform->getPosition().distance(transformTarget->getPosition()) > 100 || z1 == 1 || axisZ == 1)
	{
		if (z1)
		{
			transform->translate(Vector3(0, 0, z1 * dt * 0.1f * localSpeed));
		}
		else
		{
			transform->translate(Vector3(0, 0, axisZ * dt * 0.07f * localSpeed));
		}
					
	}

	transform->lookAt(transformTarget);

	Vector3 newPos = transform->getPosition();

	if (!SurfaceManager::getInstance()->isInside(newPos))
	{
		transform->setPosition(newPos);
	}

	if( (SurfaceManager::getInstance()->getOpacity() > 0.0 ) && (collisionEnabled) )
	{
		if (SurfaceManager::getInstance()->isSurfaceCollision(oldPos, 5, newPos))
		{
			transform->setPosition(newPos);
		}
	}
}

bool AutoCameraBehavior::keyPressed(const OIS::KeyEvent &arg)
{
	if (pInput->getRenderView()->isOnRender() && (!pInput->getKey(OIS::KC_LCONTROL)))
	{
		if (arg.key == OIS::KC_W)
		{
			axisZ = -1;
		}
		else if (arg.key == OIS::KC_S)
		{
			axisZ = 1;
		}

		if (arg.key == OIS::KC_SPACE)
		{
			pInput->grabMouse(!pInput->isMouseGrabbed());
		}
	}

	return true;
}

bool AutoCameraBehavior::keyReleased(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_W)
	{
		axisZ = 0;
	}
	else if (arg.key == OIS::KC_S)
	{
		axisZ = 0;
	}

	return true;
}

bool AutoCameraBehavior::mouseMoved( const OIS::MouseEvent &arg )
{
	if (pInput->isMouseGrabbed())
	{
		if (mTargetParent)
		{
			Transform* transformTarget = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
			transformTarget->turnY(-arg.state.X.rel * 0.1f, Space::WORLD);
			transformTarget->turnX(-arg.state.Y.rel * 0.1f);
		}
	}

	return true;
}

bool AutoCameraBehavior::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if ( (pInput->getRenderView()->isActiveWindow()) && (pInput->getRenderView()->isOnRender()) )
	{
		if ( id == OIS::MB_Middle)
		{
			pInput->grabMouse(!pInput->isMouseGrabbed());
		}
	}

	return true;
}

void AutoCameraBehavior::prepareBeforeChangeBehavior()
{

}

void AutoCameraBehavior::prepareNextChangeBehavior()
{
	if (mTargetParent)
	{
		Transform* transformTarget = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
		transformTarget->setInheritOrientation(false);
		transformTarget->setInheritScale(false);
	}
}

void AutoCameraBehavior::setTarget(GameObject* target)
{
	if (target)
	{
		Transform* transformTarget = dynamic_cast<Transform*>(target->getComponent("Transform"));
		Transform* transformTargetParent = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
		Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
		
		transformTarget->attachChild(transformTargetParent);
		transformTargetParent->attachChild(transformCamera);
		transformTargetParent->setLocalPosition(Vector3::ZERO);
        transformTargetParent->resetOrientation();

		//falta
		//double d = ConfigIni::getInstance()->getAttribute(CustomPreferences::DistOrbitCam).toDouble();
		double d = 15;

		if (d == 0)
			d = 7.5;

		Vector3 dist;
		double c = SurfaceManager::getInstance()->kRealTo3D();
        dist = Vector3(0, d * c, d * c);

		setYawPitchDist(0, 0, dist);
	}
}

GameObject* AutoCameraBehavior::getTarget()
{
	return mTargetParent;
}

//-----------------------------------------------------------------------------
void AutoCameraBehavior::setYawPitchDist(const float yaw, const float pitch, const Vector3 &dist)
{
	if (cameraObject)
	{
		Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
		
		transformCamera->setLocalPosition(Vector3::ZERO);
		transformCamera->resetOrientation();
		transformCamera->turnY(yaw);
		transformCamera->turnX(-pitch);
		transformCamera->translate(dist);
	}
}

void AutoCameraBehavior::configurePrerequisites(const QString& name)
{
	mTargetParent = new GameObject(name.toStdString() + "AutomaticTargetParent");
	mTargetParent->transform->setFixedYawAxis(true);
}

