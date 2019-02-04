
#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "KernelPrerequisites.h"
#include <string>

class GameObject;

class Transform;
class Camera;
class Light;
class Renderer;
class ParticleSystem;
class Animation;
class AudioSource;
class Billboard;
class FollowText;
class FollowTexture;
class BoundingBox;
class Collider;
class Rigidbody;
class Script;
class ConstantForce;
class TrailRenderer;

class KERNEL_VS_EXPORT Component 
{
public:
	//used to save data before component's initialization. 
	typedef std::map<unsigned, std::string> ComponentDescMap;

	Component();
	Component(std::string idComponent, std::string idFamily);
	virtual ~Component();

	virtual void prerequisites() = 0;
	virtual void init(const std::string& name) = 0;
	virtual void nextInit() = 0;

	virtual void setComponentID(std::string id);
	virtual const std::string& componentID() const;

	virtual void setFamilyID(std::string id);
	virtual const std::string& familyID() const;

	virtual void update();
	virtual void fixedUpdate();
	virtual void lateUpdate();

	virtual void setEnabled(bool enable) = 0;
	virtual bool isEnabled() = 0;

	virtual void setGameObject( GameObject* obj);
	GameObject* getGameObject() const;

	// atalhos para todos outros componentes disponíveis, null se componente não existe
	Transform* transform();
	Camera* camera(); 
	Light* light();
	Renderer* renderer();
	ParticleSystem* particleSystem();
	Animation* animation();
	AudioSource* audio();
	Billboard* billboard();
	FollowText* followText();
	FollowTexture* followTexture();
	BoundingBox* boundingBox();
	Collider* collider();
	Rigidbody* rigidbody();
	Script* script();
	ConstantForce* constantForce();
	TrailRenderer* trailRenderer();

protected:
	virtual void applyDescription(ComponentDescMap& mComponentDescMap);
	virtual void applyDesc(int key, const std::string& value) {};

protected:
	GameObject *gameObject;

	std::string mComponentId;
	std::string mFamilyId;

	bool initialized;
};

#endif
