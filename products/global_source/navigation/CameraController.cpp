#include "CameraController.h" 

#include "behaviors/CameraBehavior.h"
#include "behaviors/FreeCameraBehavior.h"
#include "behaviors/AutoCameraBehavior.h"
#include "behaviors/AvatarCameraBehavior.h"
#include "behaviors/GoogleEarthCameraBehavior.h"
#include "behaviors/OrbitCameraBehavior.h"


#include <engine/Engine.h>
#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"
#include "components/audio/AudioListener.h"

#include "surface/SurfaceManager.h"
#include "system/ProjectConfig.h"

#include "view/minimap/element/MiniMapElement.h"

//#include "moc_CameraController.cpp"

CameraController::CameraController(const QString& n, 
								   InputManager* i, 
								   RenderView* render, 
								   int flags)
	: //camera(0),
	  gameObjectCamera(0),
	  activeCameraBehavior(0),
	  pInput(i),
	  pRenderView(render),
	  name(n),
	  oldPos(Vector3::ZERO),
	  oldOri(Quaternion::IDENTITY),
	  defaultTopSpeed(0),
	  mFastMove(false),
	  //myTarget(NULL),
	  myTargetObject(0),
	  miniMapElement(NULL)
{
	//camera = new Camera(name.toStdString());

	Camera* cameraComponent = new Camera();

	gameObjectCamera = new GameObject(name.toStdString());
	gameObjectCamera->addComponent(cameraComponent);

	pRenderView->setCamera(cameraComponent);
	cameraComponent->setVisibilityMask(flags);

	pInput->addKeyListener(this);
	pInput->addMouseListener(this);
}

CameraController::~CameraController()
{
	/*if (camera)
	{
		delete camera;
		camera = 0;
	}*/

	if (gameObjectCamera)
	{
		GameObjectManager::getInstance()->destroy(gameObjectCamera);
		gameObjectCamera = 0;
	}

	if (activeCameraBehavior)
	{
		activeCameraBehavior = 0;
	}

	destroyAllCameraBehavior();

	if (pRenderView)
	{
		pRenderView = 0;
	}

	if (miniMapElement)
	{
		delete miniMapElement;
		miniMapElement = 0;
	}

	if (pInput)
	{
		pInput = 0;
	}
}

void CameraController::update()
{
	if (activeCameraBehavior)
	{
		activeCameraBehavior->update();

		//testando se camera se moveu. Então emitimos um sinal informando mudança
		/*Vector3 newPos = camera->getWorldPosition();
		Quaternion newOri = camera->getWorldOrientation();*/
		
		Vector3 newPos = dynamic_cast<Transform*>(gameObjectCamera->getComponent("Transform"))->getPosition();
		Quaternion newOri = dynamic_cast<Transform*>(gameObjectCamera->getComponent("Transform"))->getRotation();

		if ( (oldPos != newPos) || (oldOri != newOri) )
		{
			emit cameraMoved();

			oldPos = newPos;
			oldOri = newOri;
		}
	}
}

bool CameraController::keyPressed(const OIS::KeyEvent &arg)
{
	if (activeCameraBehavior)
	{
		activeCameraBehavior->keyPressed(arg);
	}

	if ( (pInput->getRenderView()->isActiveWindow()) && (pInput->getRenderView()->isOnRender()) )
	{
		if (arg.key == OIS::KC_LSHIFT)
		{
			mFastMove = true;

			QMapIterator<CameraBehaviorTypes, CameraBehavior*> i(cameraBehaviors);
			while (i.hasNext())
			{
				i.next();
				CameraBehavior* temp = i.value();
				temp->setFastMove(mFastMove);
			}
		}
	}

	return true;
}

bool CameraController::keyReleased(const OIS::KeyEvent &arg)
{
	if (activeCameraBehavior)
	{
		activeCameraBehavior->keyReleased(arg);
	}

	if (arg.key == OIS::KC_LSHIFT)
	{
		mFastMove = false;

		QMapIterator<CameraBehaviorTypes, CameraBehavior*> i(cameraBehaviors);
		while (i.hasNext())
		{
			i.next();
			CameraBehavior* temp = i.value();
			temp->setFastMove(mFastMove);
		}
	}

	return true;
}

bool CameraController::mouseMoved( const OIS::MouseEvent &arg )
{
	if (activeCameraBehavior)
	{
		activeCameraBehavior->mouseMoved(arg);
	}

	return true;
}

