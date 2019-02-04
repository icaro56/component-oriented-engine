#ifndef CONSTANT_FORCE_H
#define CONSTANT_FORCE_H

#include "KernelPrerequisites.h"
#include "../Component.h"

#include "math/Vector3.h"

/*!
	\brief A for�a aplicada constantemente.
	Esta � uma pequena classe de utilit�rio f�sica usada para aplicar uma for�a cont�nua a um objeto.
	Rigidbody.AddForce aplica uma for�a ao Rigidbody apenas para um quadro, portanto, voc� tem que manter
	a chamar a fun��o. ConstantForce por outro lado vai aplicar a for�a cada quadro at� que voc� mude a 
	for�a ou de torque para um novo valor.
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
      * @param e booleano de ativa��o
    */
	virtual void setEnabled(bool e);
	/**
      * @brief Acessa se o Componente esta ativado
    */
	virtual bool isEnabled();
	/**
      * @brief Atribui uma for�a constante ao Collider 
      * @param f valor da for�a
    */
	void setForce(const Vector3& f);
	/**
      * @brief Acessa o valor da for�a exercida
    */
	Vector3 getForce();
	/**
      * @brief Atribui uma for�a relativa ao Collider 
      * @param f valor da for�a
    */
	void setRelativeForce(const Vector3& f);
	/**
      * @brief Acessa o valor da for�a relativa exercida
    */
	Vector3 getRelativeForce();
	/**
      * @brief Atribui torque ao Collider 
      * @param f valor da for�a de torque
    */
	void setTorque(const Vector3& f);
	/**
      * @brief Acessa o valor da for�a de torque
    */
	Vector3 getTorque();
	/**
      * @brief Atribui torque relativo ao Collider 
      * @param f valor da for�a de torque relativo
    */
	void setRelativeTorque(const Vector3& f);
	/**
      * @brief Acessa o valor da for�a de torque relativo
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