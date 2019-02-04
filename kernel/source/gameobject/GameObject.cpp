#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "gameobject/Tag.h"
#include "gameobject/LayerMask.h"

#include "components/transform/Transform.h"
#include "components/rendering/Camera.h"
#include "components/rendering/Light.h"
#include "components/mesh/MeshRenderer.h"
#include "components/effects/ParticleSystem.h"
#include "components/miscellaneous/Animation.h"
#include "components/rendering/Billboard.h"
#include "components/rendering/BoundingBox.h"
#include "components/rendering/FollowText.h"
#include "components/rendering/FollowTexture.h"
#include "components/audio/AudioSource.h"
#include "components/physics/Collider.h"
#include "components/physics/Rigidbody.h"
#include "components/physics/ConstantForce.h"
#include "components/script/Script.h"
#include "components/effects/TrailRenderer.h"

#include "engine/Engine.h"
#include "engine/Scene.h"

#include "utils/KernelConverter.h"

GameObject::GameObject( const std::string& id, Scene* scene, bool isRoot)
  : camera(nullptr),
	light(nullptr),
	renderer(nullptr),
	particleSystem(nullptr),
	animation(nullptr),
	audio(nullptr),
	billboard(nullptr),
	followText(nullptr),
	boundingBox(nullptr),
	followTexture(nullptr),
	collider(nullptr),
	rigidbody(nullptr),
	script(nullptr),
	constantForce(nullptr),
	tag("Untagged"),
	layer("Default"),
	activeSelf(true),
	activeParent(true),
	trailRenderer(nullptr)
{
	setName(id);

	//std::string sceneName = scene ? scene->getName() : "";

	if (scene)
	{
		parentScene = scene;

		if (!parentScene)
		{
			parentScene = GEngine->getCurrentScene();
			std::cerr << "\nCena não encontrada, usando cena principal";
		}
	}
	else
	{
		parentScene = GEngine->getCurrentScene();
	}
	
	parentScene->addGameObject(this);

	Transform* t = new Transform();

	if (isRoot)
		t->iAmRootScene(true);

	addComponent(t);
}

GameObject::~GameObject()
{
	clearComponents();

	//parentScene->removeGameObject(this);
}

const std::string& GameObject::getName() const 
{ 
	return name; 
}

void GameObject::setName( const std::string& n ) 
{ 
	name = n; 
}

Component* GameObject::getComponent(const std::string& compId ) 
{
	if (mComponents.count(compId))
		return mComponents[compId];

	return NULL;
}

Component* GameObject::getComponentInChildren(const std::string& compId)
{
	if (transform && transform->getChildCount())
	{
		Transform::TransformChildren children = transform->getChildren();

		for (auto it = children.begin(); it != children.end(); ++it)
		{
			GameObject* aux = it->second->getGameObject();

			Component* c = aux->getComponent(compId);

			if (c)
			{
				return c;
			}
		}
	}

	return nullptr;
}

GameObject::ComponentVector GameObject::getComponents(const std::string& familyId)
{
	unsigned count = mFamilyCountMap.count(familyId);
	ComponentVector list;

	if (count)
	{
		unsigned i = 0;
		for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if (i < count)
			{
				if (it->second->familyID() == familyId)
				{
					list.push_back(it->second);
					//map[it->first] = it->second;
					++i;
				}
			}
			else
			{
				break;
			}
		}
	}

	return list;
}

GameObject::ComponentVector GameObject::getComponentsInChildren(const std::string& familyId)
{
	if (transform && transform->getChildCount())
	{
		Transform::TransformChildren children = transform->getChildren();
		ComponentVector list;

		for (auto it = children.begin(); it != children.end(); ++it)
		{
			GameObject* aux = it->second->getGameObject();

			ComponentVector c = aux->getComponents(familyId);

			if (!c.empty())
			{
				list.insert(list.end(), c.begin(), c.end());
			}
		}

		return list;
	}

	ComponentVector list2;
	return list2;
}

GameObject::ComponentMap& GameObject::getComponents()
{
	return mComponents;
}

