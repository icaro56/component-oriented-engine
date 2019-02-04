#ifndef CAPSULECOLLIDER_H
#define CAPSULECOLLIDER_H

#include "KernelPrerequisites.h"
#include "Collider.h"

/*!
	\brief Um colisor primitivo em forma de Capsula.
*/
class KERNEL_VS_EXPORT CapsuleCollider : public Collider
{
public:
	enum CapsuleDir
	{
		X_Axis,
		Y_Axis,
		Z_Axis
	};

	enum CapsuleColliderDesc
	{
		CCD_Radius,
		CCD_Center,
		CCD_Height,
		CCD_Size
	};

	/**
      * @brief Construtor Default
    */
	CapsuleCollider();
	/**
      * @brief Construtor 
      * @param &d Descritor da classe
     */
	CapsuleCollider(const ComponentDescMap &d);
	/**
      * @brief Destrutor Default 
    */
	virtual ~CapsuleCollider();
	/**
      * @brief Inicializa o Component 
      * @param name Nome do GameObject
     */

	virtual void init(const std::string& name);
	/**
      * @brief Atribui um valor a direção do CapsuleCollider 
      * @param d dimensão a ser acrescida
     */
	void setDirection(CapsuleDir d);
	/**
      * @brief Atribui um valor ao tamanho do CapsuleCollider 
      * @param h dimensão a ser acrescida
     */
	void setHeight(float h);
	/**
      * @brief Acessa o tamanho do CapsuleCollider
    */
	float getHeight();
	/**
      * @brief Atribui um valor ao tamanho a largura do CapsuleCollider 
      * @param r dimensão a ser acrescida
     */
	void setRadius(float r);
	/**
      * @brief Acessa o tamanho da largura do CapsuleCollider
    */
	float getRadius();
	/**
      * @brief Atribui o Centro ao CapsuleCollider 
      * @param posCenter Vetor da posição desejada
     */
	void setCenter(const Vector3& pos);
	/**
      * @brief Atribui um valor de escala ao  CapsuleCollider 
      * @param s dimensão a ser acrescida
     */
	virtual void setScale(const Vector3& s);

private:

	virtual void applyDesc(int key, const std::string& value);
	

private:
	ComponentDescMap mCapsuleColliderDescMap;

	CapsuleDir myDir;
};

#endif