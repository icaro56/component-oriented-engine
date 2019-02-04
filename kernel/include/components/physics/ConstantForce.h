#ifndef CONSTANT_FORCE_H
#define CONSTANT_FORCE_H

#include "KernelPrerequisites.h"
#include "../Component.h"

#include "math/Vector3.h"

/*!
	\brief A força aplicada constantemente.
	Esta é uma pequena classe de utilitário física usada para aplicar uma força contínua a um objeto.
	Rigidbody.AddForce aplica uma força ao Rigidbody apenas para um quadro, portanto, você tem que manter
	a chamar a função. ConstantForce por outro lado vai aplicar a força cada quadro até que você mude a 
	força ou de torque para um novo valor.
*/
class KERNEL_VS_EXPORT ConstantForce : public Component
{
public:
	/**
      * @brief Construtor Default
    */
	ConstantForce();
	/**
      * @brief Construtor 
      * @param idComponent Nome do Componente
     */
	ConstantForce(std::string idComponent);
	/**
      * @brief Destrutor Default 
    */
	~ConstantForce();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit();
	virtual void fixedUpdate();
	/**
      * @brief Ativa ou desativa o Componente 
      * @param e booleano de ativação
    */
	virtual void setEnabled(bool e);
	/**
      * @brief Acessa se o Componente esta ativado
    */
	virtual bool isEnabled();
	/**
      * @brief Atribui uma força constante ao Collider 
      * @param f valor da força
    */
	void setForce(const Vector3& f);
	/**
      * @brief Acessa o valor da força exercida
    */
	Vector3 getForce();
	/**
      * @brief Atribui uma força relativa ao Collider 
      * @param f valor da força
    */
	void setRelativeForce(const Vector3& f);
	/**
      * @brief Acessa o valor da força relativa exercida
    */
	Vector3 getRelativeForce();
	/**
      * @brief Atribui torque ao Collider 
      * @param f valor da força de torque
    */
	void setTorque(const Vector3& f);
	/**
      * @brief Acessa o valor da força de torque
    */
	Vector3 getTorque();
	/**
      * @brief Atribui torque relativo ao Collider 
      * @param f valor da força de torque relativo
    */
	void setRelativeTorque(const Vector3& f);
	/**
      * @brief Acessa o valor da força de torque relativo
    */
	Vector3 getRelativeTorque();

private:
	void defaultValues();

private:
	bool enabled;

	Vector3 force;
	Vector3 relativeForce;
	Vector3 torque;
	Vector3 relativeTorque;
};

#endif