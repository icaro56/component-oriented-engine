#ifndef COLLIDER_H
#define COLLIDER_H

#include "KernelPrerequisites.h"
#include "../Component.h"
#include <QObject>

#include "math/Vector3.h"
#include "engine/buffers/Collision.h"

namespace physx
{
	class PxRigidStatic;
	class PxShape;
	class PxMaterial;
	class PxBounds3;
}


/*
!@brief Classe base para todos colisores
Se o objeto com o Collider precisa ser movido durante o jogo,
ent�o voc� tamb�m deve anexar um componente Rigidbody ao objeto.
O Rigidbody pode ser configurado para ser cinem�tica, se voc� n�o
deseja que o objeto tenha intera��o f�sica com outros objetos.
*/

class KERNEL_VS_EXPORT Collider : public QObject, public Component
{
	Q_OBJECT

	friend class SimulationEventCallback;
public:
	enum ColliderDesc
	{
		CD_Enabled,
		CD_IsTrigger,
		CD_IsSceneQuery,
		CD_Size
	};

	/**
      * @brief Construtor Default
    */
	Collider();
	/**
      * @brief Construtor 
      * @param idComponent identidade do componente
     */
	Collider(const std::string& idComponent, const std::string& fID = "Collider");
    /**
     * @brief Destrutor Default 
    */
	virtual ~Collider();

	virtual void prerequisites();
	virtual void init(const std::string& name){}
	virtual void nextInit();
	virtual void reset();

	physx::PxBounds3 getBounds();

	/*!
		\brief Ativa e Desativa colis�o. Se trigger est� ativo, logo colis�o fica desativada.
	*/
	virtual void setEnabled(bool e);

	/*!
		\brief Acessa se a colis�o est� ativada.
	*/
	virtual bool isEnabled();

	/*!
		\brief Ativa se a colis�o ser� do tipo trigger.
		\param e se verdadeiro ativa a op��o trigger
	*/
	void setTrigger(bool e);

	/*!
		\brief Acessa se a colis�o � do tipo trigger.
	*/
	bool isTrigger();

	/*!
		\brief Ativa se a collider participar� de scenes queries.
		\param e se verdadeiro ativa a op��o query
	*/
	void setSceneQuery(bool active);

	/*!
		\brief Acessa se o collider participa de scene queries.
	*/
	bool isSceneQuery();

	/*!
		\brief Atribui um material f�sico ao collider.
		\param m material atribuido ao collider
	*/
	void setMaterial(physx::PxMaterial* m);
	
	/*!
		\brief Acessa um material f�sico do collider.
	*/
	physx::PxMaterial* getMaterial();
	
	/**
      * @brief Atribui a escala do Collider 
      * @param s Vetor da escala desejada
     */
	virtual void setScale(const Vector3& s) = 0;

	/*!
		\brief Com quais grupos os eventos de colis�o ser�o reportados.
	*/
	virtual void setReportCollision(bool active);
	bool getReportCollision();

	/*!
		\brief Configura o layer do collider.
	*/
	virtual void setLayerId(unsigned layerId);
	unsigned getLayerId();

	//retorn 1 << x
	unsigned getLayer();

	void setRayPass(bool pass);
	bool getRayPass();

	bool _getWasDeleted();
	void _setWasDeleted(bool w);
	physx::PxRigidStatic * _getStaticActor();

signals:
	
	/*!
		\brief OnCollisionEnter � chamado quando este colisor / rigidbody come�ou tocar outra rigidbody / colisor.
	*/
	void onColisionEnter(Collision& collision);
	
	/*!
		\brief OnCollisionStay � chamado uma vez por quadro para cada colisor / rigidbody que est� tocando rigidbody / colisor.
	*/
	void onColisionStay(Collision& collision);
	
	/*!
		\brief OnCollisionExit � chamado quando este colisor / rigidbody parou de tocar outra rigidbody / colisor.
	*/
	void onColisionExit(Collision& collision);
	
	/*!
		\brief OnTriggerEnter � chamado, quando o LHC outra entra no gatilho.
	*/
	void onTriggerEnter(Collider* otherCollider);
	
	/*!
		\brief OnTriggerExit � chamado quando o Collider outro parou de tocar o gatilho.
	*/
	void onTriggerExit(Collider* otherCollider);

protected:
	void destroyCollider();

	virtual void applyDesc(int key, const std::string& value);

	void informOnCollisionEnter(Collision &collision);
	void informOnCollisionStay(Collision& collision);
	void informOnCollisionExit(Collision& collision);
	void informOnTriggerEnter(Collider* collider);
	void informOnTriggerExit(Collider* collider);

	void informSlotsClients();
protected:
	//usado caso n�o haja rigidbody atachado
	physx::PxRigidStatic *pxStaticActor;
	physx::PxShape* pxShape;

	ComponentDescMap mColliderDescMap;
	bool wasDeleted, wasTrigger, wasSceneQuery;
	bool isStatic;
	bool rayPass;
};

#endif