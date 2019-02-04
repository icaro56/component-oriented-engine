#include "FreeCameraBehavior.h"


#include <engine/RenderView.h>

#include "system/ProjectConfig.h"
#include "surface/SurfaceManager.h"
#include <time/Time.h>

#include "gameobject/GameObject.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"

FreeCameraBehavior::FreeCameraBehavior()
	: mVelocity(Vector3::ZERO),
	  mGoingForward(false),
	  mGoingBack(false),
	  mGoingLeft(false),
	  mGoingRight(false),
	  mGoingUp(false),
	  mGoingDown(false),
	  mSlowdown(false),
	  speedCam(0),
	  isTop(false)
{
	setCameraBehaviorType(CBT_FREE);
}

FreeCameraBehavior::~FreeCameraBehavior()
{

}

void FreeCameraBehavior::update()
{
	Transform* transform = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	Vector3 oldPos = transform->getPosition();
	Ogre::Real localSpeed = mFastMove ? speedCam * 4 : speedCam;
				
	int mouseWheelPosition = pInput->mouseZDelta();
	int flag = 0;

/*#ifdef TOOL_STATE_MESSAGE

	if (mouseWheelPosition && pInput->getRenderView()->isActiveWindow() && pInput->getRenderView()->isOnRender())
	{
		if (mouseWheelPosition > 0)
		{
		#ifdef TOOL_STATE_MESSAGE
			if(StateMessage::getInstance()->isMouseOver())
		#endif		
				flag = 1;
						
		}
		else if (mouseWheelPosition < 0)
		{
		#ifdef TOOL_STATE_MESSAGE
			if(StateMessage::getInstance()->isMouseOver())
		#endif
				flag = -1;
						
		}
	}
#endif*/

	// build our acceleration vector based on keyboard input composite
	Vector3 accel = Vector3::ZERO;

	if (mGoingForward) accel += transform->getForward();
	if (mGoingBack) accel -= transform->getForward();
	if (mGoingRight) accel += transform->getRight();
	if (mGoingLeft) accel -= transform->getRight();
	if (mGoingUp || (flag == 1)) accel += transform->getUp();
	if (mGoingDown || (flag == -1)) accel -= transform->getUp();

	double dt = Time->getDeltaTime();

	if (mSlowdown)
	{
		if (accel.squaredLength() != 0)
		{
			accel.normalise();
			mVelocity += accel * localSpeed * dt;
		}
		//se não está acelerado diminui a velocida em um certo tempo
		else
		{
			mVelocity -= mVelocity * dt;
		}

		Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

		//mantem a velocidade da camera abaixo do topSpeed e acima de epsilon
		if (mVelocity.squaredLength() > localSpeed * localSpeed)
		{
			mVelocity.normalise();
			mVelocity *= localSpeed;
		}
		else if (mVelocity.squaredLength() < tooSmall * tooSmall)
		{
			mVelocity = Vector3::ZERO;
		}
		if (mVelocity != Vector3::ZERO)
		{
			transform->translate(mVelocity * dt, Space::WORLD);
		}
					
	}
	else
	{
		mVelocity = accel * localSpeed * dt;
		transform->translate(mVelocity, Space::WORLD);
	}

	if( (SurfaceManager::getInstance()->getOpacity() > 0.0 ) &&  (collisionEnabled) )
	{
		Vector3 newPos = transform->getPosition();
					
		if (SurfaceManager::getInstance()->isSurfaceCollision(oldPos, 5, newPos))
		{
			transform->setPosition(newPos);
		}

	}

	Vector3 actualPos = transform->getPosition();

	/*if (!SurfaceManager::getInstance()->isInside(actualPos))
	{
		pCamera->setWorldPosition(actualPos);
	}*/
}

void FreeCameraBehavior::prepareBeforeChangeBehavior()
{
	saveAttributes();
}

void FreeCameraBehavior::prepareNextChangeBehavior()
{
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	if (transformCamera->hasParent())
    {
        transformCamera->detach();
		transformCamera->setPosition(placeRecoveryPosFree.position);
		transformCamera->setLocalRotation(placeRecoveryPosFree.orientation);
    }

	saveAttributes();
}

