#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "KernelPrerequisites.h"

#include "../Component.h"

#include "math/Vector3.h"
#include "math/Quaternion.h"

#include <map>

namespace Ogre
{
	class SceneNode;
	class MovableObject;
}

enum class Space
{
    SELF = 0,
    PARENT = 1,
    WORLD = 2
};

/*! \class Transform
	\brief Posição, rotação e escala de um objeto.
	\brief Cada objeto em uma cena tem um Transform. 
	\brief É usado para armazenar e manipular a posição,
	\brief rotação e escala do objeto. Cada Transform
	\brief pode ter um pai, o que permite que você 
	\brief aplique posição, rotação e escala hierárquica.
*/

class KERNEL_VS_EXPORT Transform : public Component
{
public:
	typedef std::map<std::string, Transform*> TransformChildren;

	Transform();
	virtual ~Transform();

	bool operator == (const Transform* t) const ;

	virtual void prerequisites() {}
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	/*!
		\brief Atribui a visibilidade
	*/
    virtual void setEnabled(bool enable);
	 /*!
		\brief Verifica a visibilidade 
	*/
    virtual bool isEnabled();

	void setPosition(const Vector3 &pos);
	Vector3 getPosition();

	void _updatePosition(const Vector3 &pos);

	/**********************************************************************************************//**
	 * \fn	void Transform::setPosition(const Vector3 &pos);
	 *
	 * \brief	Sets a position.
	 *
	 * \author	Admin
	 * \date	06/02/2014
	 *
	 * \param	pos	The position.
	 **************************************************************************************************/

	void setLocalPosition(const Vector3 &pos);

	/**********************************************************************************************//**
	 * \fn	Vector3 Transform::getPosition();
	 *
	 * \brief	Gets the position.
	 *
	 * \author	Admin
	 * \date	06/02/2014
	 *
	 * \return	The position.
	 **************************************************************************************************/

	Vector3 getLocalPosition();

	/**********************************************************************************************//**
	 * \fn	void Transform::setOrientation(const Quaternion& quat);
	 *
	 * \brief	Sets an orientation.
	 *
	 * \author	Admin
	 * \date	06/02/2014
	 *
	 * \param	quat	The quaternion.
	 **************************************************************************************************/

	void setLocalRotation(const Quaternion& quat);

	/**********************************************************************************************//**
	 * \fn	Quaternion Transform::getOrientation();
	 *
	 * \brief	Gets the orientation.
	 *
	 * \author	Admin
	 * \date	06/02/2014
	 *
	 * \return	The orientation.
	 **************************************************************************************************/

	Quaternion getLocalRotation();

	/**********************************************************************************************//**
	 * \fn	void Transform::setWorldOrientation(const Quaternion& quat);
	 *
	 * \brief	Sets world orientation.
	 *
	 * \author	Admin
	 * \date	06/02/2014
	 *
	 * \param	quat	The quaternion.
	 **************************************************************************************************/

	void setRotation(const Quaternion& quat);
	void _updateRotation(const Quaternion& quat);

	/**********************************************************************************************//**
	 * \fn	Quaternion Transform::getWorldOrientation();
	 *
	 * \brief	Gets world orientation.
	 *
	 * \author	Admin
	 * \date	06/02/2014
	 *
	 * \return	The world orientation.
	 **************************************************************************************************/

	Quaternion getRotation();

	//rever
	void setLocalScale(const Vector3& s, bool scaleChildren = true);
	Vector3 getLocalScale();
	Vector3 getLossyScale();

	// Gira o nodo em torno do eixo X. 'ts' informa se será considerado
    //o eixo local, do pai ou o global.
    /**
     * @brief Gira o nodo em torno do eixo X.
     * @param ts informa se será considerado o eixo local, do pai ou o
     global.
    */
    void turnX(double degrees, Space ts = Space::SELF);

    // Gira o nodo em torno do eixo Y. 'ts' informa se será considerado
    //o eixo local, do pai ou o global.
    /**
     * @brief Gira o nodo em torno do eixo Y.
     * @param ts informa se será considerado o eixo local, do pai ou o
     global.
    */
    void turnY(double degrees, Space ts = Space::SELF);

    // Gira o nodo em torno do eixo Z. 'ts' informa se será considerado
    //o eixo local, do pai ou o global.
    /**
     * @brief Gira o nodo em torno do eixo Z.
     * @param ts informa se será considerado o eixo local, do pai ou o
     global.
    */
    void turnZ(double degrees, Space ts = Space::SELF);

    // Faz o nodo olhar para um ponto.
    /**
     * @brief Faz o nodo olhar para um ponto.
     * @remarks Usa o metodo lookAt() da classe Ogre::SceneNode
    */
    void lookAt(Transform* target, const Vector3& worldUp = Vector3::UNIT_Y, Space ts = Space::WORLD);
    void lookAt(const Vector3& worldPosition, const Vector3& worldUp = Vector3::UNIT_Y, Space ts = Space::WORLD);

