#include "GoogleEarthCameraBehavior.h"

#include <engine/Engine.h>

#include <time/Time.h>

#include "surface/SurfaceManager.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"
#include "system/ProjectConfig.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"

#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"

GoogleEarthCameraBehavior::GoogleEarthCameraBehavior()
	: canUsePan(true),
	  mTargetParent(0),
	  mTarget(0),
	  posZoomInitial(Ogre::Vector2::ZERO),
	  usedZoomScroll(false),
	  countZoomScroll(0)
{
	setCameraBehaviorType(CBT_GOOGLE_EARTH);
}

GoogleEarthCameraBehavior::~GoogleEarthCameraBehavior()
{
	if (mTarget)
	{
		GameObjectManager::getInstance()->destroy(mTarget);
		mTarget = 0;
	}

	if (mTargetParent)
	{
		GameObjectManager::getInstance()->destroy(mTargetParent);
		mTargetParent = 0;
	}
}

void GoogleEarthCameraBehavior::configurePrerequisites(const QString& name)
{
	mTargetParent = new GameObject(name.toStdString() + "GETargetParent");
	mTargetParent->transform->setFixedYawAxis(true);
	
    //mTarget = new SceneNodeOld(name.toStdString() + "GETarget", mTargetParent);
	mTarget = new GameObject(name.toStdString() + "GETarget");
	mTargetParent->transform->attachChild(mTarget->transform);
	mTarget->transform->setFixedYawAxis(true);
}

void GoogleEarthCameraBehavior::update()
{
	if (!pInput->isMouseGrabbed())
	{
		Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
		Vector3 oldPos = transformCamera->getPosition();

		if (pInput->getMouseButton(OIS::MB_Right))
		{
			zoom(pInput->mouseY());
		}
						
		if (pInput->mouseZDelta())
		{
			setTargetPos3(pInput->mouseX(), pInput->mouseY());
			zoomScroll();
			usedZoomScroll = true;
		}
		else
		{
			if (usedZoomScroll)
			{
				countZoomScroll += Time->getDeltaTime();
				
				if (countZoomScroll > 0.7)
				{
					QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "handopen.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
					QCursor c = QCursor(pix);
					pInput->getRenderView()->setCursor(c);
					usedZoomScroll = false;
					countZoomScroll = 0;
				}
			}
		}

		if( (SurfaceManager::getInstance()->getOpacity() > 0.0 ) &&  (collisionEnabled) )
		{
			Vector3 newPos = transformCamera->getPosition();
					
			if (SurfaceManager::getInstance()->isSurfaceCollision(oldPos, 5, newPos))
			{
				transformCamera->setPosition(newPos);
			}

		}

		Vector3 actualPos = transformCamera->getPosition();

		if (!SurfaceManager::getInstance()->isInside(actualPos))
		{
			transformCamera->setPosition(actualPos);
		}
	}
}

void GoogleEarthCameraBehavior::prepareBeforeChangeBehavior()
{
	saveAttributes();

	if (pInput->getRenderView())
		pInput->getRenderView()->unsetCursor();
}

void GoogleEarthCameraBehavior::prepareNextChangeBehavior()
{
	QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "handopen.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QCursor c = QCursor(pix);
	pInput->getRenderView()->setCursor(c);
	
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	if (transformCamera->hasParent())
    {
        transformCamera->detach();

		if (placeRecoveryPosFree.wasInitialized())
		{
			transformCamera->setPosition(placeRecoveryPosFree.position);
			transformCamera->setLocalRotation(placeRecoveryPosFree.orientation);
		}
		else
		{
			Vector3 posIni = GlobalConfig::INITIAL_POS_CAM;
			SurfaceManager::getInstance()->convertRealTo3D(posIni);

			placeRecoveryPosFree.setValue(posIni, GlobalConfig::INITIAL_ORI_CAM);
			transformCamera->setPosition(placeRecoveryPosFree.position);
			transformCamera->setLocalRotation(placeRecoveryPosFree.orientation);
		}
    }
}

