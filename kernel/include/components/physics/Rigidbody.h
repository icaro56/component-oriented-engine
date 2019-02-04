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

	\warning Passar código para um cabeçalho apenas com struturas e enumerators
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
      * @brief Controle de posição de um objeto por meio de simulação de física.
		Componentes Rigidbody assumir o controle sobre a posição de um objeto -
		que faz com que os objetos caem sob a influência da gravidade, e pode 
		calcular como os objetos vão responder a colisões.
		Ao manipular parâmetros rigidbody você deve trabalhar dentro da função 
		FixedUpdate . Simulação física é realizada em passos_de_tempo discretas.
		A função FixedUpdate é chamada imediatamente antes de cada etapa.
		Coisas que atente para quando se usa corpos rígidos:
		1. Se a sua simulação parece câmera lenta e não sólida : 
		Este é um problema de escala. Quando o mundo do jogo é enorme tudo parecerá
		estar se movem muito lentamente. Certifique-se de todos os seus modelos 
		são em tamanhos reais. Por exemplo, um carro deve ser de cerca de 4 metros
		de comprimento. Um personagem em torno de 2 metros de altura. Um objeto cai 
		na mesma aceleração, não importa grande ou pequeno, pesado ou leve. Se o seu
		mundo de jogo tem uma grande escala, os objetos ainda vai cair na mesma 
		aceleração. Mas uma vez que tudo é maior, os objetos aparecerão como eles caem mais lento.
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
      * @param e booleano de ativação
    */
	virtual void setEnabled(bool e);
	
	/**
      * @brief Acessa se o Componente esta ativado
    */
	virtual bool isEnabled();
	
	/**
      * @brief Recebe um shape e aloca-o no Componente 
      * @param s Shape Físico
    */
	void attachShape(physx::PxShape* s);
	
	physx::PxRigidDynamic* getActor();
	
	/**
      * @brief Seta o valor de arrasto angular , Quanto mais elevada for a resistência mais a rotação diminui.
      * @param a valor de arrasto
    */
	void setAngularDrag(float a);
	
	/**
      * @brief Acessa se o valor de arrasto angular
    */
	float getAngularDrag();
	
	/**
      * @brief Seta o valor de velocidade angular , Na maioria dos casos você não deve modificá-lo diretamente, pois isso pode resultar em um comportamento irrealista.
      * @param angVec vetor de arrasto
    */
	void setAngularVelocity(const Vector3& angVec);
	
	/**
      * @brief Acessa se o valor de velocidade angular
    */
	Vector3 getAngularVelocity();
	
	/**
      * @brief Atribui o Centro da massa do GameObject 
      * @param center Vetor da posição desejada
     */
	void setCenterOfMass(const Vector3& center);
	
	/**
      * @brief Acessa a posição do centro da massa
    */
	Vector3 getCenterOfMass();
	
	/**
      * @brief Seta o tipo de detecção de colisão que o Rigidbody executara
      * @param mode tipo de detecção de colisão
     */
	void setCollisionDetectionMode(CollisionDetectionMode mode);
	CollisionDetectionMode getCollisionDetectionMode();

	//physx version used by unity is 2.8: https://devtalk.nvidia.com/default/topic/538594/physx-and-physics-modeling/missing-features-in-3-series-compared-to-2-/
	//void setConstraints();

	/**
      * @brief O tensor de inércia de massa na diagonal em relação ao centro de massa.
      * @param i ponto que exercerá a inércia
     */
	void setInertiaTensor(const Vector3& i);
	
	/**
      * @brief Acessa o ponto de tensão da inércia
    */
	Vector3 getInertiaTensor();
	
	/**
      * @brief Ativa se haverá detecção de colisão
      * @param enable booleano de ativação
     */
	void setDetectCollision(bool enable);
	
	/**
      * @brief Acessa se a colisão ativada
    */
	bool getDetectCollision();
	
	/**
      * @brief Atribui uma resistência de arrasto ao objeto
      * @param d valor de resistência
     */
	void setDrag(float d);
	
	/**
      * @brief Acessa o valor de resistência de arrasto do objeto
    */
	float getDrag();

	//veja setConstraints
	//void setFreezeRotation(bool enable);

	/**
      * @brief Controla se a física afetará o Rigidbody
      * @param active booleano de ativação
     */
	void setKinematic(bool active);
	
	/**
      * @brief Acessa se a física afetará o Rigidbody
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
      * @brief Acessa o ponto de posição do objeto na cena
    */
	Vector3 getPosition();
	
	/**
      * @brief Atribui um Quaterniom para a rotação do objeto
      * @param q Quaterniom de rotação
     */
	void setRotation(const Quaternion& q);
	
	/**
      * @brief Acessa o Quaternion de rotaçao do objeto
    */
	Quaternion getRotation();
	
	/**
      * @brief Atribui um valor de descanço de velocidade
      * @param v valor de descanço
     */
	void setSleepVelocity(float v);
	
	/**
      * @brief Acessa o valor de descanço
    */
	float getSleepVelocity();
	
	/**
      * @brief Permite substituir a contagem de solver iteração por rigidbody.
      * @param s quantidade de iteração ao Rigidbody
     */
	void setSolverIterationCount(int s);
	
	/**
      * @brief Acessa o valor de quantidade de iteração ao Rigidbody
    */
	int getSolverIterationCount();
	
	/**
      * @brief Ativa gravidade
      * @param enable Booleano de ativação
     */
	void useGravity(bool enable);
	
	/**
      * @brief Se a gravidade está ativada
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
      * @brief Adiciona uma força ao Rigidbody
      * @param force direção da força
      * @param mode tipo de força
     */
	void addForce(const Vector3& force, ForceMode mode = ForceMode::Force);
	
	/**
      * @brief Adiciona uma força ao Rigidbody para determinada posição
      * @param force direção da força
      * @param position poisção de destino
      * @param mode tipo de força
     */
	void addForceAtPosition(const Vector3& force, const Vector3& position, ForceMode mode = ForceMode::Force);
	
	/**
      * @brief Adiciona uma força para o rigidbody em relação ao sistema de coordenadas.
      * @param force direção da força
      * @param mode tipo de força
     */
	void addRelativeForce(const Vector3& force, ForceMode mode = ForceMode::Force);
	
	//rever
	/**
      * @brief Adiciona um torque ao rigidbody em relação ao sistema de coordenadas do próprio rigidbodie.
      * @param torque torque relativo
      * @param mode tipo de força
     */
	void addRelativeTorque(const Vector3& torque, ForceMode mode = ForceMode::Force);
	
	/**
      * @brief Adiciona um torque ao rigidbody.
      * @param torque valor do torque
      * @param mode tipo de força
     */
	void addTorque(const Vector3& torque, ForceMode mode = ForceMode::Force);

	//rever
	/**
      * @brief O ponto mais próximo à caixa delimitadora dos aceleradores em anexo.
      * @param position posição em relação ao Rigidbody
     */
	Vector3 closestPointOnBounds(const Vector3& position);
	
	/**
      * @brief A velocidade do rigidbody no ponto worldPoint no espaço global.
      * @param worldPoint ponto no mundo
     */
	Vector3 getPointVelocity(const Vector3& worldPoint);
	
	/**
      * @brief A velocidade relativa para a rigidbody no ponto RelativePoint .
      * @param localPoint ponto local.
     */
	Vector3 getRelativePointVelocity(const Vector3& localPoint);
	
	/**
      * @brief Rigidbody está dormindo?
     */
	bool isSleeping();
	
	/**
      * @brief Move o rigidbody a posição .
		Mudando a posição de um objeto Rigidbody definindo seu valor Transform.Position 
		fará com que ele "teletransportar" diretamente para a nova posição (ou seja, ele 
		não se move através de quaisquer posições intermediárias ao longo do caminho). 
		Quaisquer outras Rigidbodies situada entre a posição velho eo novo não será 
		afetada pelo movimento. Além disso, quaisquer aceleradores na posição de alvo 
		vai reagir imediatamente com o rigidbody movido na próxima atualização.
		Se você usar MovePosition em vez disso, o motor de física irá traçar um caminho 
		em linha reta entre a posição do objeto antigo eo novo e se comportam como se ele 
		rapidamente mudou ao longo desse caminho entre as atualizações. Objetos situadas
		ao longo do caminho vai ser deixado de lado, se necessário. Isso muitas vezes dá 
		um resultado mais aceitável do que mudar Transformar.Posição. No entanto, se os 
		movimentos são pequenas e frequentes, em seguida, as duas técnicas comportam de 
		forma muito semelhante.
      * @param position posição de destino.
     */
	void movePosition(const Vector3& position);
	
	/**
      * @brief Gira a rigidbody a rotação .
		Para rigidbodies cinemáticas se aplica atrito baseado no
		movimento do rigidbody. Isso permite simular movimento / plataformas 
		rotativas com rigidbodies sentado em cima do elevador. Se você 
		quiser outros rigidbodies para interagir com o rigidbody cinemática 
		você precisa movê-lo na função FixedUpdate.
      * @param rotation Quaternion de rotação.
     */
	void moveRotation(const Quaternion& rotation);
	
	/**
      * @brief Define a massa com base nos aceleradores anexados assumindo uma densidade constante.
        Isto é útil para ajustar a massa para um valor que é proporcional ao tamanho dos aceleradores.
      * @param dens densidade.
     */
	void setDensity(float dens);
	
	/**
      * @brief Força uma rigidbody dormir pelo menos um quadro.
        Um uso comum é chamar isto de Awake, a fim de fazer um sono rigidbody na inicialização. Veja Rigidbody Dormir para mais informações.
     */
	void sleep();
	
	/**
      * @brief Acorda um Rigidbody.
     */
	void wakeUp();
	
	/**
      * @brief Retorna:
		booleano True quando a varredura rigidbody cruza qualquer colisor, caso 
		contrário false.
		Descrição:
		Testa se um rigidbody iria colidir com qualquer coisa, se ele foi movido 
		pela cena.
		Isto é semelhante ao que faz um Physics.Raycast para todos os pontos 
		contidos em qualquer dos aceleradores de um Rigidbody e retornando o mais
		próximo de todos os acessos (se houver) relatou. Isso é útil para código 
		de AI, quando você precisa de saber se um objeto caberia em algum lugar, 
		sem colidir com qualquer coisa.
		Note que esta função só funciona quando um tipo primitivo colisor (esfera, 
		cubo ou cápsula) é anexado ao objeto rigidbody - malha aceleradores não vai 
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