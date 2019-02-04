#include "Component.h"

#include "components/transform/Transform.h"
#include "components/rendering/Camera.h"
#include "components/rendering/Light.h"
#include "components/rendering/Billboard.h"
#include "components/rendering/BoundingBox.h"
#include "components/rendering/FollowText.h"
#include "components/rendering/FollowTexture.h"
#include "components/mesh/MeshRenderer.h"
#include "components/effects/ParticleSystem.h"
#include "components/miscellaneous/Animation.h"
#include "components/script/Script.h"
#include "components/effects/TrailRenderer.h"

#include "gameobject/GameObject.h"

Component::Component() 
	: gameObject(nullptr), 
	  mComponentId(""),
	  mFamilyId(""),
	  initialized(false)
{

}

Component::Component(std::string idComponent, std::string idFamily)
	: gameObject(nullptr), 
	  initialized(false)
{
	mComponentId = idComponent;
	mFamilyId = idFamily;
}

Component::~Component()
{

}

void Component::update() 
{

}

void Component::fixedUpdate()
{

}

void Component::lateUpdate()
{

}

void Component::setGameObject( GameObject* obj) 
{ 
	gameObject = obj; 
}

GameObject* Component::getGameObject() const 
{ 
	return gameObject; 
}

void Component::setComponentID(std::string id)
{
	mComponentId = id;
}

const std::string& Component::componentID() const
{
	return mComponentId;
}


void Component::setFamilyID(std::string id)
{
	mFamilyId = id;
}

const std::string& Component::familyID() const
{
	return mFamilyId;
}

Transform* Component::transform()
{
	return gameObject->transform;
}

Camera* Component::camera()
{
	return gameObject->camera;
}

Billboard* Component::billboard()
{
	return gameObject->billboard;
}

BoundingBox* Component::boundingBox()
{
	return gameObject->boundingBox;
}

Light* Component::light()
{
	return gameObject->light;
}

Renderer* Component::renderer()
{
	return gameObject->renderer;
}

ParticleSystem* Component::particleSystem()
{
	return gameObject->particleSystem;
}

Animation* Component::animation()
{
	return gameObject->animation;
}

AudioSource* Component::audio()
{
	return gameObject->audio;
}

FollowText* Component::followText()
{
	return gameObject->followText;
}

FollowTexture* Component::followTexture()
{
	return gameObject->followTexture;
}

Collider* Component::collider()
{
	return gameObject->collider;
}

Rigidbody* Component::rigidbody()
{
	return gameObject->rigidbody;
}

Script* Component::script()
{
	return gameObject->script;
}

ConstantForce* Component::constantForce()
{
	return gameObject->constantForce;
}

TrailRenderer* Component::trailRenderer()
{
	return gameObject->trailRenderer;
}

void Component::applyDescription(ComponentDescMap& mComponentDescMap)
{
	for(auto iter = mComponentDescMap.begin(); iter != mComponentDescMap.end(); iter++) 
	{
		int key = iter->first;
		std::string value = iter->second;

		applyDesc(key, value);
	}

	// não é necessário mais armazenar valores presentes no descritor
	mComponentDescMap.clear();
}