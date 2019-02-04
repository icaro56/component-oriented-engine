#include "engine/callback/SceneCallback.h"
#include "engine/Engine.h"

#include "gameobject/GameObject.h"
#include "components/mesh/MeshRenderer.h"
#include "components/mesh/SkinnedMeshRenderer.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"
#include "components/miscellaneous/Animation.h"

#include "utils/KernelConverter.h"
#include "math/Vector3.h"

SceneCallback::SceneCallback()
{
    //ctor
}
//---------------------------------------------------------
SceneCallback::~SceneCallback()
{
    //dtor
}
//---------------------------------------------------------
void SceneCallback::CreatedEntity(const OgreMax::OgreMaxScene *scene,
                                  Ogre::Entity *entity)
{
	if (entity->getParentSceneNode())
	{
		Ogre::Vector3 worldpos = entity->getParentSceneNode()->_getDerivedPosition();
		Ogre::Quaternion worldori = entity->getParentSceneNode()->_getDerivedOrientation();

		GameObject* gameObject = new GameObject(entity->getName());
		Transform* transform = new Transform();
		transform->_setOgreSceneNode(entity->getParentSceneNode());

		if (entity->hasSkeleton())
		{
			SkinnedMeshRenderer* renderer = new SkinnedMeshRenderer();
			renderer->setOgreEntity(entity);
			gameObject->addComponent(renderer);
			gameObject->addComponent(new Animation());
		}
		else
		{
			MeshRenderer* renderer = new MeshRenderer();
			renderer->setOgreEntity(entity);
			gameObject->addComponent(renderer);
		}

		gameObject->transform->setPosition(Convert::toKernel(worldpos));
		gameObject->transform->setRotation(Convert::toKernel(worldori));
	}
}
//---------------------------------------------------------
void SceneCallback::CreatedCamera(const OgreMax::OgreMaxScene* scene,
                                  Ogre::Camera* camera)
{
	Ogre::Vector3 worldpos = camera->getDerivedPosition();
	Ogre::Quaternion worldori = camera->getDerivedOrientation();

	camera->setPosition(0,0,0);

	GameObject* camObject = new GameObject(camera->getName());
	Transform* transform = new Transform();
	transform->_setOgreSceneNode(camera->getParentSceneNode());
	Camera* cameraComp = new Camera();
	cameraComp->setOgreCamera(camera);
	camObject->addComponent(cameraComp);

	camObject->transform->setPosition(Convert::toKernel(worldpos));
	camObject->transform->setRotation(Convert::toKernel(worldori));
}
