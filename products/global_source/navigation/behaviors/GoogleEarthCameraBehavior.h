#ifndef GOOGLEEARTHCAMERABEHAVIOR_H
#define GOOGLEEARTHCAMERABEHAVIOR_H

#include "CameraBehavior.h"

#include "OgreVector2.h"

class SceneNodeOld;

class GoogleEarthCameraBehavior : public CameraBehavior
{
public:
	GoogleEarthCameraBehavior();
	virtual ~GoogleEarthCameraBehavior();

	virtual void update();
	virtual void prepareBeforeChangeBehavior();
	virtual void prepareNextChangeBehavior();

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	virtual void configurePrerequisites(const QString& name);

private:
	/*!

		\brief Método usado para usar a função pan de movimentação da câmera
	*/
	void pan();

	/*!
		\brief Método usado para zoom da camera (sem mexer em fov)
	*/
	void zoom(int mousey);
	void zoomScroll();

	/*!
		\brief Método usado para dar rotate na câmera
	*/
	void rotate();

	void setTargetPos(int x, int y);
	void setTargetPos2(int x, int y);
	void setTargetPos3(int x, int y);

	void detachFromTarget();

	void saveAttributes();

private:
	bool canUsePan;
	bool usedZoomScroll;
	double countZoomScroll;
	GameObject *mTarget;
	GameObject *mTargetParent;

	Ogre::Vector2 posZoomInitial;

	PlaceRecovery placeRecoveryPosFree;
};

#endif