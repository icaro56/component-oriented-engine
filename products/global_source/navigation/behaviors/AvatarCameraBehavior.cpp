#include "AvatarCameraBehavior.h"

#include "surface/SurfaceManager.h"

#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"

AvatarCameraBehavior::AvatarCameraBehavior()
	: mTargetAvatar(0),
	  avatarDistanceZ(0),
	  avatarDistanceY(0),
	  avatarDistanceX(0),
	  distance(Vector3::ZERO)
{
	setCameraBehaviorType(CBT_AVATAR);
}

AvatarCameraBehavior::~AvatarCameraBehavior()
{
	if (mTargetAvatar)
	{
		GameObjectManager::getInstance()->destroy(mTargetAvatar);
		mTargetAvatar = 0;
	}
}

void AvatarCameraBehavior::update()
{
	if (pInput->getRenderView()->isOnRender() && pInput->getRenderView()->isActiveWindow())
	{
		if (pInput->mouseZDelta()  < 0)
		{
			double c = SurfaceManager::getInstance()->kRealTo3D();
			
			if(avatarDistanceZ < 9 * c)
			{
				avatarDistanceZ += 0.15 * c;
				if(avatarDistanceZ < 1.35 * c && avatarDistanceZ > 0.60 * c)
					avatarDistanceX -= 0.045 * c;

				distance = Vector3(avatarDistanceX,avatarDistanceY,avatarDistanceZ);
				setYawPitchDist(180, 0 , distance);
			}
		}
		if (pInput->mouseZDelta()  > 0)
		{
			double c = SurfaceManager::getInstance()->kRealTo3D();
			if(avatarDistanceZ > 0.60 * c)
			{
				avatarDistanceZ -= 0.15 *c;
				if(avatarDistanceZ < 1.35 * c && avatarDistanceZ > 0.60 * c)
					avatarDistanceX += 0.045 * c;

				distance = Vector3(avatarDistanceX,avatarDistanceY,avatarDistanceZ);
				setYawPitchDist(180,0,distance);
			}
		}
	}
}

bool AvatarCameraBehavior::keyPressed(const OIS::KeyEvent &arg)
{
	if (pInput->getRenderView()->isOnRender() && (!pInput->getKey(OIS::KC_LCONTROL)))
	{
		/*if (arg.key == OIS::KC_SPACE)
		{
			pInput->grabMouse(!pInput->isMouseGrabbed());
		}*/
	}

	return true;
}

bool AvatarCameraBehavior::mouseMoved( const OIS::MouseEvent &arg )
{
	if (pInput->isMouseGrabbed())
	{
		mTargetAvatar->transform->turnX(arg.state.Y.rel * 0.1f);
	}

	return true;
}

bool AvatarCameraBehavior::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
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

void AvatarCameraBehavior::prepareBeforeChangeBehavior()
{

}

void AvatarCameraBehavior::prepareNextChangeBehavior()
{
	double c = SurfaceManager::getInstance()->kRealTo3D();
    avatarDistanceZ = 3.35 * c;//variaveis inicializadas
    avatarDistanceY = 0;//para setar a distancia entre
    avatarDistanceX = 0;//camera e avatar

	if (mTargetAvatar)
	{
		mTargetAvatar->transform->setInheritOrientation(true);
		mTargetAvatar->transform->setInheritScale(false);
	}
}

void AvatarCameraBehavior::configurePrerequisites(const QString& name)
{
	mTargetAvatar = new GameObject(name.toStdString() + "AvatarTargetParent");
	mTargetAvatar->transform->setFixedYawAxis(true);
}

void AvatarCameraBehavior::setYawPitchDist(const float yaw, const float pitch, const Vector3 &dist)
{
	if (cameraObject)
	{
		cameraObject->transform->setLocalPosition(Vector3::ZERO);
		cameraObject->transform->resetOrientation();
		cameraObject->transform->turnY(yaw);
		cameraObject->transform->turnX(-pitch);
		cameraObject->transform->translate(dist);
	}
}

void AvatarCameraBehavior::setTarget(GameObject* target)
{
	if (target)
	{
		target->transform->attachChild(mTargetAvatar->transform);
		mTargetAvatar->transform->attachChild(cameraObject->transform);
		mTargetAvatar->transform->setLocalPosition(Vector3::ZERO);
        mTargetAvatar->transform->resetOrientation();

		double c = SurfaceManager::getInstance()->kRealTo3D();
		Vector3 pos = target->transform->getPosition();
		pos.y += 1.65 * c;
		mTargetAvatar->transform->setPosition(pos);
			
        Vector3 dist = Vector3(avatarDistanceX,avatarDistanceY,avatarDistanceZ);
        //Vector3 dist = Vector3(0, 0, 5);
        setYawPitchDist(180, 0, dist);
	}
}

GameObject* AvatarCameraBehavior::getTarget()
{
	return mTargetAvatar;
}

