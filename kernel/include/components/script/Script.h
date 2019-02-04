#ifndef SCRIPT_H
#define SCRIPT_H

#include "KernelPrerequisites.h"
#include "../Component.h"
#include "components/physics/Collider.h"
#include "components/physics/CharacterController.h"
#include <QObject>

class Collider;

class KERNEL_VS_EXPORT Script : public QObject,public Component
{
	Q_OBJECT
public:

	Script();
	Script(std::string idComponent);
	virtual ~Script();

	virtual void prerequisites(){};
	virtual void init(const std::string& name){initialized = true;}
	void nextInit();
	virtual void update() {if (enabled){}}

	virtual void setEnabled(bool enable);
	virtual bool isEnabled();

	void updateConnects();
	void updateConnectByCollider(Collider* c);

public slots:
	virtual void onColisionEnter(Collision& collision);
	virtual void onColisionStay(Collision& collision);
	virtual void onColisionExit(Collision& collision);

	virtual void onTriggerEnter(Collider* otherCollider);
	virtual void onTriggerExit(Collider* otherCollider);

	virtual void onControllerColliderHit(ControllerColliderHit& hit);
	virtual void onControllerControllerHit(ControllerControllerHit& hit);

protected:

	bool enabled;
};

#endif