bool CameraController::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (activeCameraBehavior)
	{
		activeCameraBehavior->mousePressed(arg, id);
	}

	return true;
}

bool CameraController::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (activeCameraBehavior)
	{
		activeCameraBehavior->mouseReleased(arg, id);
	}

	return true;
}

void CameraController::createCameraBehavior(CameraBehaviorTypes cbt)
{
	if (!cameraBehaviors.contains(cbt))
	{
		CameraBehavior* cBehavior;

		switch(cbt)
		{
			case CBT_NONE:
			{
				cBehavior = new CameraBehavior();
				break;
			}
			case CBT_FREE:
			{
				cBehavior = new FreeCameraBehavior();
				break;
			}
			case CBT_ORBIT:
			{
				cBehavior = new OrbitCameraBehavior();
				break;
			}
			case CBT_AVATAR:
			{
				cBehavior = new AvatarCameraBehavior();
				break;
			}
			case CBT_AUTOMATIC:
			{
				cBehavior = new AutoCameraBehavior();
				break;
			}
			case CBT_GOOGLE_EARTH:
			{
				cBehavior = new GoogleEarthCameraBehavior();
				break;
			}
			default:
			{
				cBehavior = new CameraBehavior();
				break;
			}
		}

		//cBehavior->setCamera(camera);
		cBehavior->setCamera(gameObjectCamera);
		cBehavior->setInput(pInput);
		cBehavior->setTopSpeed(defaultTopSpeed);
		cBehavior->configurePrerequisites(name);
		cameraBehaviors.insert(cbt, cBehavior);
	}
	else
	{
		std::cerr << "\nControlador de Camera " << name.toStdString() << 
			"ja possui esse comportamento: " << 
			convertCameraBehaviorTypeToString(cbt).toStdString();
	}
}

void CameraController::destroyCameraBehavior(CameraBehaviorTypes cbt)
{
	if (cameraBehaviors.contains(cbt))
	{
		CameraBehavior* temp = cameraBehaviors.value(cbt);
        delete temp;
        temp = 0;

		cameraBehaviors.remove(cbt);
	}
}

void CameraController::destroyAllCameraBehavior()
{
	if (!cameraBehaviors.empty())
	{
		QMapIterator<CameraBehaviorTypes, CameraBehavior*> i(cameraBehaviors);
        while (i.hasNext())
        {
            i.next();

            CameraBehavior* temp = i.value();
            delete temp;
            temp = 0;
        }

        cameraBehaviors.clear();
	}
}

void CameraController::setCameraBehavior(CameraBehaviorTypes cbt)
{
	if (cameraBehaviors.contains(cbt))
	{
		//antes
		if (activeCameraBehavior)
		{
			//myTarget = NULL;
			myTargetObject = 0;
			activeCameraBehavior->prepareBeforeChangeBehavior();
		}

		//atualiza
		activeCameraBehavior = cameraBehaviors.value(cbt);

		//após
		activeCameraBehavior->prepareNextChangeBehavior();
	}
}

CameraBehaviorTypes CameraController::getCameraBehavior()
{
	return activeCameraBehavior->getCameraBehaviorType();
}

QString CameraController::convertCameraBehaviorTypeToString(CameraBehaviorTypes cbt)
{
	QString text;

	switch(cbt)
	{
		case CBT_NONE:
		{
			text = "CBT_NONE";
			break;
		}
		case CBT_FREE:
		{
			text = "CBT_FREE";
			break;
		}
		case CBT_ORBIT:
		{
			text = "CBT_ORBIT";
			break;
		}
		case CBT_AVATAR:
		{
			text = "CBT_AVATAR";
			break;
		}
		case CBT_AUTOMATIC:
		{
			text = "CBT_AUTOMATIC";
			break;
		}
		case CBT_GOOGLE_EARTH:
		{
			text = "CBT_GOOGLE_EARTH";
			break;
		}
		default:
		{
			text = "NONE";
			break;
		}
	}

	return text;
}

