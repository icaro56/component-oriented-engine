#include "ComponentTemplate.h"
#include "mesh/MeshRenderer.h"
#include "mesh/MeshFilter.h"
#include "physics/BoxCollider.h"
#include "physics/SphereCollider.h"
#include "physics/CapsuleCollider.h"
#include "gameobject/GameObject.h"
#include "transform/Transform.h"

ComponentSetTemplate::ComponentSetTemplate() 
{

}

//----------------------------------------------------------------

BoxPrefab::BoxPrefab()
	: ComponentSetTemplate()
{

}

BoxPrefab::~BoxPrefab()
{

}

void BoxPrefab::makeComponent(GameObject* gameObject)
{
	MeshFilter* filter2 = new MeshFilter();
	filter2->setFileName("box.mesh");
	gameObject->addComponent(filter2);
	gameObject->addComponent(new MeshRenderer());
	gameObject->transform->setLocalScale(Vector3(2,2,2));
	gameObject->addComponent(new BoxCollider());
}

//----------------------------------------------------------------

SpherePrefab::SpherePrefab()
	: ComponentSetTemplate()
{

}

SpherePrefab::~SpherePrefab()
{

}

void SpherePrefab::makeComponent(GameObject* gameObject)
{
	MeshFilter* filter2 = new MeshFilter();
	filter2->setFileName("Sphere02.mesh");
	gameObject->addComponent(filter2);
	gameObject->addComponent(new MeshRenderer());
	gameObject->addComponent(new SphereCollider());
}

//----------------------------------------------------------------

CapsulePrefab::CapsulePrefab()
	: ComponentSetTemplate()
{

}

CapsulePrefab::~CapsulePrefab()
{

}

void CapsulePrefab::makeComponent(GameObject* gameObject)
{
	MeshFilter* filter2 = new MeshFilter();
	filter2->setFileName("capsule.mesh");
	gameObject->addComponent(filter2);
	gameObject->addComponent(new MeshRenderer());
	gameObject->addComponent(new CapsuleCollider());
}
