#include "rendering/Billboard.h"
#include "engine/Engine.h"
#include "engine/Scene.h"
#include "gameobject/GameObject.h"
#include "gameobject/LayerMask.h"
#include "components/transform/Transform.h"

#include "OgreSceneManager.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"

#include "rendering/Camera.h"

#include "utils/KernelConverter.h"
//----------------------------------------------------------
Billboard::Billboard()
	: Component("Billboard", "Billboard"),
	billboardSet(nullptr),
	billboard(nullptr)
{

}
//----------------------------------------------------------
Billboard::Billboard(const ComponentDescMap& d)
	: Component("Billboard", "Billboard"),
	billboardSet(nullptr),
	billboard(nullptr),
	mBillboardDescMap(d)
{

}
//----------------------------------------------------------
Billboard::~Billboard()
{
	if (billboardSet)
	{
		billboardSet->removeBillboard(billboard);
		billboardSet->detachFromParent();
		getGameObject()->getParentScene()->getOgreScene()->destroyBillboardSet(billboardSet);
		billboardSet = nullptr;
	}

	if (billboard)
	{
		delete billboard;
		billboard = nullptr;
	}

}
//----------------------------------------------------------
void Billboard::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}
//----------------------------------------------------------
void Billboard::init(const std::string& name)
{
	if (!initialized)
	{
		billboardSet = getGameObject()->getParentScene()->getOgreScene()->createBillboardSet(name+"_Billboard");
		billboard = billboardSet->createBillboard(0,0,0);

		unsigned layer = LayerMask::nameToLayer(getGameObject()->getLayer());
		setLayer(layer);
	
		if (transform())
		{
			transform()->_attachObject(billboardSet);

			initialized = true;

			if (mBillboardDescMap.size())
			{
				applyDescription(mBillboardDescMap);
			}
		}
	}
}
//----------------------------------------------------------
void Billboard::setType(int type)
{
	if (initialized)
	{
		billboardSet->setBillboardType(Ogre::BillboardType(type));
	}
	else
	{
		mBillboardDescMap[BD_Type] = Convert::toString(type);
	}
}
//----------------------------------------------------------
void Billboard::setColor(double r, double g, double b)
{
	setColor(Color(r, g, b));
}
//----------------------------------------------------------
void Billboard::setColor(const Color& c)
{
	if (initialized)
	{
		 billboard->setColour(Convert::toOgre(c));
	}
	else
	{
		mBillboardDescMap[BD_Color] = Convert::toString(c);
	}
}
//----------------------------------------------------------
void Billboard::setEnabled(bool enable)
{
	if (initialized)
	{
		billboardSet->setVisible(enable);
		if(billboardSet)
		{
			billboardSet->setVisible(enable);
		}
	}
	else
	{
		mBillboardDescMap[BD_Enabled] = Convert::toString(enable);
	}
}

//----------------------------------------------------------
bool Billboard::isEnabled()
{
	return billboardSet->getVisible();
}

//----------------------------------------------------------
bool Billboard::isVisible()
{
	bool visible = false;
	for(int i = 0; i < Camera::allCameras.size(); ++i)
	{

		if (Camera::allCameras.at(i)->isOnView(Convert::toKernel(billboardSet->getBoundingBox())))
		{
			visible = true;
			break;
		}
	}

	return visible;
}

void Billboard::setDimensions(const Vector2& dim)
{
	if (initialized)
	{
		billboard->setDimensions(dim.x, dim.y);
	}
	else
	{
		mBillboardDescMap[BD_Dimension] = Convert::toString(dim);
	}
}

//----------------------------------------------------------
void Billboard::setDimensions(double w,double h)
{
	setDimensions(Vector2(w,h));
}

//---------------------------------------------------------
void Billboard::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case BD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;

	case BD_Type:
		setType(Convert::parseInt(value));
		break;

	case BD_Color:
		setColor(Convert::parseColor(value));
		break;

	case BD_Material:
		setMaterial(value);
		break;

	case BD_Dimension:
		setDimensions(Convert::parseVector2(value));
		break;
	
	case BD_QueryFlags:
		setLayer(Convert::parseUnsignedInt(value));
		break;

	default:
		break;
	}
}

void Billboard::setMaterial(std::string name)
{
	if (initialized)
	{
		billboardSet->setMaterialName(name);
	}
	else
	{
		mBillboardDescMap[BD_Material] = name;
	}
}

void Billboard::setOrigin(int type)
{
	if (initialized)
	{
		billboardSet->setBillboardOrigin(Ogre::BillboardOrigin(type));
	}
	else
	{
		mBillboardDescMap[BD_Origin] = Convert::toString(type);
	}
}

void Billboard::setLayer(unsigned flags)
{
	if (initialized)
	{
		billboardSet->setQueryFlags(flags);
	}
	else
	{
		mBillboardDescMap[BD_QueryFlags] = Convert::toString(flags);
	}
}

unsigned Billboard::getLayer()
{
	if (initialized)
	{
		return billboardSet->getQueryFlags();
	}

	return 1;
}

