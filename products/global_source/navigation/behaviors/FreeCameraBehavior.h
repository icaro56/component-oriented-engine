#ifndef FREECAMERABEHAVIOR_H
#define FREECAMERABEHAVIOR_H

#include "CameraBehavior.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"

class FreeCameraBehavior : public CameraBehavior
{
public:
	FreeCameraBehavior();
	virtual ~FreeCameraBehavior();

	virtual void update();
	virtual void prepareBeforeChangeBehavior();
	virtual void prepareNextChangeBehavior();

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	virtual void setTopSpeed(double speed);

private:
	void saveAttributes();
	void setSpeedCam(double s);
	
	/*!
        \brief Configura a desaceleração da camera
        \param active bool ativado ou não
    */
    void setSlowdown(const bool active);

	/*!
		\brief método para configurar visão topo da camera. Alterna entre visão topo
		ou a posição anterior
	*/
	void swappTop();

private:
	Vector3 mVelocity;

	bool mGoingForward;
	bool mGoingBack;
	bool mGoingLeft;
	bool mGoingRight;
	bool mGoingUp;
	bool mGoingDown;

	bool mSlowdown;
	bool isTop;

	double speedCam;

	PlaceRecovery mPR[9];
	PlaceRecovery placeRecoveryBeforeTop;
	PlaceRecovery placeRecoveryPosFree;
};

#endif