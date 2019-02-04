#include "script/Script.h"
#include "physics/Collider.h"
#include "gameobject/GameObject.h"

#include "moc_Script.cpp"

Script::Script()
	: Component("Script", "Script"),
	enabled(true)
{

}

Script::Script(std::string idComponent)
	: Component(idComponent, "Script"),
	enabled(true)
{

}

Script::~Script()
{

}

void Script::nextInit()
{
	updateConnects();
}

void Script::setEnabled(bool enable)
{
	enabled = enable;
}

bool Script::isEnabled()
{
	return enabled;
}

void Script::updateConnects()
{
	GameObject::ComponentVector map = getGameObject()->getComponents("Collider");
	if (!map.empty())
	{
		for(unsigned i = 0; i < map.size(); ++i)
		//for (auto it = map.begin(); it != map.end(); ++it)
		{
			Component* aux = map.at(i);
			if (aux)
			{
				Collider* c = dynamic_cast<Collider*>(aux);
		
				connect(c,
					SIGNAL(onColisionEnter(Collision&)),
					this,
					SLOT(onColisionEnter(Collision&)));

				connect(c,
					SIGNAL(onColisionStay(Collision&)),
					this,
					SLOT(onColisionStay(Collision&)));

				connect(c,
					SIGNAL(onColisionExit(Collision&)),
					this,
					SLOT(onColisionExit(Collision&)));

				connect(c,
					SIGNAL(onTriggerEnter(Collider*)),
					this,
					SLOT(onTriggerEnter(Collider*)));

				connect(c,
					SIGNAL(onTriggerExit(Collider*)),
					this,
					SLOT(onTriggerExit(Collider*)));
			}
		}
	}

	//CharacterController
	GameObject::ComponentVector map2 = getGameObject()->getComponents("CharacterController");
	if (!map2.empty())
	{
		for(unsigned i = 0; i < map2.size(); ++i)
			//for (auto it = map.begin(); it != map.end(); ++it)
		{
			Component* aux = map2.at(i);
			if (aux)
			{
				CharacterController* c = dynamic_cast<CharacterController*>(aux);

				connect(c,
					SIGNAL(onControllerColliderHit(ControllerColliderHit&)),
					this,
					SLOT(onControllerColliderHit(ControllerColliderHit&)));

				connect(c,
					SIGNAL(onControllerControllerHit(ControllerControllerHit& )),
					this,
					SLOT(onControllerControllerHit(ControllerControllerHit& )));
			}
		}
	}
}

void Script::updateConnectByCollider(Collider* c)
{
	if (c)
	{
		connect(c,
			SIGNAL(onColisionEnter(Collision&)),
			this,
			SLOT(onColisionEnter(Collision&)));

		connect(c,
			SIGNAL(onColisionStay(Collision&)),
			this,
			SLOT(onColisionStay(Collision&)));

		connect(c,
			SIGNAL(onColisionExit(Collision&)),
			this,
			SLOT(onColisionExit(Collision&)));

		connect(c,
			SIGNAL(onTriggerEnter(Collider*)),
			this,
			SLOT(onTriggerEnter(Collider*)));

		connect(c,
			SIGNAL(onTriggerExit(Collider*)),
			this,
			SLOT(onTriggerExit(Collider*)));
	}
}

void Script::onColisionEnter(Collision& collision)
{
	std::cerr << "\nCollision Enter";
}

void Script::onColisionStay(Collision& collision)
{
	std::cerr << "\nCollision Stay";
}

void Script::onColisionExit(Collision& collision)
{
	std::cerr << "\nCollision Exit";
}

void Script::onTriggerEnter(Collider* otherCollider)
{
	std::cerr << "\nTrigger Enter";
}

void Script::onTriggerExit(Collider* otherCollider)
{
	std::cerr << "\nTrigger Exit";
}

void Script::onControllerColliderHit(ControllerColliderHit& hit)
{
	std::cerr << "\nController colidiu com Collider";
}

void Script::onControllerControllerHit(ControllerControllerHit& hit)
{
	std::cerr << "\nController colidiu com Controller";
}