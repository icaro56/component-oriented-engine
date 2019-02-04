#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "KernelPrerequisites.h"

#include <string>
#include <map>
#include <utility>

#include "components/Component.h"

class Scene;
class Transform;
class Camera;
class Light;
class Renderer;
class ParticleSystem;
class Animation;
class AudioSource;
class FollowText;
class FollowTexture;
class Billboard;
class BoundingBox;
class Collider;
class BoxCollider;
class SphereCollider;
class CapsuleCollider;
class MeshCollider;
class TerrainCollider;
class WheelCollider;
class Rigidbody;
class MeshFilter;
class Script;
class ConstantForce;
class TrailRenderer;

/*! \class GameObject
	\brief Classe base para todas entidades na cena.
*/
class KERNEL_VS_EXPORT GameObject 
{
	friend class Scene;
	friend class GameObjectManager;
public:
	typedef std::map<const std::string, Component*> ComponentMap;
	typedef std::vector<Component*> ComponentVector;
	/*!
		\brief Constructor
		\param id  Nome do GameObject
	*/
	GameObject( const std::string& id, Scene* scene = NULL, bool isRoot = false);

	/*!
		\brief Acessa o nome do GameObjetc
		\param null
	*/
	const std::string& getName() const;

	/*!
		\brief Atribui um nome do GameObjetc
		\param n  Nome do Game Object
	*/
	void setName( const std::string& n );

	ComponentMap& getComponents();

	ComponentVector getComponents(const std::string& familyId);
	
	ComponentVector getComponentsInChildren(const std::string& familyId);

	 /*!
		\brief Responsavel por acessar componentes por meio do nome do componente
		\param compId nome do componente
	 */
	Component* getComponent(const std::string& compId );

	/*!
		\brief Returns the component of Type type in the GameObject or any of its children using depth first search.
	 */
	Component* getComponentInChildren(const std::string& compId);

	/*!
		\brief Responsavel por acessar componentes por meio do tipo do componente
		\param 	type Enum do tipo de Componente
	*/
	//Component* getComponent(ComponentType type); 

	template<class T1>
	std::string getTypeName(T1 a)
	{
		std::string s = ;
		return s;
	}

	/*!
		\brief Responsavel por acessar componentes por meio do cast da função
		\obs getComponent<Transform>();
	*/
	template<class T1>
	T1* getComponent()
	{
		//cria uma variável temporária para pegar o id do componente
		T1 teste;
		std::string cid = teste.componentID();

		Component* component = getComponent(cid);
		if (component)
			return dynamic_cast<T1*>(component);

		return NULL;
	}

	template<class T1>
	T1* getComponentInChildren()
	{
		//cria uma variável temporária para pegar o id do componente
		T1 teste;
		std::string cid = teste.componentID();

		Component* component = getComponentInChildren(cid);
		if (component)
			return dynamic_cast<T1*>(component);

		return NULL;
	}

	template<class T1>
	ComponentVector getComponents()
	{
		//cria uma variável temporária para pegar o id do componente
		T1 teste;
		std::string cid = teste.familyID();

		ComponentVector components = getComponents(cid);
		return components;
	}

	template<class T1>
	ComponentVector getComponentsInChildren()
	{
		//cria uma variável temporária para pegar o id do componente
		T1 teste;
		std::string cid = teste.familyID();

		ComponentVector components = getComponentsInChildren(cid);
		return components;
	}

	/*!
		\brief Adiciona componente ao gameObject
		\param newComp recebe uma classe de componente
	*/
	void addComponent(Component *newComp);

	void removeComponent(Component *c);
	//destroi componente sem se importar com dependencias
	void destroyComponent(Component *c);

	/*!
		\brief Destroi todos componentes do gameObject
	*/
	void clearComponents();

	/*!
		\brief Update do gameObject e seus Componentes
	*/
	void update();

	void fixedUpdate();
	void lateUpdate();

	void setParentScene(Scene* s);
	Scene* getParentScene();

	// atalhos para todos outros componentes disponíveis, null se componente não existe
	Transform* transform;
	Camera* camera; 
	Light* light;
	Renderer* renderer;
	ParticleSystem* particleSystem;
	Animation* animation;
	AudioSource* audio;
	Billboard* billboard;
	FollowText* followText;
	FollowTexture* followTexture;
	BoundingBox* boundingBox;
	Collider* collider;
	Rigidbody* rigidbody;
	//primeiro script adicionado
	Script* script;
	ConstantForce* constantForce;
	TrailRenderer* trailRenderer;

	void setTag(const std::string& tagName);
	std::string getTag();
	bool compareTag(const std::string& otherTag);

	void setLayer(const std::string& layerName);
	std::string getLayer();

	void setActive(bool a);
	bool getActiveSelf();

	bool getActiveInHierarchy();

private:
	/*!
		\brief Destructor
		\param null
	*/
	virtual ~GameObject();

	void shortcut(Component* comp);
	void disableShortcut(const std::string& fam, const std::string& comp);

private:
	std::string name;			//unique identifier for this GameObject

	typedef std::map<const std::string, unsigned> ComponentFamilyCountMap;

	ComponentMap mComponents;		//map of all components;
	ComponentFamilyCountMap mFamilyCountMap;

	Scene* parentScene;

	std::string tag;
	std::string layer;

	bool activeSelf;
	bool activeParent;
};

#endif
