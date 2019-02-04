#include "rendering/BoundingBox.h"

#include "gameobject/GameObject.h"
#include "gameobject/LayerMask.h"
#include "components/mesh/Renderer.h"
#include "components/transform/Transform.h"
#include "components/physics/Collider.h"
#include "utils/KernelConverter.h"

BoundingBox::BoundingBox()
	: Component("BoundingBox", "BoundingBox"),
	  mOrientedBoundingBox(nullptr)
{

}

BoundingBox::BoundingBox(const ComponentDescMap& d)
	: Component("BoundingBox", "BoundingBox"),
	  mOrientedBoundingBox(nullptr),
	  mBoundingBoxDescMap(d)
{

}

BoundingBox::~BoundingBox()
{
	if (mOrientedBoundingBox)
	{
		mOrientedBoundingBox->detachFromParent();
		delete mOrientedBoundingBox;
		mOrientedBoundingBox = nullptr;
	}
}

void BoundingBox::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void BoundingBox::init(const std::string& name)
{
	if (!initialized)
	{
		if (renderer())
		{
			mOrientedBoundingBox = new OrientedBoundingBox();
			mOrientedBoundingBox->setupVertices(Convert::toOgre(renderer()->getBounds()));
			transform()->_attachObject(mOrientedBoundingBox);

			unsigned layer = LayerMask::nameToLayer(getGameObject()->getLayer());
			setLayer(layer);

			initialized = true;

			if (mBoundingBoxDescMap.size())
				applyDescription(mBoundingBoxDescMap);
		}
		else if (collider())
		{
			mOrientedBoundingBox = new OrientedBoundingBox();
			mOrientedBoundingBox->setupVertices(Convert::toOgre(collider()->getBounds()), Convert::toOgre(-transform()->getPosition()));
			transform()->_attachObject(mOrientedBoundingBox);

			unsigned layer = LayerMask::nameToLayer(getGameObject()->getLayer());
			setLayer(layer);

			initialized = true;

			if (mBoundingBoxDescMap.size())
				applyDescription(mBoundingBoxDescMap);
		}

	}
}

void BoundingBox::setColor(const Color& c)
{
	if(initialized)
	{
		mOrientedBoundingBox->setColor(Convert::toOgre(c));
	}
	else
	{
		mBoundingBoxDescMap[BBD_Color] = Convert::toString(c);
	}
}

Color BoundingBox::getColor()
{
	return Convert::toKernel(mOrientedBoundingBox->getColor());
}

void BoundingBox::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case BBD_Color:
		setColor(Convert::parseColor(value));
		break;
	
	case BBD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;
	
	case BBD_QueryFlags:
		setLayer(Convert::parseUnsignedInt(value));
		break;

	default:
		break;
	}
}

void BoundingBox::setEnabled(bool enable)
{
	if (initialized)
	{
		mOrientedBoundingBox->setVisible(enable);
	}
	else
	{
		mBoundingBoxDescMap[BBD_Enabled] = Convert::toString(enable);
	}
}

bool BoundingBox::isEnabled()
{
	if (initialized)
	{
		return mOrientedBoundingBox->getVisible();
	}

	return true;
}

void BoundingBox::setLayer(unsigned flags)
{
	if (initialized)
	{
		mOrientedBoundingBox->setQueryFlags(flags);
	}
	else
	{
		mBoundingBoxDescMap[BBD_QueryFlags] = Convert::toString(flags);
	}
}

unsigned BoundingBox::getLayer()
{
	if (initialized)
	{
		return mOrientedBoundingBox->getQueryFlags();
	}

	return 1;
}