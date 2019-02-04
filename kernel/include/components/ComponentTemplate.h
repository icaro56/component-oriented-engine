
#ifndef __COMPONENTTEMPLATE_HPP__
#define __COMPONENTTEMPLATE_HPP__

#include "KernelPrerequisites.h"
#include "Component.h"

class GameObject;

class KERNEL_VS_EXPORT ComponentSetTemplate 
{
public:
	ComponentSetTemplate();
	virtual ~ComponentSetTemplate(){};

	// returns the ComponentID that, by default, we should
	// register created Component as
	virtual const std::string ID() const = 0;

	virtual void makeComponent(GameObject* gameObject) = 0;
};

class KERNEL_VS_EXPORT BoxPrefab : public ComponentSetTemplate
{
public:
	BoxPrefab();
	virtual ~BoxPrefab();

	// returns the ComponentID that, by default, we should
	// register created Component as
	virtual const std::string ID() const {return "BoxPrefab";};

	virtual void makeComponent(GameObject* gameObject);
};

class KERNEL_VS_EXPORT SpherePrefab : public ComponentSetTemplate
{
public:
	SpherePrefab();
	virtual ~SpherePrefab();

	// returns the ComponentID that, by default, we should
	// register created Component as
	virtual const std::string ID() const {return "SpherePrefab";};

	virtual void makeComponent(GameObject* gameObject);
};

class KERNEL_VS_EXPORT CapsulePrefab : public ComponentSetTemplate
{
public:
	CapsulePrefab();
	virtual ~CapsulePrefab();

	// returns the ComponentID that, by default, we should
	// register created Component as
	virtual const std::string ID() const {return "CapsulePrefab";};

	virtual void makeComponent(GameObject* gameObject);
};



#endif
