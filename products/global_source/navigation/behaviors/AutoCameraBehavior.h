#ifndef AUTOCAMERABEHAVIOR_H
#define AUTOCAMERABEHAVIOR_H

#include "CameraBehavior.h"

class AutoCameraBehavior : public CameraBehavior
{
public:
	AutoCameraBehavior();
	virtual ~AutoCameraBehavior();

	virtual void update();

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//método usado para configurar comportamento antes de ser trocado por outro
	virtual void prepareBeforeChangeBehavior();
	//método usado para configurar comportamento após comportamento estar ativo
	virtual void prepareNextChangeBehavior();

	virtual void setTarget(GameObject* t);
	virtual GameObject* getTarget();

	virtual void configurePrerequisites(const QString& name);

private:
	/*!
        \brief Método responsável por configurar a rotação no eixo y, eixo x e
        a distância que a camera ficará do seu alvo respectivamente
        \param yaw float rotação no eixo y.
        \param pitch float rotação no eixo x.
        \param dist const Vector3 distância do alvo.
    */
    void setYawPitchDist(const float yaw, const float pitch, const Vector3 &dist);

private:
	GameObject *mTargetParent;

	int axisZ;

};

#endif