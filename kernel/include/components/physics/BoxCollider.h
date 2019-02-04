#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "KernelPrerequisites.h"
#include "Collider.h"

	/*!
		\brief Um colisor primitivo em forma de caixa.
	*/
class KERNEL_VS_EXPORT BoxCollider : public Collider
{
public:
	enum BoxColliderDesc
	{
		BCD_SizeDim,
		BCD_Center,
		BCD_Size
	};
	
	/**
      * @brief Construtor Default
    */
	BoxCollider();
	
	/**
      * @brief Construtor 
      * @param &d Descritor da classe
     */
	BoxCollider(const ComponentDescMap &d);
	
	/**
      * @brief Destrutor Default 
    */
	virtual ~BoxCollider();
	
	/**
      * @brief Inicializa o Component 
      * @param name Nome do GameObject
     */
	virtual void init(const std::string& name);
	
	/**
      * @brief Atribui o Centro ao BoxCollider 
      * @param posCenter Vetor da posição desejada
     */
	void setCenter(const Vector3& posCenter);
	
	/**
      * @brief Atribui um valor ao tamanho do BoxCollider 
      * @param dim dimensão a ser acrescida
     */
	void setSize(const Vector3& dim);
	
	/**
      * @brief Acessa o tamanho do BoxCollider
    */
	Vector3 getSize();
	
	/**
      * @brief Atribui a escala do BoxCollider 
      * @param s Vetor da escala desejada
     */
	virtual void setScale(const Vector3& s);

private:
	
	virtual void applyDesc(int key, const std::string& value);
	
private:
	ComponentDescMap mBoxColliderDescMap;
};

#endif