void CameraController::configureMainCamera()
{
	double constante = SurfaceManager::getInstance()->kRealTo3D();

	//double nearTemp = 2.8 / constante;
	double nearTemp = 0.1f;

	Transform* transform = dynamic_cast<Transform*>(gameObjectCamera->getComponent("Transform"));

	Camera* camComponent = dynamic_cast<Camera*>(gameObjectCamera->getComponent("Camera"));

	camComponent->setRange(nearTemp, 500000.0f);

    if (Rendering->renderSystemHasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        camComponent->getOgreCamera()->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

	//Vector3 posIni = GlobalConfig::INITIAL_POS_CAM;
	//SurfaceManager::getInstance()->convertRealTo3D(posIni);
    transform->setPosition(Vector3(30, 50, 30));
	transform->setLocalRotation(GlobalConfig::INITIAL_ORI_CAM);
	camComponent->setLodBias(90 * constante);

	Vector3 posOldCam = transform->getPosition();
	Vector3 posNewCam = transform->getPosition();

    if (SurfaceManager::getInstance()->isSurfaceCollision(posOldCam, 5, posNewCam))
	{
		transform->setPosition(posNewCam);
	}

	// Máscara de seleção ////
	camComponent->setLayer(Physics->DefaultRaycastLayers());

	// Criando componente AudioListener
	gameObjectCamera->addComponent(new AudioListener());

	this->createMiniMapElement();

	connect(this,
			SIGNAL(cameraMoved()),
			this,
			SLOT(updateMiniMapElement()) );
}

void CameraController::setDefaultTopSpeed(double speed, bool updateBehaviors)
{
	defaultTopSpeed = speed;

	QMapIterator<CameraBehaviorTypes, CameraBehavior*> i(cameraBehaviors);
    while (i.hasNext())
    {
        i.next();

        CameraBehavior* temp = i.value();
		temp->setTopSpeed(defaultTopSpeed);
    }
}

double CameraController::getDefaultTopSpeed()
{
	return defaultTopSpeed;
}

void CameraController::setTarget(GameObject* target)
{
	if (activeCameraBehavior)
	{
		myTargetObject = target;
		
		activeCameraBehavior->setTarget(target);
	}
}

/*Camera* CameraController::getCamera()
{
	return camera;
}*/

GameObject* CameraController::getCamera()
{
	return gameObjectCamera;
}

void CameraController::setWorldPositionToMap(Vector3& pos)
{
	if (gameObjectCamera)
	{
		switch(getCameraBehavior())
		{
			case CBT_FREE:
			case CBT_GOOGLE_EARTH:
			{
				Transform* transform = dynamic_cast<Transform*>(gameObjectCamera->getComponent("Transform"));

				pos.y +=  SurfaceManager::getInstance()->kRealTo3D() * 350;
				transform->setPosition(pos);
				transform->resetOrientation();
				transform->turnX(-45);
			}
			break;
			case CBT_AVATAR:
			{
				pos.y += 0.05 * SurfaceManager::getInstance()->kRealTo3D();
				if (myTargetObject)
				{
					Transform* transform = dynamic_cast<Transform*>(myTargetObject->getComponent("Transform"));
					transform->setPosition(pos);
					transform->resetOrientation();
					transform->turnY(180);
				}
			}
			break;
		}
	}
}

void CameraController::createMiniMapElement()
{
	miniMapElement = new MiniMapElement(name + "_MiniMapElement");

	miniMapElement->setDimensionsProportionalToTheParent(0.05f, 0.05f);

	Transform* transform = dynamic_cast<Transform*>(gameObjectCamera->getComponent("Transform"));
	miniMapElement->update(transform->getPosition());
	miniMapElement->setMaterial("TerraVR/Seta");
}

void CameraController::updateMiniMapElement()
{
	if (miniMapElement)
	{
		float degree = 0.0f;;
		CameraBehaviorTypes t = activeCameraBehavior->getCameraBehaviorType();
		Vector3 pos;

		if (t == CBT_FREE || t == CBT_GOOGLE_EARTH)
		{
			Transform* transform = dynamic_cast<Transform*>(gameObjectCamera->getComponent("Transform"));
			degree = transform->getRotation().getYaw().valueDegrees();
			pos = transform->getPosition();
		}
		else if (t == CBT_ORBIT || t == CBT_AUTOMATIC)
		{
			Transform* transform = dynamic_cast<Transform*>(myTargetObject->getComponent("Transform"));
			degree = transform->getYaw();
			pos = transform->getPosition();
		}
		else if (t == CBT_AVATAR)
		{
			Transform* transform = dynamic_cast<Transform*>(myTargetObject->getComponent("Transform"));
			degree = transform->getYaw();
			pos = transform->getPosition();
		}

		miniMapElement->update(pos, degree);
	}
}