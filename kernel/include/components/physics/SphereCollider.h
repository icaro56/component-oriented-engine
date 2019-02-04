#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "KernelPrerequisites.h"
#include "Collider.h"
/*!
		\brief Um colisor primitivo em forma de esfera.
	*/
class KERNEL_VS_EXPORT SphereCollider : public Collider
{
public:
	enum SphereColliderDesc
	{
		SCD_Radius,
		SCD_Center,
		SCD_Size
	};

	/**
      * @brief Construtor Default
    */
	SphereCollider();
	/**
      * @brief Construtor 
      * @param &d Descritor da classe
     */
	SphereCollider(const ComponentDescMap &d);
	/**
      * @brief Destrutor Default 
    */
	virtual ~SphereCollider();
	/**
      * @brief Inicializa o Component 
      * @param name Nome do GameObject
     */
	virtual void init(const std::string& name);
	/**
      * @brief O raio da SphereCollider, medido no espaço local do objecto.
      * @param r raio.
     */
	void setRadius(float r);
	/**
      * @brief Acessa o raio da SphereCollider.
    */
	float getRadius();
	/**
      * @brief Atribui o Centro ao SphereCollider 
      * @param pos Vetor da posição desejada
     */
	void setCenter(const Vector3& pos);
	/**
      * @brief Atribui a escala do SphereCollider 
      * @param s Vetor da escala desejada
     */

	virtual void setScale(const Vector3& s);

private:
	virtual void applyDesc(int key, const std::string& value);
	
private:
	ComponentDescMap mSphereColliderDescMap;
};

#endif