	/*!
		\brief Reseta a orientação.
	*/
    void resetOrientation();

	/*!
		\brief Retorna a rotação atual no eixo X (pitch).
	*/
    float getPitch();

	/*!
		\brief Retorna a rotação atual global no eixo X (pitch).
	*/
    float getWorldPitch();

	/*!
		\brief Retorna a rotação atual no eixo Y (yaw).
	*/
    float getYaw();

	/*!
		\brief Retorna a rotação atual global no eixo Y (yaw).
	*/
    float getWorldYaw();

	/*!
		\brief Retorna a rotação atual no eixo Z (roll).
	*/
    float getRoll();

	/*!
		\brief Retorna a rotação atual global no eixo Z (roll).
	*/
    float getWorldRoll();

	/*!
		\brief Translada o nodo.
		\param tx posição x
		\param ty posiçao y
		\param tz posição z
		\param ts posição no mundo ex: SELF
	*/
    void translate(double tx, double ty, double tz, Space ts = Space::SELF);

	/*!
		\brief Translada o nodo.
		\param d Vector3 de posição
		\param ts posição no mundo ex: SELF
	*/
    void translate(const Vector3 &d, Space ts = Space::SELF);

	void translate(const Vector3& translation, Transform* relativeTo);
	void translate(float x, float y, float z, Transform* relativeTo);

	/*!
		\brief   Atribui uma direção ao nodo.
		\param dir Vector3 de direção
	*/
	void setForward(const Vector3& dir);
	Vector3 getForward();
	Vector3 getRight();
	Vector3 getUp();

	void setInheritOrientation(bool op);
	void setInheritScale(bool op);
	void setFixedYawAxis(bool op);

	// Adiciona um nodo filho ao nodo.
    /**
     * @brief Adiciona um nodo filho ao nodo.
    */
    void attachChild(Transform *node);

    // Desliga este nodo do seu nodo pai e o acopla ao nodo raiz da cena.
    /**
     * @brief Desliga este nodo do seu nodo pai e o acopla ao nodo raiz
     da cena.
    */
    void detach(bool attachToRoot = true);

    // Retorna se este nodo tem um nodo pai (excluindo o nodo raiz da cena).
    /**
     * @brief Retorna se este nodo tem um nodo pai (excluindo o nodo raiz
                                                 da cena).
    */
    bool hasParent();

    // Retorna o nodo pai.
    /**
     * @brief Retorna o nodo pai (excluindo o nodo raiz da cena).
    */
    Transform* getParent();
	/*!
		\brief Seta um Parente do tipo Transform.
		\param n Um componente Transform
	*/
	void setParent(Transform* n);

	int getChildCount();
	void detachChildren();

	void setEulerAngles(Vector3& angle);
	Vector3 getEulerAngles();

	void setLocalEulerAngles(Vector3& angle);
	Vector3 getLocalEulerAngles();

	/*!
	\brief Has the transform changed since the last time the flag was set to 'false'?

	A change to the transform can be anything that can cause its matrix to be recalculated: 
	any adjustment to its position, rotation or scale. Note that operations which can change the transform 
	will not actually check if the old and new value are different before setting this flag. So setting, 
	for instance, transform.position will always set hasChanged on the transform, regardless of there being any 
	actual change.
	*/
	bool hasChanged();
	void restoreHasChanged();

	void iAmRootScene(bool active);

	/*!
		\brief Retorna o transform no alto da hierarquia excluindo o RootTransform da cena. Se nao tem, retorna o chamador
	*/
	Transform* getSubRoot();

	Transform* getRoot();

	Transform* find(const std::string& name);

	Transform* getChild(int index);

	Vector3 inverseTransformDirection(const Vector3& direction);
	Vector3 inverseTransformPoint(const Vector3& position);

	Vector3 transformDirection(const Vector3& direction);
	Vector3 transformPoint(const Vector3& position);

	bool isChildOf(Transform* parent);

	void rotate(const Vector3& eulerAngles, Space relativeTo = Space::SELF);
	void rotate(float xAngle, float yAngle, float zAngle, Space relativeTo = Space::SELF);
	void rotate(const Vector3& axis, float angle, Space relativeTo = Space::SELF);

	void rotateAround(const Vector3& point, const Vector3& axis, float angle);

	TransformChildren& getChildren();

	/*!
		\brief função usada para atachar movables da Ogre. É aconselhavel não usa-la, pois seu uso é interno.
	*/
	void _attachObject(Ogre::MovableObject*  movable);
	
    /*!
     * \brief Retorna o SceneNode. Uso interno
    */
    Ogre::SceneNode* _getOgreSceneNode();
	void _setOgreSceneNode(Ogre::SceneNode* sceneNode);

private:
	Ogre::SceneNode* mSceneNode;
	Transform* mParent;

	TransformChildren children;

	bool changed;
	bool isRootScene;

	bool enabled;
};

#endif