#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "KernelPrerequisites.h"
#include "../Component.h"

#include "math/Vector3.h"
#include "engine/buffers/RaycastHit.h"

namespace physx
{
	class PxRigidDynamic;
	class PxShape;
	class PxTransform;
	struct PxSweepHit;
}

/*!
	\brief Use this to set up a Rigidbody's for continuous collision detection, which is used to prevent fast 
	moving objects from passing through other objects without detecting collisions. 
	For best results, set this value to CollisionDetectionMode.ContinuousDynamic for fast moving objects, 
	and for other objects which these need to collide with, set it to CollisionDetectionMode.Continuous. 
	This has a big impact on physics performance, so just leave it set to the default value of 
	CollisionDetectionMode.Discrete, if you don't have any issues with collisions of fast objects. 
	Continuous Collision Detection is only supported for Rigidbodies with Sphere-, Capusle- or BoxColliders.

	\warning Passar c�digo para um cabe�alho apenas com struturas e enumerators
*/

enum class CollisionDetectionMode
{
	Discrete,
	Continuous,
	ContinuousDynamic
};

enum class ForceMode
{
	Force,
	Acceleration,
	Impulse,
	VelocityChange
};


/**
      * @brief Controle de posi��o de um objeto por meio de simula��o de f�sica.
		Componentes Rigidbody assumir o controle sobre a posi��o de um objeto -
		que faz com que os objetos caem sob a influ�ncia da gravidade, e pode 
		calcular como os objetos v�o responder a colis�es.
		Ao manipular par�metros rigidbody voc� deve trabalhar dentro da fun��o 
		FixedUpdate . Simula��o f�sica � realizada em passos_de_tempo discretas.
		A fun��o FixedUpdate � chamada imediatamente antes de cada etapa.
		Coisas que atente para quando se usa corpos r�gidos:
		1. Se a sua simula��o parece c�mera lenta e n�o s�lida : 
		Este � um problema de escala. Quando o mundo do jogo � enorme tudo parecer�
		estar se movem muito lentamente. Certifique-se de todos os seus modelos 
		s�o em tamanhos reais. Por exemplo, um carro deve ser de cerca de 4 metros
		de comprimento. Um personagem em torno de 2 metros de altura. Um objeto cai 
		na mesma acelera��o, n�o importa grande ou pequeno, pesado ou leve. Se o seu
		mundo de jogo tem uma grande escala, os objetos ainda vai cair na mesma 
		acelera��o. Mas uma vez que tudo � maior, os objetos aparecer�o como eles caem mais lento.
 */
class KERNEL_VS_EXPORT Rigidbody : public Component
{
public:

	enum RigibodyDesc
	{
		RD_AngularDrag,
		RD_AngularVelocity,
		RD_CenterOfMass,
		RD_CollisionDetectionMode,
		RD_InertiaTensor,
		RD_DetectCollisions,
		RD_Drag,
		RD_IsKinematic,
		RD_Mass,
		RD_MaxAngularVelocity,
		RD_Position,
		RD_Rotation,
		RD_SleepVelocity,
		RD_SolverIterationCount,
		RD_UseGravity,
		RD_Velocity,
		RD_Density,
		RD_Size
	};
     
	/**
      * @brief Construtor Default
    */
	Rigidbody();
	
	/**
      * @brief Construtor 
      * @param idComponent Nome do Componente
     */
	Rigidbody(std::string idComponent);
	
	/**
      * @brief Construtor 
      * @param &d Descritor da classe
     */
	Rigidbody(const ComponentDescMap &d);
	
	/**
      * @brief Destrutor Default 
    */
	~Rigidbody();

	virtual void prerequisites();
	
	/**
      * @brief Inicializa o Component 
      * @param name Nome do GameObject
     */
	virtual void init(const std::string& name);
	virtual void nextInit();
	virtual void update();
	
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
      * @brief Recebe um shape e aloca-o no Componente 
      * @param s Shape F�sico
    */
	void attachShape(physx::PxShape* s);
	
	physx::PxRigidDynamic* getActor();
	
	/**
      * @brief Seta o valor de arrasto angular , Quanto mais elevada for a resist�ncia mais a rota��o diminui.
      * @param a valor de arrasto
    */
	void setAngularDrag(float a);
	
	/**
      * @brief Acessa se o valor de arrasto angular
    */
	float getAngularDrag();
	
	/**
      * @brief Seta o valor de velocidade angular , Na maioria dos casos voc� n�o deve modific�-lo diretamente, pois isso pode resultar em um comportamento irrealista.
      * @param angVec vetor de arrasto
    */
	void setAngularVelocity(const Vector3& angVec);
	
	/**
      * @brief Acessa se o valor de velocidade angular
    */
	Vector3 getAngularVelocity();
	
	/**
      * @brief Atribui o Centro da massa do GameObject 
      * @param center Vetor da posi��o desejada
     */
	void setCenterOfMass(const Vector3& center);
	
	/**
      * @brief Acessa a posi��o do centro da massa
    */
	Vector3 getCenterOfMass();
	
