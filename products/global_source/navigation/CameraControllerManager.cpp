#include "CameraControllerManager.h"
#include "CameraController.h"

#include <engine/Engine.h>
#include <engine/RenderView.h>
#include <input/InputManager.h>

#include <QObject>

#include "gizmo/GizmoControl.h"
#include "view/compass/CompassControl.h"
#include "view/statusbar/StatusBarControl.h"
#include "components/rendering/Camera.h"

CameraControllerManager* CameraControllerManager::instance = 0;


CameraControllerManager* CameraControllerManager::getInstance()
{
	if (!instance)
	{
		instance = new CameraControllerManager();
	}

	return instance;
}

void CameraControllerManager::init()
{
	RenderView* mRender = Input->getRenderView();
	QList<CameraBehaviorTypes> listCBT;
	listCBT << CBT_FREE << CBT_ORBIT << CBT_AVATAR << CBT_AUTOMATIC << CBT_GOOGLE_EARTH;
	createCameraController("AllCamera", 
							listCBT, 
							Input,
							mRender,
							CullingMask(VF_DEFAULT | VF_RENDER1));

	setMainCameraController("AllCamera");
	connectsToMainCameraController();
	getMainCameraController()->setCameraBehavior(CBT_FREE);
}

void CameraControllerManager::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

CameraControllerManager::CameraControllerManager()
	: mainCameraController(0),
	  defaultTopSpeed(0)
{

}

CameraControllerManager::~CameraControllerManager()
{
	if (mainCameraController)
	{
		mainCameraController = 0;
	}

	destroyAllCameraController();
}

void CameraControllerManager::createCameraController(const QString& name, 
													 const QList<CameraBehaviorTypes> &cbtList, 
													 InputManager* input,
													 RenderView* render,
													 int flags)
{
	if (!cameraControllers.contains(name))
	{
		CameraController* cameraController = new CameraController(name, input, render, flags);
		cameraController->setDefaultTopSpeed(defaultTopSpeed);

		for (int i = 0; i < cbtList.size(); ++i)
		{
			cameraController->createCameraBehavior(cbtList.at(i));
		}

		cameraControllers.insert(name, cameraController);
	}
	else
	{
		std::cerr << "\nNão é possível criar um controlador de camera com um nome já usado";
	}
}

void CameraControllerManager::destroyCameraController(const QString& name)
{
	if (cameraControllers.contains(name))
	{
		CameraController* temp = cameraControllers.value(name);
		delete temp;
		temp = 0;

		cameraControllers.remove(name);
	}
}

void CameraControllerManager::destroyAllCameraController()
{
	if (!cameraControllers.empty())
	{
		QMapIterator<QString, CameraController*> i(cameraControllers);
        while (i.hasNext())
        {
            i.next();

            CameraController* temp = i.value();
            delete temp;
            temp = 0;
        }

        cameraControllers.clear();
	}
}

void CameraControllerManager::setMainCameraController(const QString& name)
{
	if (cameraControllers.contains(name))
	{
		mainCameraController = cameraControllers.value(name);
	}
}

CameraController* CameraControllerManager::getMainCameraController()
{
	return mainCameraController;
}

CameraController* CameraControllerManager::getCameraController(const QString& name)
{
	return cameraControllers.value(name);
}

void CameraControllerManager::updateCameraControllers()
{
	QMapIterator<QString, CameraController*> i(cameraControllers);
    while (i.hasNext())
    {
        i.next();

        CameraController* temp = i.value();
		temp->update();
    }
}

void CameraControllerManager::setDefaultTopSpeed(double speed, bool updateSpeedControllers)
{
	defaultTopSpeed = speed;

	if (updateSpeedControllers)
	{
		QMapIterator<QString, CameraController*> i(cameraControllers);
		while (i.hasNext())
		{
			i.next();

			CameraController* temp = i.value();
			temp->setDefaultTopSpeed(defaultTopSpeed, updateSpeedControllers);
		}
	}
}

double CameraControllerManager::getDefaultTopSpeed()
{
	return defaultTopSpeed;
}

void CameraControllerManager::connectsToMainCameraController()
{
	if (mainCameraController)
	{
		QObject::connect(mainCameraController,
						 SIGNAL(cameraMoved()),
						 GizmoControl::getInstance(),
						 SLOT(calculateScale()));
		
		QObject::connect(mainCameraController,
						 SIGNAL(cameraMoved()),
						 CompassControl::getInstance(),
						 SLOT(calculateScale()));
	}
}