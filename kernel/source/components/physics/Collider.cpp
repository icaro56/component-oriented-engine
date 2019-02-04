#include "components/physics/Collider.h"

#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "engine/PhysicsEngine.h"
#include "engine/callback/SimulationEventCallback.h"
#include "transform/Transform.h"
#include "physics/Rigidbody.h"
#include "script/Script.h"

#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxRigidActor.h"
#include "PxShape.h"
#include "PxMaterial.h"
#include "extensions/PxShapeExt.h"
#include "gameobject/LayerMask.h"
#include "gameobject/GameObject.h"
#include "utils/KernelConverter.h"

#include "moc_Collider.cpp"

//-------------------------------------------------------------------------------------------------
Collider::Collider()
	: Component("Collider", "Collider"),
	  pxStaticActor(nullptr),
	  pxShape(nullptr),
	  wasDeleted(false),
	  isStatic(false),
	  wasTrigger(false),
	  wasSceneQuery(true),
	  rayPass(false)
{

}

Collider::Collider(const std::string& idComponent, const std::string& fID)
	: Component(idComponent, fID),
	  pxStaticActor(nullptr),
	  pxShape(nullptr),
	  wasDeleted(false),
	  isStatic(false),
	  wasTrigger(false),
	  wasSceneQuery(true),
	  rayPass(false)
{

}

Collider::~Collider()
{
	destroyCollider();
}

void Collider::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}

	if (!rigidbody())
	{
		if (collider())
		{
			pxStaticActor = collider()->_getStaticActor();
		}
		else
		{
			Vector3 pos = transform()->getPosition();
			pxStaticActor = getGameObject()->getParentScene()->createRigidStatic(physx::PxTransform(Convert::toPx(pos)));
			pxStaticActor->setName(getGameObject()->getName().c_str());
		}
	}
}

void Collider::nextInit()
{
	if (initialized)
	{
		if (mColliderDescMap.size())
		{
			applyDescription(mColliderDescMap);
		}

		pxShape->userData = this;
		setSceneQuery(true);

		isStatic = pxStaticActor ? true : false;

		unsigned layerID = LayerMask::nameToLayerInt(getGameObject()->getLayer());
		unsigned layer = LayerMask::nameToLayer(getGameObject()->getLayer());
		getGameObject()->getParentScene()->getSimulationEventCallback().setupFiltering(pxShape, layer, layerID, true);

		//avisar todos os componentes que precisam conectar os signals emitidos por collider
		informSlotsClients();
	}
}

void Collider::setReportCollision(bool active)
{
	if (initialized)
	{
		physx::PxFilterData filterData = pxShape->getSimulationFilterData();
		filterData.word2 = active;

		pxShape->setSimulationFilterData(filterData);
		pxShape->setQueryFilterData(filterData);
	}
}

bool Collider::getReportCollision()
{
	if (initialized)
	{
		physx::PxFilterData filterData = pxShape->getSimulationFilterData();
		return filterData.word2;
	}

	return true;
}

void Collider::setLayerId(unsigned layerId)
{
	if (initialized)
	{
		physx::PxFilterData filterData = pxShape->getSimulationFilterData();
		filterData.word0 = 1 << layerId;
		filterData.word1 = layerId;

		pxShape->setSimulationFilterData(filterData);
		pxShape->setQueryFilterData(filterData);
	}
}

unsigned Collider::getLayerId()
{
	if (initialized)
	{
		physx::PxFilterData filterData = pxShape->getSimulationFilterData();
		return filterData.word1;
	}

	return 0;
}

unsigned Collider::getLayer()
{
	if (initialized)
	{
		physx::PxFilterData filterData = pxShape->getSimulationFilterData();
		return filterData.word0;
	}

	return 1;
}

void Collider::setRayPass(bool pass)
{
	rayPass = pass;
}

bool Collider::getRayPass()
{
	return rayPass;
}

void Collider::reset()
{
	//rever paramentros já configurados após usar esse reset

	destroyCollider();
	initialized = false;
	prerequisites();
	init(getGameObject()->getName());
	nextInit();
}