bool GoogleEarthCameraBehavior::mouseMoved( const OIS::MouseEvent &arg )
{
	if ( canUsePan )
	{
		if (pInput->getMouseButton(OIS::MB_Left))
		{
			pan();
		}
		else if (pInput->getMouseButton(OIS::MB_Middle))
		{
			rotate();
		}
	}

	return true;
}

bool GoogleEarthCameraBehavior::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (pInput->getRenderView()->isActiveWindow() && pInput->getRenderView()->isOnRender())
	{
		if (!pInput->isMouseGrabbed())
		{
			//QString evt = cameraObject->camera->mousePickEntity();

			Ray ray = cameraObject->camera->screenPointToRay(Vector2(arg.state.X.abs, arg.state.Y.abs));
			bool collided = cameraObject->getParentScene()->raycastRendering(ray);
					
		#ifdef FUNC_MINI_MAP
			bool cond = !MiniMap::getInstance()->onMapElements();
		#else
			bool cond = true;
		#endif

			if (!collided && cond)
				canUsePan = true;
			else
				canUsePan = false;

			if (canUsePan)
			{
				if (id == OIS::MB_Left)
				{
					QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "handclosed.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

					QCursor c = QCursor(pix);

					pInput->getRenderView()->setCursor(c);
				}
				else if (id == OIS::MB_Right)
				{
					QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "lupa.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

					QCursor c = QCursor(pix);

					pInput->getRenderView()->setCursor(c);
					setTargetPos(arg.state.X.abs, arg.state.Y.abs);
				}
				else if (id == OIS::MB_Middle)
				{
					QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "orbital.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

					QCursor c = QCursor(pix);

					pInput->getRenderView()->setCursor(c);

					setTargetPos2(arg.state.X.abs, arg.state.Y.abs);
				}
			}
		}
	}
	return true;
}

bool GoogleEarthCameraBehavior::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (!pInput->isMouseGrabbed())
	{
		if ( canUsePan)
		{
			canUsePan = false;

			if (id == OIS::MB_Right || id == OIS::MB_Middle)
				detachFromTarget();

			//if (id == OIS::MB_Left)
			{
				QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "handopen.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

				QCursor c = QCursor(pix);

				pInput->getRenderView()->setCursor(c);
			}
		}
	}

	return true;
}

void GoogleEarthCameraBehavior::pan()
{
	float x = - pInput->mouseXDelta();
	float y = pInput->mouseYDelta();

	if (x != 0 || y != 0)
	{
		Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
		Quaternion quat = transformCamera->getRotation();
		Vector3 accel = quat * (Vector3(x, 0, 0)) + Vector3::UNIT_Y.crossProduct( quat * transformCamera->getRight()) * y;
		
		float localSpeed = mFastMove ? topSpeed * 4 : topSpeed;
		accel *= localSpeed * Time->getDeltaTime();
		

		//*************************calculando distancia para o terreno
		/*int mX = x;
		int mY = y;
		Ogre::Vector2 pos = GRender->getRenderView("render1")->
							getAbsolutePosition();

		float largura = float(GRender->getRenderView("render1")->
								getRenderWindow()->getWidth());
		float altura = float(GRender->getRenderView("render1")->
								getRenderWindow()->getHeight());

		//calculando a posição correta
		double mx = (mX - pos.x);
		double my = (mY - pos.y);*/

		Vector3 p = transformCamera->getPosition();
		p.y = SurfaceManager::getInstance()->getHeightAt(p.x, p.z ,0.0, true);

		/*Vector3 mousePos = pTerrain->mouseWorldPosition(mx ,
									my, largura, altura,
									GRender->getRenderView("render1")->
									getViewport()->getCamera());*/

		float dist = sqrt( abs( p.distance(transformCamera->getPosition()))) * 0.02;

		transformCamera->translate(accel * dist, Space::WORLD);
	}
}

