#include "physics/ConstantForce.h"
#include "physics/Rigidbody.h"
#include "gameobject/GameObject.h"
#include "time/Time.h"

ConstantForce::ConstantForce()
	: Component("ConstantForce", "ConstantForce"),
	enabled(true)
{
	defaultValues();
}

ConstantForce::ConstantForce(std::string idComponent)
	: Component(idComponent, "ConstantForce"),
	enabled(true)
{
	defaultValues();
}

ConstantForce::~ConstantForce()
{
	defaultValues();
	initialized = false;
}

void ConstantForce::prerequisites()
{
	if (!rigidbody())
	{
		getGameObject()->addComponent(new Rigidbody());
	}
}

void ConstantForce::init(const std::string& name)
{
	if (!initialized)
	{
		initialized = true;
	}
}

void ConstantForce::nextInit()
{

}

void ConstantForce::fixedUpdate()
{
	//passar para o fixed update
	if (initialized && isEnabled() && rigidbody())
	{
		if (!force.positionEquals(Vector3::ZERO))
		{
			rigidbody()->addForce(force);
		}

		if (!relativeForce.positionEquals(Vector3::ZERO))
		{
			rigidbody()->addRelativeForce(relativeForce);
		}

		if (!torque.positionEquals(Vector3::ZERO))
		{
			rigidbody()->addTorque(torque);
		}

		if (!relativeTorque.positionEquals(Vector3::ZERO))
		{
			rigidbody()->addRelativeTorque(relativeTorque);
		}
	}
}

void ConstantForce::setEnabled(bool e)
{
	enabled = e;
}

bool ConstantForce::isEnabled()
{
	return enabled;
}

void ConstantForce::setForce(const Vector3& f)
{
	force = f;
}

Vector3 ConstantForce::getForce()
{
	return force;
}

void ConstantForce::setRelativeForce(const Vector3& f)
{
	relativeForce = f;
}

Vector3 ConstantForce::getRelativeForce()
{
	return relativeForce;
}

void ConstantForce::setTorque(const Vector3& f)
{
	torque = f;
}

Vector3 ConstantForce::getTorque()
{
	return torque;
}

void ConstantForce::setRelativeTorque(const Vector3& f)
{
	relativeTorque = f;
}

Vector3 ConstantForce::getRelativeTorque()
{
	return relativeTorque;
}

void ConstantForce::defaultValues()
{
	force = Vector3::ZERO;
	relativeForce = Vector3::ZERO;
	torque = Vector3::ZERO;
	relativeTorque = Vector3::ZERO;
}