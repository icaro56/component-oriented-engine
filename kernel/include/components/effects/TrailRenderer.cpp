#include "components/effects/TrailRenderer.h"
#include "utils/KernelConverter.h"
#include "engine/Scene.h"
#include "engine/Engine.h"
#include "gameobject/GameObject.h"
#include "components/transform/Transform.h"


TrailRenderer::TrailRenderer()
	: Component("TrailRenderer", "TrailRenderer"),
	trail(nullptr)
{

}

TrailRenderer::TrailRenderer(std::string idComponent)
	: Component(idComponent, "Renderer"),
	trail(nullptr)
{

}

TrailRenderer::TrailRenderer(const ComponentDescMap& d)
	: Component("Renderer", "Renderer"),
	trail(nullptr),
	mTrailDescMap(d)
{

}

TrailRenderer::~TrailRenderer()
{
	if (trail)
	{
		trail->detachFromParent();
		trail = nullptr;
	}
}

void TrailRenderer::setMaxElements(const int max)
{
	if (initialized)
	{
		trail->setMaxChainElements(max);
	}
	else
	{
		mTrailDescMap[TR_MaxElements] = (max);
	}

}
void TrailRenderer::init(const std::string& name)
{
	if (!initialized)
	{
		Ogre::NameValuePairList params;
		params["numberOfChains"] = "120";
		params["maxElements"] = "200";
		trail =(Ogre::RibbonTrail*)GEngine->getCurrentScene()->getOgreScene()->createMovableObject(name + "_RibbonTrail", "RibbonTrail", &params);
		getGameObject()->getParentScene()->getRootTransform()->_attachObject(trail);
		//transform()->_attachObject(trail);

		setAnchor(getGameObject());

		if(trail)
		{
		  initialized = true;
		}
	}
}

void TrailRenderer::setNunberOfChains(const int num)
{
	if (initialized)
	{
		trail->setNumberOfChains(num);
	}
	else
	{
		mTrailDescMap[TR_NumberOfChains] = (num);
	}
}

void TrailRenderer::setMaterial(const std::string mat)
{
	if (initialized)
	{
		trail->setMaterialName(mat);
	}
	else
	{
		mTrailDescMap[TR_MaterialName] = (mat);
	}
}

void TrailRenderer::setInitialColour(const Color color )
{
	if (initialized)
	{
		trail->setInitialColour(color.r,color.g,color.b,color.a);
	}
	else
	{
		mTrailDescMap[TR_InitialColour] = Convert::toString(color);
	}
}

void TrailRenderer::setColourChange(const int size ,const Color color )
{
	if (initialized)
	{
		trail->setColourChange(size,color.r,color.g,color.b,color.a);
	}
	else
	{
		mTrailDescMap[TR_ColourChange] = Convert::toString(color);
	}

}

void TrailRenderer::setInitialWidth(const int max )
{
	if (initialized)
	{
		trail->setInitialWidth(0,max);
	}
	else
	{
		mTrailDescMap[TR_InitialWidth] = (max);
	}
}

void TrailRenderer::setTrailLength(const int max)
{
	if (initialized)
	{
		trail->setTrailLength(max);
	}
	else
	{
		mTrailDescMap[TR_TrailLength] = (max);
	}
}

void TrailRenderer::setEnabled(bool enable)
{
	if (initialized)
	{
		trail->setVisible(enable);
	}
	else
	{
		mTrailDescMap[TR_Enabled] = Convert::toString(enable);
	}
}

bool TrailRenderer::isEnabled()
{
	return trail->getVisible();
}

void TrailRenderer::setLayer(unsigned flags)
{
	if (initialized)
	{
		trail->setQueryFlags(flags);
	}
	else
	{
		mTrailDescMap[TR_QueryFlags] = Convert::toString(flags);
	}

}

unsigned TrailRenderer::getLayer()
{
	if (initialized)
	{
		return trail->getQueryFlags();
	}

	return 1;
}

void TrailRenderer::setVisibilityFlags(int flags)
{
	if (initialized)
	{
		trail->setVisibilityFlags(flags);
	}
	else
	{
		mTrailDescMap[TR_VisibilityFlags] = Convert::toString(flags);
	}
}

unsigned TrailRenderer::getVisibilityFlags()
{
	if (initialized)
	{
		return trail->getVisibilityFlags();
	}

	return 1;
}

void TrailRenderer::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
		case TR_MaxElements:
			setMaxElements(Convert::parseInt(value));
			break;

		case TR_NumberOfChains:
			setNunberOfChains(Convert::parseInt(value));
			break;

		case TR_QueryFlags:
			setLayer(Convert::parseInt(value));
			break;

		case TR_VisibilityFlags:
			setVisibilityFlags(Convert::parseInt(value));
			break;

		case TR_MaterialName:
			setMaterial(value);
			break;

		case TR_InitialColour:
			setInitialColour(Convert::parseColor(value));
				break;

		case TR_TrailLength:
			setTrailLength(Convert::parseInt(value));
				break;

		case TR_ColourChange:
			setColourChange(0,Convert::parseColor(value));
				break;

		case TR_InitialWidth:
			setInitialWidth(Convert::parseInt(value));
				break;

		case TR_Enabled:
			setEnabled(Convert::parseBool(value));
				break;
		default:
			break;
	}
}

void TrailRenderer::setAnchor(GameObject*pivot)
{
	 trail->addNode(pivot->transform->_getOgreSceneNode());
}

void TrailRenderer::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}