void FreeCameraBehavior::saveAttributes()
{
	mVelocity = Vector3::ZERO;

	placeRecoveryPosFree.setValue(cameraObject->transform->getPosition(),
								  cameraObject->transform->getRotation());

	mGoingForward = false;
	mGoingBack = false;
	mGoingLeft = false;
	mGoingRight = false;
	mGoingUp = false;
	mGoingDown = false;
}

bool FreeCameraBehavior::keyPressed(const OIS::KeyEvent &arg)
{
	if ( (pInput->getRenderView()->isActiveWindow()) && (pInput->getRenderView()->isOnRender()) )
	{
		if (arg.key == OIS::KC_RETURN)
		{
			if(!mSlowdown)
			{
				setSpeedCam(topSpeed * 3);
				setSlowdown(true);
			}
			else
			{
				setSpeedCam(topSpeed);
				setSlowdown(false);
			}
		}

		if (arg.key == OIS::KC_SPACE)
		{
			saveAttributes();

			pInput->grabMouse(!pInput->isMouseGrabbed());
		}

		if (!pInput->getKey(OIS::KC_LCONTROL))
		{
			if(arg.key == OIS::KC_T)
			{
				if (!pInput->getKey(OIS::KC_LSHIFT))
				{
					swappTop();
				}
			}

			if (arg.key == OIS::KC_W || arg.key == OIS::KC_UP) mGoingForward = true;
			else if (arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN) mGoingBack = true;
			else if (arg.key == OIS::KC_A || arg.key == OIS::KC_LEFT) mGoingLeft = true;
			else if (arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT) mGoingRight = true;
			else if (arg.key == OIS::KC_PGUP) mGoingUp = true;
			else if (arg.key == OIS::KC_PGDOWN) mGoingDown = true;
		}
		
		Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
		
		if( arg.key == OIS::KC_F1)
		{
			if (pInput->getKey(OIS::KC_LSHIFT ))
			{
				mPR[1].setValue(transformCamera->getLocalPosition(), transformCamera->
								getLocalRotation());
			}
			else
			{
				if(mPR[1].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");

					mPR[0].setValue(transformCamera->getLocalPosition(), 
									transformCamera->getLocalRotation());
					transformCamera->setLocalPosition(mPR[1].position);

					transformCamera->setLocalRotation(Quaternion( mPR[1].orientation.w,
												mPR[1].orientation.x,
												mPR[1].orientation.y,
												mPR[1].orientation.z));
				}
			}
		}

		if(arg.key == OIS::KC_F2)
		{
			if (pInput->getKey(OIS::KC_LSHIFT))
			{
				//USERLOG->write("salvo coordenadas da camera", "evento de teclado");
				mPR[2].setValue(transformCamera->getLocalPosition(),
								transformCamera->getLocalRotation());
               
			}
			else
			{
				if(mPR[2].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");
					mPR[0].setValue(transformCamera->getLocalPosition(),
									transformCamera->getLocalRotation());

					transformCamera->setLocalPosition(mPR[2].position);
					transformCamera->setLocalRotation(Quaternion(mPR[2].orientation.w,
												mPR[2].orientation.x,
												mPR[2].orientation.y,
												mPR[2].orientation.z));
				}
			}
		}

		if(arg.key == OIS::KC_F3)
		{
			if (pInput->getKey(OIS::KC_LSHIFT))
			{
				//USERLOG->write("salvo coordenadas da camera", "evento de teclado");
				mPR[3].setValue(transformCamera->getLocalPosition(),
								transformCamera->getLocalRotation());
               
			}
			else
			{
				if(mPR[3].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");
					mPR[0].setValue(transformCamera->getLocalPosition(),
									transformCamera->getLocalRotation());

					transformCamera->setLocalPosition(mPR[3].position);
					transformCamera->setLocalRotation(Quaternion(mPR[3].orientation.w,
												mPR[3].orientation.x,
												mPR[3].orientation.y,
												mPR[3].orientation.z));
				}
			}
		}

		if(arg.key == OIS::KC_F4)
		{
			if (pInput->getKey(OIS::KC_LSHIFT))
			{
				//USERLOG->write("salvo coordenadas da camera", "evento de teclado");
				mPR[4].setValue(transformCamera->getLocalPosition(),
								transformCamera->getLocalRotation());
                
			}
			else
			{
				if (mPR[4].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");
					mPR[0].setValue(transformCamera->getLocalPosition(),
									transformCamera->getLocalRotation());

					transformCamera->setLocalPosition(mPR[4].position);
					transformCamera->setLocalRotation(Quaternion(mPR[4].orientation.w,
												mPR[4].orientation.x,
												mPR[4].orientation.y,
												mPR[4].orientation.z));
				}
			}
		}

		if(arg.key == OIS::KC_F5)
		{
			if (pInput->getKey(OIS::KC_LSHIFT))
			{
				//USERLOG->write("salvo coordenadas da camera", "evento de teclado");
				mPR[5].setValue(transformCamera->getLocalPosition(),
								transformCamera->getLocalRotation());
                
			}
			else
			{
				if (mPR[5].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");
					mPR[0].setValue(transformCamera->getLocalPosition(),
									transformCamera->getLocalRotation());

					transformCamera->setLocalPosition(mPR[5].position);
					transformCamera->setLocalRotation(Quaternion(mPR[5].orientation.w,
												mPR[5].orientation.x,
												mPR[5].orientation.y,
												mPR[5].orientation.z));
				}
			}
		}

		if(arg.key == OIS::KC_F6)
		{
			if (pInput->getKey(OIS::KC_LSHIFT))
			{
				//USERLOG->write("salvo coordenadas da camera", "evento de teclado");
				mPR[6].setValue(transformCamera->getLocalPosition(),
								transformCamera->getLocalRotation());
                
			}
			else
			{
				if (mPR[6].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");
					mPR[0].setValue(transformCamera->getLocalPosition(),
									transformCamera->getLocalRotation());

					transformCamera->setLocalPosition(mPR[6].position);
					transformCamera->setLocalRotation(Quaternion(mPR[6].orientation.w,
												mPR[6].orientation.x,
												mPR[6].orientation.y,
												mPR[6].orientation.z));
				}
			}
		}

		if(arg.key == OIS::KC_F7)
		{
			if (pInput->getKey(OIS::KC_LSHIFT))
			{
				//USERLOG->write("salvo coordenadas da camera", "evento de teclado");
				mPR[7].setValue(transformCamera->getLocalPosition(),
								transformCamera->getLocalRotation());
             
			}
			else
			{
				if (mPR[7].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");
					mPR[0].setValue(transformCamera->getLocalPosition(),
									transformCamera->getLocalRotation());

					transformCamera->setLocalPosition(mPR[7].position );
					transformCamera->setLocalRotation(Quaternion(mPR[7].orientation.w,
												mPR[7].orientation.x,
												mPR[7].orientation.y,
												mPR[7].orientation.z));
				}
			}
		}

		if (arg.key == OIS::KC_F8)
		{
			if (pInput->getKey(OIS::KC_LSHIFT))
			{
				//USERLOG->write("salvo coordenadas da camera", "evento de teclado");
				mPR[8].setValue(transformCamera->getLocalPosition(),
								transformCamera->getLocalRotation());
               
			}
			else
			{
				if(mPR[8].position.x != -99999)
				{
					//USERLOG->write("carregada coordenadas salvas da camera", "evento de teclado");
					mPR[0].setValue(transformCamera->getLocalPosition(),
									transformCamera->getLocalRotation());

					transformCamera->setLocalPosition(mPR[8].position);
					transformCamera->setLocalRotation(Quaternion(mPR[8].orientation.w,
												mPR[8].orientation.x,
												mPR[8].orientation.y,
												mPR[8].orientation.z));
				}
			}
		}

		if((arg.key == OIS::KC_F11)  && (mPR[0].position.x != -99999))
		{
			//USERLOG->write("restaura posicao anterior ao salvamento de alguma camera", "evento de teclado");
			transformCamera->setLocalPosition(mPR[0].position);
			transformCamera->setLocalRotation(Quaternion(mPR[0].orientation.w,
										mPR[0].orientation.x,
										mPR[0].orientation.y,
										mPR[0].orientation.z));
		}

		if(arg.key == OIS::KC_F12)
		{
			//USERLOG->write("carregada coordenadas da camera inicial", "evento de teclado");
			mPR[0].setValue(transformCamera->getLocalPosition(),
							transformCamera->getLocalRotation());

			Vector3 posIni = GlobalConfig::INITIAL_POS_CAM;
			SurfaceManager::getInstance()->convertRealTo3D(posIni);

			transformCamera->setPosition(posIni);
			transformCamera->setLocalRotation(GlobalConfig::INITIAL_ORI_CAM);
		}
	}

	return true;
}

