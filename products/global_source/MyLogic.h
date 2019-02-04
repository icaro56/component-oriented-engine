#include "Logic.h"

#include "components/physics/Collider.h"

class MyLogic : public Logic
{
public:
	MyLogic();
	virtual ~MyLogic();

	virtual void init();
	virtual void updateLogic();
	Ogre::SceneNode *node;
	Ogre::RibbonTrail *trail;

private:

	GameObject* go, *CCobject;
};