void GameObject::addComponent(Component *newComp)
{
	std::string family = newComp->familyID();
	std::string myName = newComp->componentID();

	unsigned countFamily = 0;
	
	//se já existe familia do componente registrada
	if (mFamilyCountMap.count(family))
		countFamily = mFamilyCountMap[family]; // recupera a quantidade de componentes da mesma família

	//se são componentes exclusivos, ou seja, que só pode ter 1 para cada gameObject
	if (family != "AudioSource" && family != "Collider" && family != "Script"/* && family != "MeshFilter" && family != "Renderer"*/)
	{
		if (!countFamily)
		{
			mFamilyCountMap[family] = countFamily + 1;
			mComponents[myName] = newComp;

			// atribuindo atalhos
			shortcut(newComp);

			newComp->setGameObject(this);
			newComp->prerequisites();
			newComp->init(this->getName());
			newComp->nextInit();
		}
		else
		{
			std::cerr << "\nSomente pode ter um " + family + " atachado ao objeto";
			delete newComp;
			newComp = nullptr;
			return;
		}
	}
	else
	{
		unsigned ini = countFamily + 1;
		unsigned i = ini;

		mFamilyCountMap[family] = i;

		unsigned sizeString = myName.size();
		while(mComponents.count(myName))
		{
			if (i > ini)
				myName = myName.erase(sizeString);

			myName += "_" + Convert::toString(i);
			i++;
		}
		
		if (i > ini)
			newComp->setComponentID(myName);

		mComponents[myName] = newComp;

		newComp->setGameObject(this);
		newComp->prerequisites();

		if (!countFamily)
		{
			// atribuindo atalhos
			shortcut(newComp);
		}
		
		newComp->init(this->getName());
		newComp->nextInit();
	}
}

void GameObject::removeComponent(Component *c)
{
	if (c)
	{
		if (mComponents.count(c->componentID()))
		{
			if (mFamilyCountMap.count(c->familyID()))
				mFamilyCountMap[c->familyID()] -= 1;

			if (c->familyID() == "Rigidbody")
			{
				if (collider)
				{
					GameObject::ComponentVector map = getComponents("Collider");

					for(unsigned i = 0; i < map.size(); ++i)
					//for (auto it = map.begin(); it != map.end(); ++it)
					{
						Collider* c = dynamic_cast<Collider*>(map.at(i));
						if (c)
						{
							c->reset();
						}
					}
				}
			}

			mComponents.erase(c->componentID());
			delete c;
			c = nullptr;
		}
	}
}

void GameObject::destroyComponent(Component *c)
{
	if (c)
	{
		if (mComponents.count(c->componentID()))
		{
			if (mFamilyCountMap.count(c->familyID()))
				mFamilyCountMap[c->familyID()] -= 1;

			mComponents.erase(c->componentID());
			disableShortcut(c->familyID(), c->componentID());
			delete c;
			c = nullptr;
		}
	}
}

