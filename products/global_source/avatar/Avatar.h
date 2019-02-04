#ifndef AVATAR_H
#define AVATAR_H

#include "AvatarBasics.h"

#include <QString>
#include <OgreVector2.h>
#include <OgreVector3.h>

#include <input/MouseEvents.h>
#include <input/KeyEvents.h>

class GameObject;
class CharacterController;

namespace Ogre
{
	class BillboardSet;
	class Billboard;
	class Light;
}

/*! \class Avatar
    \brief Classe Avatar responsável pelo representação gráfica de
    humanoíde na Mina.

    \author Ícaro - 
    \version 3.0
    \date 28/08/2013

*/
class Avatar : public KeyEvents, public MouseEvents
{
public:
    /*!
        \brief Contrutor da Classe

        \param name std::string& Nome do avatar.
        \warning o name tem que ser único para cada avatar.
    */
    Avatar(const QString& name);

    /*!
       \brief Destrutor da Classe
            */
    virtual ~Avatar();

	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);

	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    /*!
       \brief Atribui um novo estado de animação para o avatar.
       \param state AvatarState novo estado de animação.
       \sa Avatar::AvatarState
            */
    void setAvatarState(AvatarStates state);

    /*!
        \brief Retorna o estado de animação do avatar
        \return AvatarStates estado da animação
        \sa setAvatarState
    */
    AvatarStates getAvatarStates() const;

    /*!
        \brief Atribui uma nova velocidade de andar para o avatar.
        \param speed float valor da nova velocidade.
    */
    void setWalkSpeed(const float speed);

    /*!
        \brief Atribui uma nova velocidade de correr para o avatar.
        \param speed float valor da nova velocidade.
    */
    void setRunSpeed(const float speed);

    /*!
        \brief Retorna velocidade de andar do avatar.
        \return float valor da velocidade.
        \sa setWalkSpeed
    */
    float getWalkSpeed() const;

    /*!
        \brief Retorna velocidade de correr do avatar.
        \return float valor da velocidade.
        \sa setRunSpeed
    */
    float getRunSpeed() const;

    /*!
        \brief Retorna o ponteiro para entidade do avatar.
        \return EntityOld* ponteiro da entidade.
        \sa EntityOld
    */
    GameObject* getGameObject() const;


    /*!
        \brief Faz com que o avatar se movimente no mundo 3D
        \param axis Vector2* contém os eixos que o avatar vai andar.<br>
        <table cellspacing="2" cellpadding="2" border="1" align="center">
        <tr><td>axis.x:</td><td>1 vai para frente;</td><td>-1 vai
        para trás;</td><td>0 não anda nesse eixo;</td></tr>
        <tr><td>axis.y:</td><td>1 vai para direita;</td><td>-1
        vai para esquerda;</td><td>0 não anda nesse eixo;</td></tr>
        </table><br>
        \param delta float tempo desde o último frame renderizado.
        \sa Vector2
        \param terrain MineTerrain* usado para cálculo de posicionamento.
    */
    void move();

	/*!
        \brief Para a animação do Avatar
    */
    void stop();

	/*!
		\brief Troca o estado da lanterna do avatar
	*/
	void swappActivitiyFlashLight();

	/*!
		\brief Configura se a lanterna está ligada ou não
	*/
	void setActivitiyFlashLight(bool active);

	bool isActiveFlashLight();

	void activeParticle(bool a);

	void changeAvatar(bool isMale);

	bool isChangingAvatar();

	void setActive(bool active);
	bool isActived() {return isActive;}

	void teleportAvatar();
	bool isTeleporting()const {return teleport;}

private:

	void removeAvatar();
	void constructAvatar(bool isMale);

	bool hasGround();

	void updateLightPosition();

private:
	GameObject* gameObject;
	GameObject* lightObject;

	CharacterController* controller;
	float jumpSpeed;
	bool jumpFires;
	float gravity;
	Vector3 moveDirection;

    AvatarStates mState;
    float mWalkSpeed;
    float mRunSpeed;
	float delayTeleport;

	/// Conjunto de Billboards
    /*Ogre::BillboardSet* billboardSet;
    Ogre::BillboardSet* billboardSet2;
    /// Billboard
    Ogre::Billboard* billboard;
    Ogre::Billboard* billboard2;*/

	bool isAvatarMale;
	bool isChangeAvatar;
	bool lastVisibilityLight;
	bool isActive;
	bool teleport;

	std::string myName;

	Vector3 lastValidPosition;
	Ogre::Vector2 axis;
	Vector3 posTeleport;
};


#endif