bool FreeCameraBehavior::keyReleased(const OIS::KeyEvent &arg)
{
	if (arg.key == OIS::KC_W || arg.key == OIS::KC_UP) mGoingForward = false;
	else if (arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN) mGoingBack = false;
	else if (arg.key == OIS::KC_A || arg.key == OIS::KC_LEFT) mGoingLeft = false;
	else if (arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT) mGoingRight = false;
	else if (arg.key == OIS::KC_PGUP) mGoingUp = false;
	else if (arg.key == OIS::KC_PGDOWN) mGoingDown = false;

	return true;
}

bool FreeCameraBehavior::mouseMoved( const OIS::MouseEvent &arg )
{
	if (pInput->isMouseGrabbed())
	{
		Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
		transformCamera->turnY(-arg.state.X.rel * 0.1f, Space::WORLD);
		transformCamera->turnX(-arg.state.Y.rel * 0.1f);
	}

	return true;
}

bool FreeCameraBehavior::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if ( (pInput->getRenderView()->isActiveWindow()) && (pInput->getRenderView()->isOnRender()) )
	{
		if ( id == OIS::MB_Middle)
		{
			saveAttributes();
			pInput->grabMouse(!pInput->isMouseGrabbed());
		}

		if (pInput->isMouseGrabbed())
		{
			if ( (id ==  OIS::MB_Left) && (mGoingBack) )
			{
				mGoingForward = false;
				mFastMove = true;
				mGoingBack = true;
			}
			else if (((id ==  OIS::MB_Right) && mGoingForward))
			{
				mGoingForward = true;
				mFastMove = true;
				mGoingBack = false;
			}
			else if ((id ==  OIS::MB_Left) && (!mGoingBack))
				mGoingForward = true;
			else if ((id ==  OIS::MB_Right) && (!mGoingForward))
				mGoingBack = true;
		}
	}

	return true;
}

