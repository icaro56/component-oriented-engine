#include "gameobject/GameObjectManager.h"
#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "engine/Engine.h"
#include "gameobject/GameObjectTemplateManager.h"

GameObjectManager* GameObjectManager::instance = nullptr;

GameObjectManager* GameObjectManager::getInstance()
{
	if(!instance)
		instance = new GameObjectManager();

	return instance;
}

GameObjectManager::GameObjectManager()
{

}

GameObjectManager::~GameObjectManager()
{

}

void GameObjectManager::addObject(GameObject* gameObject)
{
	if (!gameObjects.count(gameObject->getName()))
	{
		gameObjects[gameObject->getName()] = gameObject;
	}
}

void GameObjectManager::destroy(GameObject* gameObject)
{
	if (gameObjects.count(gameObject->getName()))
	{
		gameObjects.erase(gameObject->getName());
		gameObject->getParentScene()->removeGameObject(gameObject);

		delete gameObject;
		gameObject = 0;
	}
}

void GameObjectManager::destroyAll()
{
	GameObjectMap::iterator iter;
	for(iter = gameObjects.begin(); iter != gameObjects.end(); iter++) 
	{
		GameObject* gameobject = iter->second;

		if (gameobject)
		{
			delete gameobject;
			gameobject = 0;
		}
	}

	gameObjects.clear();

	//como deletou todos gameobjects, logo os Scenes precisam ser informados
	GEngine->clearScenes();
}

void GameObjectManager::removeObject(GameObject* gameObject)
{
	if (gameObjects.count(gameObject->getName()))
	{
		gameObjects.erase(gameObject->getName());
	}
}

GameObject* GameObjectManager::find(const std::string& name)
{
	if (gameObjects.count(name))
	{
		return gameObjects[name];
	}

	return NULL;
}

GameObject* GameObjectManager::createPrimitive(PrimitiveType type, const std::string& name)
{
	GameObject* g;

	switch (type)
	{
	case PrimitiveType::Sphere:
		g = GameObjectTemplateManager::getInstance()->createObject("SpherePrefab", name);
		break;
	case PrimitiveType::Capsule:
		g = GameObjectTemplateManager::getInstance()->createObject("CapsulePrefab", name);
		break;
	case PrimitiveType::Cube:
		g = GameObjectTemplateManager::getInstance()->createObject("BoxPrefab", name);
		break;
	default:
		break;
	}

	return g;
}
