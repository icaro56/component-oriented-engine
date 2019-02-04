#ifndef CAMERACONTROLLERMANAGER_H
#define CAMERACONTROLLERMANAGER_H

#include <QMap>
#include <QString>


#include <engine/RenderView.h>
#include "CameraBasics.h"

class CameraController;
class InputManager;

class CameraControllerManager
{
public:
	static CameraControllerManager* getInstance();

	void init();
	void destroy();

	void createCameraController(const QString& name, 
								const QList<CameraBehaviorTypes> &cbtList, 
								InputManager* input,
								RenderView* render,
								int flags);

	void destroyCameraController(const QString& name);

	void destroyAllCameraController();

	void setMainCameraController(const QString& name);
	CameraController* getMainCameraController();

	CameraController* getCameraController(const QString& name);

	void updateCameraControllers();

	void setDefaultTopSpeed(double speed, bool updateSpeedControllers = false);
	double getDefaultTopSpeed();

	void connectsToMainCameraController();

private:
	CameraControllerManager();
	~CameraControllerManager();

private:
	static CameraControllerManager* instance;

	QMap<QString, CameraController*> cameraControllers;
	CameraController* mainCameraController;

	double defaultTopSpeed;
};


#endif