#include "mesh/Renderer.h"

#include "OgreEntity.h"

#include "engine/Engine.h"
#include "engine/Scene.h"
#include "gameobject/GameObject.h"
#include "transform/Transform.h"
#include "mesh/MeshFilter.h"
#include "rendering/Camera.h"
#include "gameobject/LayerMask.h"

#include "utils/KernelConverter.h"

Renderer::Renderer()
	: Component("Renderer", "Renderer"),
	  mEntity(nullptr)
{

}

Renderer::Renderer(std::string idComponent)
	: Component(idComponent, "Renderer"),
	  mEntity(nullptr)
{

}

Renderer::Renderer(const ComponentDescMap& d)
	: Component("Renderer", "Renderer"),
	  mEntity(nullptr),
	  mRendererDescMap(d)
{

}

Renderer::~Renderer()
{
	if (mEntity)
	{
		mEntity->detachFromParent();
		getGameObject()->getParentScene()->getOgreScene()->destroyEntity (mEntity);

		mEntity = nullptr;
	}
}

void Renderer::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void Renderer::init(const std::string& name)
{
	if (!initialized)
	{
		if (getGameObject()->getComponent("MeshFilter"))
		{
			MeshFilter* meshFilter = dynamic_cast<MeshFilter*>(getGameObject()->getComponent("MeshFilter"));

			Ogre::Mesh* mesh = meshFilter->getMesh();

			if (mesh)
			{
				mEntity = getGameObject()->getParentScene()->getOgreScene()->createEntity(name + "_Renderer", mesh->getName());
		
				if (transform())
				{
					transform()->_attachObject(mEntity);

					initialized = true;

					if (mRendererDescMap.size())
						applyDescription(mRendererDescMap);
				}
			}
		}
		else if (mEntity)
		{
			if (transform())
			{
				transform()->_attachObject(mEntity);

				initialized = true;

				if (mRendererDescMap.size())
					applyDescription(mRendererDescMap);
			}
		}

		if (mEntity)
		{
			unsigned layer = LayerMask::nameToLayer(getGameObject()->getLayer());
			setLayer(layer);
		}
	}
}

void Renderer::setEnabled(bool enable)
{
	if (initialized)
	{
		mEntity->setVisible(enable);
	}
	else
	{
		mRendererDescMap[RD_Enabled] = Convert::toString(enable);
	}
}

bool Renderer::isEnabled()
{
	return mEntity->getVisible();
}

bool Renderer::isVisible()
{
	bool visible = false;
	for(int i = 0; i < Camera::allCameras.size(); ++i)
	{
		if (Camera::allCameras.at(i)->isOnView(getBounds()))
		{
			visible = true;
			break;
		}
	}

	return visible;
}

const Bounds Renderer::getBounds() const
{
	return Convert::toKernel(mEntity->getBoundingBox());
}

void Renderer::setCastShadows(bool enable)
{
	if (initialized)
	{
		mEntity->setCastShadows(enable);
	}
	else
	{
		mRendererDescMap[RD_CastShadows] = Convert::toString(enable);
	}
}

void Renderer::setLayer(unsigned flags)
{
	if (initialized)
	{
		mEntity->setQueryFlags(flags);
	}
	else
	{
		mRendererDescMap[RD_QueryFlags] = Convert::toString(flags);
	}
}

unsigned Renderer::getLayer()
{
	if (initialized)
	{
		return mEntity->getQueryFlags();
	}

	return 1;
}

void Renderer::setVisibilityFlags(int flags)
{
	if (initialized)
	{
		mEntity->setVisibilityFlags(flags);
	}
	else
	{
		mRendererDescMap[RD_VisibilityFlags] = Convert::toString(flags);
	}
}

unsigned Renderer::getVisibilityFlags()
{
	if (initialized)
	{
		return mEntity->getVisibilityFlags();
	}

	return 1;
}

void Renderer::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case RD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;

	case RD_CastShadows:
		setCastShadows(Convert::parseBool(value));
		break;

	case RD_QueryFlags:
		setLayer(Convert::parseInt(value));
		break;

	case RD_VisibilityFlags:
		setVisibilityFlags(Convert::parseInt(value));
		break;

	case RD_MaterialName:
		setMaterialName(value);
		break;

	default:
		break;
	}
}

void Renderer::setMaterialName(const std::string& name)
{
	if (initialized)
	{
		mEntity->setMaterialName(name);
	}
	else
	{
		mRendererDescMap[RD_MaterialName] = name;
	}
}

void Renderer::setOgreEntity(Ogre::Entity* ent)
{
	mEntity = ent;
}

Ogre::Entity* Renderer::getOgreEntity()
{
	return mEntity;
}
