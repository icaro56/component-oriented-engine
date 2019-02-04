#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QMap>
#include <QObject>
#include "CameraBasics.h"

#include <input/MouseEvents.h>
#include <input/KeyEvents.h>

#include <engine/RenderView.h>

class Camera;
class CameraBehavior;
class InputManager;
class SceneNodeOld;
class MiniMapElement;

class Camera;
class GameObject;

class CameraController : public QObject, public KeyEvents, public MouseEvents
{
	Q_OBJECT

public:
	CameraController(const QString& n, InputManager* i, RenderView* render, int flags);
	virtual ~CameraController();

	void update();

	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void createCameraBehavior(CameraBehaviorTypes cbt);
	void destroyCameraBehavior(CameraBehaviorTypes cbt);
	void destroyAllCameraBehavior();
	
	void setCameraBehavior(CameraBehaviorTypes cbt);
	CameraBehaviorTypes getCameraBehavior();

	void configureMainCamera();

	void setDefaultTopSpeed(double speed, bool updateBehaviors = false);
	double getDefaultTopSpeed();

	//void setTarget(SceneNode* target);
	void setTarget(GameObject* target);

	//Camera* getCamera();
	GameObject* getCamera();

	void createMiniMapElement();

public slots:
	void setWorldPositionToMap(Vector3& pos);
	void updateMiniMapElement();

signals:
	void cameraMoved();

private:
	QString convertCameraBehaviorTypeToString(CameraBehaviorTypes cbt);

private:
	//Camera* camera;
	GameObject* gameObjectCamera;

	QMap<CameraBehaviorTypes, CameraBehavior*> cameraBehaviors;
	CameraBehavior* activeCameraBehavior;

	InputManager* pInput;
	RenderView* pRenderView;
	QString name;

	Vector3 oldPos;
	Quaternion oldOri;

	double defaultTopSpeed;
	bool mFastMove;

	GameObject* myTargetObject;

	MiniMapElement* miniMapElement;
};

#endif