void Collider::destroyCollider()
{
	if (pxShape && !wasDeleted)
	{
		if (isStatic)
		{
			if (pxStaticActor)
				pxStaticActor->detachShape(*pxShape);
		}
		else
		{
			physx::PxRigidActor* a = pxShape->getActor();

			if (a)
				a->detachShape(*pxShape);
		}
		
		pxShape = nullptr;
		wasDeleted = true;
	}

	if (pxStaticActor)
	{
		pxStaticActor->release();
		pxStaticActor = nullptr;
	}
}

physx::PxBounds3 Collider::getBounds()
{
	physx::PxBounds3 bounds;

	if (initialized)
	{
		if (rigidbody())
		{
			bounds = physx::PxShapeExt::getWorldBounds(*pxShape, *rigidbody()->getActor(), 1);
		}
		else
		{
			bounds = physx::PxShapeExt::getWorldBounds(*pxShape, *pxStaticActor, 1);
		}
	}

	return bounds;
}

void Collider::setEnabled(bool e)
{
	if (initialized)
	{
		if (wasTrigger)
		{
			pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, e);
			pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		}
		else
		{
			pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, e);
			pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		}

		if (wasSceneQuery)
			setSceneQuery(e);
	}
	else
	{
		mColliderDescMap[CD_Enabled] = Convert::toString(e);
	}
}

bool Collider::isEnabled()
{
	if (initialized)
	{
		if (pxShape->getFlags() & (physx::PxShapeFlag::eTRIGGER_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE))
		{
			return true;
		}
	}

	return true;
}

void Collider::setTrigger(bool e)
{
	wasTrigger = e;
	if (initialized)
	{
		if (e)
		{
			pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		else
		{
			pxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
			pxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		}
	}
	else
	{
		mColliderDescMap[CD_IsTrigger] = Convert::toString(e);
	}
}

bool Collider::isTrigger()
{
	if (initialized)
	{
		if (pxShape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
		{
			return true;
		}
	}

	return false;
}

void Collider::setSceneQuery(bool active)
{
	wasSceneQuery = active;
	if (initialized)
	{
		pxShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, active);
	}
	else
	{
		mColliderDescMap[CD_IsSceneQuery] = Convert::toString(active);
	}
}

bool Collider::isSceneQuery()
{
	if (initialized)
	{
		if (pxShape->getFlags() & physx::PxShapeFlag::eSCENE_QUERY_SHAPE)
		{
			return true;
		}
	}

	return false;
}

void Collider::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case CD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;

	case CD_IsTrigger:
		setTrigger(Convert::parseBool(value));
		break;

	default:
		break;
	}
}

void Collider::setMaterial(physx::PxMaterial* m)
{
	if (initialized)
	{
		pxShape->setMaterials(&m, 1);
	}
}

physx::PxMaterial* Collider::getMaterial()
{
	if (initialized)
	{
		physx::PxMaterial *mat = 0;
		pxShape->getMaterials(&mat, 1);

		if (mat)
		{
			return mat;
		}
	}
	
	return NULL;
}

void Collider::informOnCollisionEnter(Collision &collision)
{
	emit onColisionEnter(collision);
}

void Collider::informOnCollisionStay(Collision& collision)
{
	emit onColisionStay(collision);
}

void Collider::informOnCollisionExit(Collision& collision)
{
	emit onColisionExit(collision);
}

void Collider::informOnTriggerEnter(Collider* collider)
{
	emit onTriggerEnter(collider);
}

void Collider::informOnTriggerExit(Collider* collider)
{
	emit onTriggerExit(collider);
}

void Collider::informSlotsClients()
{
	GameObject::ComponentVector map = getGameObject()->getComponents("Script");

	if (!map.empty())
	{
		for(unsigned i = 0; i < map.size(); ++i)
		//for (auto it = map.begin(); it != map.end(); ++it)
		{
			Component* it = map.at(i);
			if (it)
			{
				Script* s = dynamic_cast<Script*>(it);
				s->updateConnects();
			}
		}
	}
}

bool Collider::_getWasDeleted()
{
	return wasDeleted;
}

void Collider::_setWasDeleted(bool w)
{
	wasDeleted = w;
}

physx::PxRigidStatic * Collider::_getStaticActor()
{
	return pxStaticActor;
}