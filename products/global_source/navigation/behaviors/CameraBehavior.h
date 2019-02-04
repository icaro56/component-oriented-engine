#ifndef CAMERABEHAVIOR_H
#define CAMERABEHAVIOR_H

#include "navigation/CameraBasics.h"
#include <input/InputManager.h>

class Camera;
class SceneNodeOld;
class GameObject;

class CameraBehavior
{
public:
	CameraBehavior();
	virtual ~CameraBehavior();

	virtual void update();

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//método usado para configurar comportamento antes de ser trocado por outro
	virtual void prepareBeforeChangeBehavior();
	//método usado para configurar comportamento após comportamento estar ativo
	virtual void prepareNextChangeBehavior();

	//void setCamera(Camera* cam);
	//Camera* getCamera();
	
	void setCamera(GameObject* cam);
	GameObject* getCamera();

	void setInput(InputManager* i);
	InputManager* getInput();

	CameraBehaviorTypes getCameraBehaviorType();

	virtual void setTopSpeed(double speed);
	double getTopSpeed();

	void setFastMove(bool active);
	bool isFastMove();

	void setCollisionEnabled(bool active);
	bool isCollisionEnabled();

	virtual void setTarget(GameObject* t);
	virtual GameObject* getTarget();

	virtual void configurePrerequisites(const QString& name);

protected:
	void setCameraBehaviorType(CameraBehaviorTypes cbt);
	
	CameraBehaviorTypes cameraBehaviorType;
	double topSpeed;

	bool mFastMove;
	bool collisionEnabled;

	//ponteiros repassados pelo camera controller
	//Camera* pCamera;
	GameObject* cameraObject;

	InputManager* pInput;
};

#endif