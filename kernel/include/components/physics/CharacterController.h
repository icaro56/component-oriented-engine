#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include "KernelPrerequisites.h"
#include "../Component.h"

#include "characterkinematic/PxController.h"

#include "Collider.h"

namespace physx
{
	class PxController;
}

enum CollisionFlags
{
	None = 0,
	Sides = physx::PxControllerCollisionFlag::eCOLLISION_SIDES,
	Above = physx::PxControllerCollisionFlag::eCOLLISION_UP,
	Below = physx::PxControllerCollisionFlag::eCOLLISION_DOWN
};

class CharacterController;

class KERNEL_VS_EXPORT ControllerColliderHit
{
	friend class CharacterControllerCallback;
public:
	ControllerColliderHit();
	~ControllerColliderHit();

	//The collider that was hit by the controller.
	Collider* getCollider();

	//The controller that hit the collider.
	CharacterController* getController();

	//The game object that was hit by the controller.
	GameObject* getGameObject();
	Vector3 getMoveDirection();
	float getMoveLength();
	Vector3 getNormal();
	Vector3 getPoint();

	//The rigidbody that was hit by the controller.
	Rigidbody* getRigidbody();

	//The transform that was hit by the controller.
	Transform* getTransform();

private:
	void setGameObject(GameObject* g);
	void setMoveDirection(const Vector3& d);
	void setMoveLength(float l);
	void setNormal(const Vector3& d);
	void setPoint(const Vector3& d);
	void setController(CharacterController* c);

private:
	Collider* collider;
	CharacterController* controller;
	GameObject* gameObject;
	Vector3 moveDirection;
	float moveLength;
	Vector3 normal;
	Vector3 point;
	Rigidbody* rigidbody;
	Transform* transform;
};

class KERNEL_VS_EXPORT ControllerControllerHit
{
	friend class CharacterControllerCallback;
public:
	ControllerControllerHit();
	~ControllerControllerHit();

	//The controller that hit the collider.
	CharacterController* getController();

	Vector3 getMoveDirection();
	float getMoveLength();
	Vector3 getNormal();
	Vector3 getPoint();

	CharacterController* getOtherController();

private:
	void setMoveDirection(const Vector3& d);
	void setMoveLength(float l);
	void setNormal(const Vector3& d);
	void setPoint(const Vector3& d);

	void setController(CharacterController* c);
	void setOtherController(CharacterController* c);

private:

	CharacterController* controller;
	CharacterController* otherController;

	Vector3 moveDirection;
	float moveLength;
	Vector3 normal;
	Vector3 point;
};

class KERNEL_VS_EXPORT CharacterController : public Collider
{
	Q_OBJECT

	friend class CharacterControllerCallback;
public:
	enum CharacterControllerDesc
	{
		CCD_Center,
		CCD_Height,
		CCD_Radius,
		CCD_SlopeLimit,
		CCD_StepOffset,
		
		CCD_Size
	};

	 /**
      * @brief Construtor Default
    */
	CharacterController();

	/**
      * @brief Construtor 
      * @param idComponent Nome do Componente
     */
	CharacterController(std::string idComponent);

	/**
      * @brief Construtor 
      * @param &d Descritor da classe
     */
	CharacterController(const ComponentDescMap &d);

	/**
      * @brief Destrutor Default 
    */
	~CharacterController();

	//reimplementando para sobrescrever implementação do pai.
	virtual void prerequisites();

	/**
      * @brief Inicializa o Component 
      * @param name Nome do GameObject
     */
	virtual void init(const std::string& name);
	virtual void nextInit();

	/**
      * @brief Ativa ou desativa o Componente 
      * @param e booleano de ativação
    */
	virtual void setEnabled(bool e);

	/**
      * @brief Acessa se o Componente esta ativado
    */
	virtual bool isEnabled();

	void setCenter(const Vector3& c);
	Vector3 getCenter();

	CollisionFlags move(const Vector3& motion, bool useCenterPosition = true);

	void setHeight(float h);
	float getHeight();

	void setRadius(float r);
	float getRadius();

	bool isGrounded();

	void setSlopeLimit(float grau);
	float getSlopeLimit();

	void setStepOffset(float offset);
	float getStepOffset();

	Vector3 getVelocity();

	/*!
		\brief Com quais grupos os eventos de colisão serão reportados.
	*/
	virtual void setReportCollision(bool filterMask);

	/*!
		\brief Configura o layer do collider.
	*/
	virtual void setLayerId(unsigned layer);

	const physx::PxFilterData* getFilterData();

	virtual void setScale(const Vector3& s);

	void setRotation(const Quaternion& q);
	Quaternion getRotation();

	void setPosition(const Vector3& p);
	Vector3 getPosition();

	void setFootPosition(const Vector3& p);
	Vector3 getFootPosition();

signals:
	void onControllerColliderHit(ControllerColliderHit& hit);
	void onControllerControllerHit(ControllerControllerHit& hit);

private:
	void getControllerShape();

	virtual void applyDesc(int key, const std::string& value);

	void informOnControllerColliderHit(ControllerColliderHit& hit);
	void informOnControllerControllerHit(ControllerControllerHit& hit);

private:
	physx::PxController* controller;

	ComponentDescMap mCharacterControllerDescMap;

	CollisionFlags collisionFlags;

	bool mIsGrounded;
	Vector3 mVelocity;
	float minMoveDistance;

	//repassando o filter do shape.
	physx::PxFilterData* mFilterData;		    // User-defined filter data for 'move' function
	physx::PxQueryFilterCallback* mFilterCallback;	// User-defined filter data for 'move' function
	physx::PxControllerFilterCallback* mCCTFilterCallback;	// User-defined filter data for 'move' function
};

#endif