	/**
      * @brief Seta o tipo de detec��o de colis�o que o Rigidbody executara
      * @param mode tipo de detec��o de colis�o
     */
	void setCollisionDetectionMode(CollisionDetectionMode mode);
	CollisionDetectionMode getCollisionDetectionMode();

	//physx version used by unity is 2.8: https://devtalk.nvidia.com/default/topic/538594/physx-and-physics-modeling/missing-features-in-3-series-compared-to-2-/
	//void setConstraints();

	/**
      * @brief O tensor de in�rcia de massa na diagonal em rela��o ao centro de massa.
      * @param i ponto que exercer� a in�rcia
     */
	void setInertiaTensor(const Vector3& i);
	
	/**
      * @brief Acessa o ponto de tens�o da in�rcia
    */
	Vector3 getInertiaTensor();
	
	/**
      * @brief Ativa se haver� detec��o de colis�o
      * @param enable booleano de ativa��o
     */
	void setDetectCollision(bool enable);
	
	/**
      * @brief Acessa se a colis�o ativada
    */
	bool getDetectCollision();
	
	/**
      * @brief Atribui uma resist�ncia de arrasto ao objeto
      * @param d valor de resist�ncia
     */
	void setDrag(float d);
	
	/**
      * @brief Acessa o valor de resist�ncia de arrasto do objeto
    */
	float getDrag();

	//veja setConstraints
	//void setFreezeRotation(bool enable);

	/**
      * @brief Controla se a f�sica afetar� o Rigidbody
      * @param active booleano de ativa��o
     */
	void setKinematic(bool active);
	
	/**
      * @brief Acessa se a f�sica afetar� o Rigidbody
    */
	bool isKinematic();
	
	/**
      * @brief Atribui um valor de massa ao objeto
      * @param m valor de massa
     */
	void setMass(float m);
	
	/**
      * @brief Acessa a massa do objeto
    */
	float getMass();
	
	/**
      * @brief Atribui um valor maximo de velocidade angular que um objeto possa chegar
      * @param m valor maximo
     */
	void setMaxAngularVelocity(float m);
	
	/**
      * @brief Acessa o valor maximo de velocidade angular
    */
	float getMaxAngularVelocity();

	/*!
	\fn	void Rigidbody::setPosition(const Vector3& pos);
	\brief	Sets a position.
	\param	pos	The position.
	\warning This is similar to setting transform.position, however the position will only be applied to the 
	transform at the end of the physics step. If you want to continously move a rigidbody or kinematic rigidbody use 
	MovePosition and MoveRotation instead
	*/
	void setPosition(const Vector3& pos);
	
	/**
      * @brief Acessa o ponto de posi��o do objeto na cena
    */
	Vector3 getPosition();
	
	/**
      * @brief Atribui um Quaterniom para a rota��o do objeto
      * @param q Quaterniom de rota��o
     */
	void setRotation(const Quaternion& q);
	
	/**
      * @brief Acessa o Quaternion de rota�ao do objeto
    */
	Quaternion getRotation();
	
	/**
      * @brief Atribui um valor de descan�o de velocidade
      * @param v valor de descan�o
     */
	void setSleepVelocity(float v);
	
	/**
      * @brief Acessa o valor de descan�o
    */
	float getSleepVelocity();
	
	/**
      * @brief Permite substituir a contagem de solver itera��o por rigidbody.
      * @param s quantidade de itera��o ao Rigidbody
     */
	void setSolverIterationCount(int s);
	
	/**
      * @brief Acessa o valor de quantidade de itera��o ao Rigidbody
    */
	int getSolverIterationCount();
	
	/**
      * @brief Ativa gravidade
      * @param enable Booleano de ativa��o
     */
	void useGravity(bool enable);
	
	/**
      * @brief Se a gravidade est� ativada
    */
	bool isUseGravity();
	
	/**
      * @brief Atribui um valor para a velocidade
      * @param vel velocidade
     */
	void setVelocity(const Vector3& vel);
	
	/**
      * @brief Acessa o valor de velocidade
    */
	Vector3 getVelocity();

	physx::PxTransform getWorldCenterOfMass();
	
	/**
      * @brief Adiciona uma for�a ao Rigidbody
      * @param force dire��o da for�a
      * @param mode tipo de for�a
     */
	void addForce(const Vector3& force, ForceMode mode = ForceMode::Force);
	
	/**
      * @brief Adiciona uma for�a ao Rigidbody para determinada posi��o
      * @param force dire��o da for�a
      * @param position pois��o de destino
      * @param mode tipo de for�a
     */
	void addForceAtPosition(const Vector3& force, const Vector3& position, ForceMode mode = ForceMode::Force);
	
	/**
      * @brief Adiciona uma for�a para o rigidbody em rela��o ao sistema de coordenadas.
      * @param force dire��o da for�a
      * @param mode tipo de for�a
     */
	void addRelativeForce(const Vector3& force, ForceMode mode = ForceMode::Force);
	