void GoogleEarthCameraBehavior::zoom(int mousey)
{
	float y = mousey;
	float dif = posZoomInitial.y - y;

	if (dif > 1 || dif < -1)
	{
		Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
		Transform* transformTargetParent = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
		
		Vector3 posTarget = transformTargetParent->getPosition();
		Vector3 posCamera = transformCamera->getPosition();
		float dist = abs( posTarget.distance(posCamera));

		if ( (dist >= 30 && dif < -1) || (dist <= 64000 && dif > 1 ))
		{
			Vector3 dir = posTarget - posCamera;
			dir.normalise();

			Vector3 accel = -dir * (dif * 0.0001) * dist;

			float localSpeed = mFastMove ? topSpeed * 4 : topSpeed;
			accel *= localSpeed * Time->getDeltaTime();
			transformCamera->translate(accel, Space::WORLD);
		}
	}

	if (dif < -1)
	{
		QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "lupamax.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QCursor c = QCursor(pix);
		pInput->getRenderView()->setCursor(c);
	}
	else if (dif > 1)
	{
		QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "lupaminus.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QCursor c = QCursor(pix);
		pInput->getRenderView()->setCursor(c);
	}
	else
	{
		QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "lupa.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QCursor c = QCursor(pix);
		pInput->getRenderView()->setCursor(c);
	}
}