void GameObject::clearComponents() 
{
	std::map<const std::string, Component*>::iterator iter;
	for(iter = mComponents.begin(); iter != mComponents.end(); ) 
	{
		Component* c = iter->second;
		if (c)
		{
			if (mFamilyCountMap.count(c->familyID()))
				mFamilyCountMap[c->familyID()] -= 1;

			std::string n = c->familyID();
			std::string m = c->componentID();

			disableShortcut(n, m);

			delete c;
			c = nullptr;

			

			mComponents.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
	mComponents.clear();
	mFamilyCountMap.clear();
}

void GameObject::update()
{
	if (activeParent && getActiveSelf())
	{
		std::map<const std::string, Component*>::iterator iter;
		for(iter = mComponents.begin(); iter != mComponents.end(); iter++) 
		{
			iter->second->update();
		}
	}
}

void GameObject::fixedUpdate()
{
	if (activeParent && getActiveSelf())
	{
		std::map<const std::string, Component*>::iterator iter;
		for(iter = mComponents.begin(); iter != mComponents.end(); iter++) 
		{
			iter->second->fixedUpdate();
		}
	}
}

void GameObject::lateUpdate()
{
	if (activeParent && getActiveSelf())
	{
		std::map<const std::string, Component*>::iterator iter;
		for(iter = mComponents.begin(); iter != mComponents.end(); iter++) 
		{
			iter->second->lateUpdate();
		}
	}
}

void GameObject::setParentScene(Scene* s)
{
	parentScene = s;
}

Scene* GameObject::getParentScene()
{
	return parentScene;
}

void GameObject::shortcut(Component* comp)
{
	if (comp->familyID() == "Transform")
		transform = dynamic_cast<Transform*>(comp);
	else if (comp->familyID() == "Camera")
		camera = dynamic_cast<Camera*>(comp);
	else if (comp->familyID() == "Light")
		light = dynamic_cast<Light*>(comp);
	else if (comp->familyID() == "Renderer")
		renderer = dynamic_cast<Renderer*>(comp);
	else if (comp->familyID() == "ParticleSystem")
		particleSystem = dynamic_cast<ParticleSystem*>(comp);
	else if (comp->familyID() == "Animation")
		animation = dynamic_cast<Animation*>(comp);
	else if (comp->familyID() == "AudioSource")
		audio = dynamic_cast<AudioSource*>(comp);
	else if (comp->familyID() == "Billboard")
		billboard = dynamic_cast<Billboard*>(comp);
	else if (comp->familyID() == "BoundingBox")
		boundingBox = dynamic_cast<BoundingBox*>(comp);
	else if (comp->familyID() == "FollowText")
		followText = dynamic_cast<FollowText*>(comp);
	else if (comp->familyID() == "FollowTexture")
		followTexture = dynamic_cast<FollowTexture*>(comp);
	else if (comp->familyID() == "Collider")
		collider = dynamic_cast<Collider*>(comp);
	else if (comp->familyID() == "Rigidbody")
		rigidbody = dynamic_cast<Rigidbody*>(comp);
	else if (comp->familyID() == "Script")
		script = dynamic_cast<Script*>(comp);
	else if (comp->familyID() == "ConstantForce")
		constantForce = dynamic_cast<ConstantForce*>(comp);
	else if (comp->familyID() == "TrailRenderer")
		trailRenderer = dynamic_cast<TrailRenderer*>(comp);
}

void GameObject::disableShortcut(const std::string& fam, const std::string& comp)
{
	if (fam == "Transform")
		transform = nullptr;
	else if (fam == "Camera")
		camera = nullptr;
	else if (fam == "Light")
		light = nullptr;
	else if (fam == "Renderer")
		renderer = nullptr;
	else if (fam == "ParticleSystem")
		particleSystem = nullptr;
	else if (fam == "Animation")
		animation = nullptr;
	else if (fam == "AudioSource")
	{
		if (audio && audio->componentID() == comp)
		{
			audio = nullptr;
		}
	}
	else if (fam == "Billboard")
		billboard = nullptr;
	else if (fam == "BoundingBox")
		boundingBox = nullptr;
	else if (fam == "FollowText")
		followText = nullptr;
	else if (fam == "FollowTexture")
		followTexture = nullptr;
	else if (fam == "TrailRenderer")
		trailRenderer = nullptr;
	else if (fam == "Collider")
	{
		if (collider && collider->componentID() == comp)
			collider = nullptr;
	}
	else if (fam == "Rigidbody")
		rigidbody = nullptr;
	else if (fam == "Script")
	{
		if (script && script->componentID() == comp)
			script = nullptr;
	}
	else if (fam == "ConstantForce")
		constantForce = nullptr;
}

void GameObject::setTag(const std::string& tagName)
{
	if (TagManager::getInstance()->getTagByName(tagName) > Tag::Untagged)
	{
		tag = tagName;
	}
}

std::string GameObject::getTag()
{
	return tag;
}

bool GameObject::compareTag(const std::string& otherTag)
{
	if (getTag() == otherTag)
		return true;
	else
		return false;
}

void GameObject::setLayer(const std::string& layerName)
{
	if (LayerMask::nameToLayer(layerName) > 0)
	{
		layer = layerName;
		unsigned lID = LayerMask::nameToLayerInt(layer); // n
		unsigned l = LayerMask::nameToLayer(layer); // 1 << n

		if (mFamilyCountMap.count("Collider"))
		{
			ComponentVector map = getComponents("Collider");

			for(unsigned i = 0; i < map.size(); ++i)
			//for (auto it = map.begin(); it != map.end(); ++it)
			{
				Collider* c = dynamic_cast<Collider*>(map.at(i));
				if (c)
				{
					c->setLayerId(lID);
				}
			}
		}

		CharacterController* controller = getComponent<CharacterController>();

		if (controller)
		{
			controller->setLayerId(l);
		}

		if (particleSystem)
		{
			particleSystem->setLayer(l);
		}

		if (renderer)
		{
			renderer->setLayer(l);
		}

		if (billboard)
		{
			billboard->setLayer(l);
		}

		if (boundingBox)
		{
			boundingBox->setLayer(l);
		}

		if (camera)
		{
			camera->setLayer(l);
		}

		if (light)
		{
			light->setLayer(l);
		}

		if (trailRenderer)
		{
			trailRenderer->setLayer(l);
		}
	}
}

std::string GameObject::getLayer()
{
	return layer;
}

void GameObject::setActive(bool a)
{
	activeSelf = a;

	//avisar aos componentes e aos filhos desse game object
	if (transform->getChildCount())
	{
		Transform::TransformChildren map = transform->getChildren();
		for (auto it = map.begin(); it != map.end(); ++it)
		{
			Transform* t = it->second;
			if (t)
			{
				GameObject * game = t->getGameObject();
				game->activeParent = a;
				
				ComponentMap map2 = game->getComponents();
				for (auto it2 = map2.begin(); it2 != map2.end(); ++it2)
				{
					it2->second->setEnabled(a);
				}
			}
		}
	}
}

bool GameObject::getActiveSelf()
{
	return activeSelf;
}

bool GameObject::getActiveInHierarchy()
{
	Transform* aux = transform->getParent();
	bool activeInHierarchy = true;
	
	while (aux && aux != transform->getRoot())
	{
		if (!aux->getGameObject()->getActiveSelf())
		{
			activeInHierarchy = false;
			break;
		}
		aux = aux->getParent();
	}

	return activeInHierarchy;
}