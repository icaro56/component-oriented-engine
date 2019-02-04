#include "CameraBehavior.h"

#include "gameobject/GameObject.h"

CameraBehavior::CameraBehavior()
	: //pCamera(0),
	  cameraObject(0),
	  topSpeed(0),
	  mFastMove(false),
	  collisionEnabled(false)
{
	setCameraBehaviorType(CBT_NONE);
}

CameraBehavior::~CameraBehavior()
{
	/*if (pCamera)
	{
		pCamera = 0;
	}*/

	if (pInput)
	{
		pInput = 0;
	}
}

void CameraBehavior::update()
{

}

bool CameraBehavior::keyPressed(const OIS::KeyEvent &arg)
{
	return true;
}

bool CameraBehavior::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

bool CameraBehavior::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

bool CameraBehavior::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

bool CameraBehavior::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

/*void CameraBehavior::setCamera(Camera* cam)
{
	pCamera = cam;
}

Camera* CameraBehavior::getCamera()
{
	return pCamera;
}*/

void CameraBehavior::setCamera(GameObject* cam)
{
	cameraObject = cam;
}

GameObject* CameraBehavior::getCamera()
{
	return cameraObject;
}

void CameraBehavior::setCameraBehaviorType(CameraBehaviorTypes cbt)
{
	cameraBehaviorType = cbt;
}

CameraBehaviorTypes CameraBehavior::getCameraBehaviorType()
{
	return cameraBehaviorType;
}

void CameraBehavior::setInput(InputManager* i)
{
	pInput = i;
}

InputManager* CameraBehavior::getInput()
{
	return pInput;
}

void CameraBehavior::setTopSpeed(double speed)
{
	topSpeed = speed;
}

double CameraBehavior::getTopSpeed()
{
	return topSpeed;
}

void CameraBehavior::prepareBeforeChangeBehavior()
{

}

void CameraBehavior::prepareNextChangeBehavior()
{

}

void CameraBehavior::setFastMove(bool active)
{
	mFastMove = active;
}

bool CameraBehavior::isFastMove()
{
	return mFastMove;
}

void CameraBehavior::setCollisionEnabled(bool active)
{
	collisionEnabled = active;
}

bool CameraBehavior::isCollisionEnabled()
{
	return collisionEnabled;
}

void CameraBehavior::setTarget(GameObject* t)
{
}

GameObject* CameraBehavior::getTarget()
{
	return NULL;
}

void CameraBehavior::configurePrerequisites(const QString& name)
{

}