#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "KernelPrerequisites.h"
#include <map>

class GameObject;

enum class PrimitiveType
{
	Sphere,
	Capsule,
	Cube
};

class KERNEL_VS_EXPORT GameObjectManager
{
	friend class Scene;

	typedef std::map<const std::string, GameObject*> GameObjectMap;

public:
	static GameObjectManager* getInstance();

	void destroy(GameObject* gameObject);
	void destroyAll();
	GameObject* find(const std::string& name);
	GameObject* createPrimitive(PrimitiveType type, const std::string& name);

private:
	GameObjectManager();
	~GameObjectManager();

	void addObject(GameObject* gameObject);
	void removeObject(GameObject* gameObject);

private:
	static GameObjectManager* instance;

	//todos gameobjects da aplicação
	GameObjectMap gameObjects;
};

#endif