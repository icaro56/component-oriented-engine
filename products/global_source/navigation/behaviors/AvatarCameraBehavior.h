#ifndef AVATARCAMERABEHAVIOR_H
#define AVATARCAMERABEHAVIOR_H

#include "CameraBehavior.h"

class SceneNodeOld;

class AvatarCameraBehavior : public CameraBehavior
{
public:
	AvatarCameraBehavior();
	virtual ~AvatarCameraBehavior();

	virtual void update();

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//método usado para configurar comportamento antes de ser trocado por outro
	virtual void prepareBeforeChangeBehavior();
	//método usado para configurar comportamento após comportamento estar ativo
	virtual void prepareNextChangeBehavior();

	virtual void configurePrerequisites(const QString& name);

	virtual void setTarget(GameObject* t);
	virtual GameObject* getTarget();

private:
	void setYawPitchDist(const float yaw, const float pitch, const Vector3 &dist);

private:

	GameObject *mTargetAvatar;

	float avatarDistanceZ;
    float avatarDistanceY;
    float avatarDistanceX;
    Vector3 distance;
};

#endif