bool FreeCameraBehavior::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (pInput->isMouseGrabbed())
	{
		if (id ==  OIS::MB_Left) 
		{
			mGoingForward = false;
			mFastMove = false;
		}
		else if (id ==  OIS::MB_Right)
		{
			mGoingBack = false;
			mFastMove = false;
		}
	}

	return true;
}

void FreeCameraBehavior::setTopSpeed(double speed)
{
	CameraBehavior::setTopSpeed(speed);

	if (!mSlowdown)
		setSpeedCam(speed);
	else
		setSpeedCam(speed * 3);
}

void FreeCameraBehavior::setSpeedCam(double s)
{
	speedCam = s;
}

void FreeCameraBehavior::setSlowdown(const bool active)
{
	mSlowdown = active;
}

void FreeCameraBehavior::swappTop()
{
	isTop = !isTop;
	
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	
	if (isTop)
	{
		placeRecoveryBeforeTop.setValue(transformCamera->getPosition(),
										transformCamera->getRotation());

		Vector3 pos = transformCamera->getPosition();

#ifdef MINE_UG
		transformCamera->setLocalPosition(pos.x, 1350 * SurfaceManager::getInstance()->kRealTo3D(), pos.z);
			
#else
		transformCamera->setLocalPosition(Vector3(pos.x,SurfaceManager::getInstance()->getHeightAt(pos.x, pos.z, 4350 * SurfaceManager::getInstance()->kRealTo3D(), true), pos.z));
#endif
			

		transformCamera->resetOrientation();
		transformCamera->turnX(-90);
	}
	else
	{
		transformCamera->setLocalPosition(placeRecoveryBeforeTop.position);
		transformCamera->setLocalRotation(placeRecoveryBeforeTop.orientation);
	}
}