void GoogleEarthCameraBehavior::zoomScroll()
{
	float y = pInput->mouseZDelta() > 0 ? 1 : -1;
	
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	Transform* transformTargetParent = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
		
	Vector3 posTarget = transformTargetParent->getPosition();
	Vector3 posCamera = transformCamera->getPosition();
	float dist = abs( posTarget.distance(posCamera));

	if ( (dist >= 30 && y >= 0) || (dist <= 64000 && y <= 0 ))
	{
		Vector3 dir = posTarget - posCamera;
		dir.normalise();

		Vector3 accel = dir * (y * 0.01) * dist;
		float localSpeed = mFastMove ? topSpeed * 4 : topSpeed;
		accel *= localSpeed * Time->getDeltaTime();
		transformCamera->translate(accel, Space::WORLD);
	}

	if (y < 0)
	{
		QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "lupaminus.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QCursor c = QCursor(pix);
		pInput->getRenderView()->setCursor(c);
	}
	else if (y > 0)
	{
		QPixmap pix = QPixmap(GlobalConfig::CURSORPATH + "lupamax.png").scaled(32,32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QCursor c = QCursor(pix);
		pInput->getRenderView()->setCursor(c);
	}
}

void GoogleEarthCameraBehavior::rotate()
{
	int deltaX = -pInput->mouseXDelta();
	
	if (deltaX != 0)
	{
		Transform* transformTargetParent = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
		transformTargetParent->turnY(deltaX * 0.1f, Space::WORLD);
	}

	//não funcionando corretamente
	int deltaY = pInput->mouseYDelta();

	if (deltaY != 0)
	{
		Transform* transformTarget = dynamic_cast<Transform*>(mTarget->getComponent("Transform"));
		float pitch =  transformTarget->getPitch();
		if (pitch > -85 || deltaY > 0)
			transformTarget->turnX(deltaY * 0.1f);
	}
}

void GoogleEarthCameraBehavior::setTargetPos(int x, int y)
{
	Transform* transformTarget = dynamic_cast<Transform*>(mTarget->getComponent("Transform"));
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	
	if (transformCamera->getParent() != transformTarget)
	{
		int mX = x;
		int mY = y;
		Vector2 pos = pInput->getRenderView()->getAbsolutePosition();

		float largura = float(pInput->getRenderView()->
								getRenderWindow()->getWidth());
		float altura = float(pInput->getRenderView()->
								getRenderWindow()->getHeight());

		//calculando a posição correta
		double mx = (mX - pos.x);
		double my = (mY - pos.y);

		Vector3 mousePos = SurfaceManager::getInstance()->mouseWorldPosition(mx ,
									my, largura, altura);

		
		

		if( (mousePos.x != 599994) && (mousePos.y != 599994)
				&& (mousePos.z != 599994 ))
		{
			Transform* transformTargetParent = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
			saveAttributes();
			transformTargetParent->setPosition(mousePos);
			transformTarget->setPosition(mousePos);
			transformTarget->attachChild(transformCamera);
			transformCamera->setPosition(placeRecoveryPosFree.position);
			transformCamera->setRotation(placeRecoveryPosFree.orientation);
			posZoomInitial = Ogre::Vector2(mX, mY);
		}
	}
}

void GoogleEarthCameraBehavior::setTargetPos2(int x, int y)
{
	Transform* transformTarget = dynamic_cast<Transform*>(mTarget->getComponent("Transform"));
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	
	if (transformCamera->getParent() != transformTarget)
	{
		int mX = x;
		int mY = y;
		Vector2 pos = pInput->getRenderView()->getAbsolutePosition();

		float largura = float(pInput->getRenderView()->
								getRenderWindow()->getWidth());
		float altura = float(pInput->getRenderView()->
								getRenderWindow()->getHeight());

		//calculando a posição correta
		double mx = (mX - pos.x);
		double my = (mY - pos.y);

		Vector3 mousePos = SurfaceManager::getInstance()->mouseWorldPosition(mx ,
									my, largura, altura);

		double mx2 = (largura/2 - pos.x);
		double my2 = (mY - pos.y);

		Vector3 posTarget = SurfaceManager::getInstance()->mouseWorldPosition(mx2 ,
									my2, largura, altura);

		if( (mousePos.x != 599994) && (mousePos.y != 599994)
				&& (mousePos.z != 599994 ))
		{
			saveAttributes();
			Transform* transformTargetParent = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
			transformTargetParent->setPosition(mousePos);
			transformTarget->setPosition(posTarget);
			transformTarget->setRotation(placeRecoveryPosFree.orientation);
			transformTarget->attachChild(transformCamera);
			transformCamera->setPosition(placeRecoveryPosFree.position);
			transformCamera->setRotation(placeRecoveryPosFree.orientation);
		}
	}
}

void GoogleEarthCameraBehavior::setTargetPos3(int x, int y)
{
	Transform* transformTarget = dynamic_cast<Transform*>(mTarget->getComponent("Transform"));
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	if (transformCamera->getParent() != transformTarget)
	{
		int mX = x;
		int mY = y;
		Vector2 pos = pInput->getRenderView()->getAbsolutePosition();

		float largura = float(pInput->getRenderView()->
								getRenderWindow()->getWidth());
		float altura = float(pInput->getRenderView()->
								getRenderWindow()->getHeight());

		//calculando a posição correta
		double mx = (mX - pos.x);
		double my = (mY - pos.y);

		Vector3 mousePos = SurfaceManager::getInstance()->mouseWorldPosition(mx ,
									my, largura, altura);

		if( (mousePos.x != 599994) && (mousePos.y != 599994)
				&& (mousePos.z != 599994 ))
		{
			Transform* transformTargetParent = dynamic_cast<Transform*>(mTargetParent->getComponent("Transform"));
			saveAttributes();
			transformTargetParent->setPosition(mousePos);
			transformTarget->setPosition(mousePos);
		}
	}
}

void GoogleEarthCameraBehavior::detachFromTarget()
{
		if (!pInput->mouseZDelta())
		{
			Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
			if (transformCamera->hasParent() && transformCamera->getParent() != 0)
			{
				saveAttributes();
				transformCamera->detach();

				transformCamera->setPosition(placeRecoveryPosFree.position);
				transformCamera->setLocalRotation(placeRecoveryPosFree.orientation);
			}
		}
}

void GoogleEarthCameraBehavior::saveAttributes()
{
	Transform* transformCamera = dynamic_cast<Transform*>(cameraObject->getComponent("Transform"));
	placeRecoveryPosFree.setValue(transformCamera->getPosition(),
								  transformCamera->getRotation());
}