	//rever
	/**
      * @brief Adiciona um torque ao rigidbody em rela��o ao sistema de coordenadas do pr�prio rigidbodie.
      * @param torque torque relativo
      * @param mode tipo de for�a
     */
	void addRelativeTorque(const Vector3& torque, ForceMode mode = ForceMode::Force);
	
	/**
      * @brief Adiciona um torque ao rigidbody.
      * @param torque valor do torque
      * @param mode tipo de for�a
     */
	void addTorque(const Vector3& torque, ForceMode mode = ForceMode::Force);

	//rever
	/**
      * @brief O ponto mais pr�ximo � caixa delimitadora dos aceleradores em anexo.
      * @param position posi��o em rela��o ao Rigidbody
     */
	Vector3 closestPointOnBounds(const Vector3& position);
	
	/**
      * @brief A velocidade do rigidbody no ponto worldPoint no espa�o global.
      * @param worldPoint ponto no mundo
     */
	Vector3 getPointVelocity(const Vector3& worldPoint);
	
	/**
      * @brief A velocidade relativa para a rigidbody no ponto RelativePoint .
      * @param localPoint ponto local.
     */
	Vector3 getRelativePointVelocity(const Vector3& localPoint);
	
	/**
      * @brief Rigidbody est� dormindo?
     */
	bool isSleeping();
	
	/**
      * @brief Move o rigidbody a posi��o .
		Mudando a posi��o de um objeto Rigidbody definindo seu valor Transform.Position 
		far� com que ele "teletransportar" diretamente para a nova posi��o (ou seja, ele 
		n�o se move atrav�s de quaisquer posi��es intermedi�rias ao longo do caminho). 
		Quaisquer outras Rigidbodies situada entre a posi��o velho eo novo n�o ser� 
		afetada pelo movimento. Al�m disso, quaisquer aceleradores na posi��o de alvo 
		vai reagir imediatamente com o rigidbody movido na pr�xima atualiza��o.
		Se voc� usar MovePosition em vez disso, o motor de f�sica ir� tra�ar um caminho 
		em linha reta entre a posi��o do objeto antigo eo novo e se comportam como se ele 
		rapidamente mudou ao longo desse caminho entre as atualiza��es. Objetos situadas
		ao longo do caminho vai ser deixado de lado, se necess�rio. Isso muitas vezes d� 
		um resultado mais aceit�vel do que mudar Transformar.Posi��o. No entanto, se os 
		movimentos s�o pequenas e frequentes, em seguida, as duas t�cnicas comportam de 
		forma muito semelhante.
      * @param position posi��o de destino.
     */
	void movePosition(const Vector3& position);
	
	/**
      * @brief Gira a rigidbody a rota��o .
		Para rigidbodies cinem�ticas se aplica atrito baseado no
		movimento do rigidbody. Isso permite simular movimento / plataformas 
		rotativas com rigidbodies sentado em cima do elevador. Se voc� 
		quiser outros rigidbodies para interagir com o rigidbody cinem�tica 
		voc� precisa mov�-lo na fun��o FixedUpdate.
      * @param rotation Quaternion de rota��o.
     */
	void moveRotation(const Quaternion& rotation);
	
	/**
      * @brief Define a massa com base nos aceleradores anexados assumindo uma densidade constante.
        Isto � �til para ajustar a massa para um valor que � proporcional ao tamanho dos aceleradores.
      * @param dens densidade.
     */
	void setDensity(float dens);
	
	/**
      * @brief For�a uma rigidbody dormir pelo menos um quadro.
        Um uso comum � chamar isto de Awake, a fim de fazer um sono rigidbody na inicializa��o. Veja Rigidbody Dormir para mais informa��es.
     */
	void sleep();
	
	/**
      * @brief Acorda um Rigidbody.
     */
	void wakeUp();
	
	/**
      * @brief Retorna:
		booleano True quando a varredura rigidbody cruza qualquer colisor, caso 
		contr�rio false.
		Descri��o:
		Testa se um rigidbody iria colidir com qualquer coisa, se ele foi movido 
		pela cena.
		Isto � semelhante ao que faz um Physics.Raycast para todos os pontos 
		contidos em qualquer dos aceleradores de um Rigidbody e retornando o mais
		pr�ximo de todos os acessos (se houver) relatou. Isso � �til para c�digo 
		de AI, quando voc� precisa de saber se um objeto caberia em algum lugar, 
		sem colidir com qualquer coisa.
		Note que esta fun��o s� funciona quando um tipo primitivo colisor (esfera, 
		cubo ou c�psula) � anexado ao objeto rigidbody - malha aceleradores n�o vai 
		funcionar, embora possam ser detectados na cena pela varredura.
     */
	bool sweepTest(const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF);
	RaycastHitList sweepTestAll(const Vector3& direction, float distance = Math::InfinityF);

	void setScale(const Vector3& s);

private:

	virtual void applyDesc(int key, const std::string& value);
	
private:
	physx::PxRigidDynamic* actor;

	ComponentDescMap mRigibodyDescMap;

	CollisionDetectionMode collisionDetectionMode;

	bool enabled;

};
#endif