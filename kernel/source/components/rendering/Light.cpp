#include "rendering/Light.h"

#include "engine/Engine.h"
#include "engine/Scene.h"
#include "gameobject/GameObject.h"
#include "gameobject/LayerMask.h"
#include "components/transform/Transform.h"

#include "OgreSceneManager.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"
#include "OgreStringConverter.h"

#include "rendering/Camera.h"

#include "utils/KernelConverter.h"

Light::Light()
	: Component("Light", "Light"),
	  mLight(nullptr),
	  billboardSet(nullptr),
	  billboard(nullptr)
{
	
}

Light::Light(const ComponentDescMap& d)
	: Component("Light", "Light"),
	  mLight(nullptr),
	  billboardSet(nullptr),
	  billboard(nullptr),
	  mLightDescMap(d)
{
	
}

Light::~Light()
{
	if (mLight)
	{
		mLight->detachFromParent();
		getGameObject()->getParentScene()->getOgreScene()->destroyLight(mLight);
		mLight = nullptr;
	}
 
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

void Light::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void Light::init(const std::string& name)
{
	if (!initialized)
	{
		mLight = getGameObject()->getParentScene()->getOgreScene()->createLight(name + "_Light");
		//direção padrão da luz é Z positivo, por isso invertemos para o padrão restantes dos objetos da ogre
		mLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);

		unsigned layer = LayerMask::nameToLayer(getGameObject()->getLayer());
		setLayer(layer);

		if (transform())
		{
			transform()->_attachObject(mLight);

			initialized = true;

			if (mLightDescMap.size())
				applyDescription(mLightDescMap);
		}
	}
}

//----------------------------------------------------------
void Light::setType(int type)
{
	if (initialized)
	{
		mLight->setType(Ogre::Light::LightTypes(type));
	}
	else
	{
		mLightDescMap[LD_LightType] = Convert::toString(type);
	}
}

//----------------------------------------------------------
void Light::setDiffuseColor(double r, double g, double b)
{
	setDiffuseColor(Color(r, g, b));
}

void Light::setDiffuseColor(const Color& c)
{
	if (initialized)
	{
		mLight->setDiffuseColour(Convert::toOgre(c));
		if (billboard)
		{
			billboard->setColour(Convert::toOgre(c));
		}
	}
	else
	{
		mLightDescMap[LD_DiffuseColor] = Convert::toString(c);
	}
}

//----------------------------------------------------------
void Light::setSpecularColor(double r, double g, double b)
{
	setSpecularColor(Color(r, g, b));
}

void Light::setSpecularColor(const Color& c)
{
	if (initialized)
	{
		mLight->setSpecularColour(Convert::toOgre(c));
		if (billboard)
		{
			billboard->setColour(Convert::toOgre(c));
		}
	}
	else
	{
		mLightDescMap[LD_SpecularColor] = Convert::toString(c);
	}
}

//----------------------------------------------------------
void Light::setSpotlightAngles(double inner, double outer)
{
	if (initialized)
	{
		mLight->setSpotlightRange(Ogre::Degree(inner), Ogre::Degree(outer));
	}
	else
	{
		mLightDescMap[LD_SpotInnerAngle] = Convert::toString(inner);
		mLightDescMap[LD_SpotOuterAngle] = Convert::toString(outer);
	}
}

void Light::setSpotlightInnerAngle(double inner)
{
	if (initialized)
	{
		mLight->setSpotlightInnerAngle(Ogre::Degree(inner));
	}
	else
	{
		mLightDescMap[LD_SpotInnerAngle] = Convert::toString(inner);
	}
}

void Light::setSpotlightOuterAngle(double outer)
{
	if (initialized)
	{
		mLight->setSpotlightOuterAngle(Ogre::Degree(outer));
	}
	else
	{
		mLightDescMap[LD_SpotOuterAngle] = Convert::toString(outer);
	}
}

//----------------------------------------------------------
void Light::setEnabled(bool enable)
{
	if (initialized)
	{
		mLight->setVisible(enable);
		if(billboardSet)
		{
			billboardSet->setVisible(enable);
		}
	}
	else
	{
		mLightDescMap[LD_Enabled] = Convert::toString(enable);
	}
}

//----------------------------------------------------------
bool Light::isEnabled()
{
	if (initialized)
		return mLight->getVisible();

	return true;
}

//----------------------------------------------------------
bool Light::isVisible()
{
	bool visible = false;
	for(int i = 0; i < Camera::allCameras.size(); ++i)
	{
		
		if (Camera::allCameras.at(i)->isOnView(Convert::toKernel(mLight->getBoundingBox())))
		{
			visible = true;
			break;
		}
	}

	return visible;
}

//----------------------------------------------------------
void Light::setBillboard(const float width, const float height,
                         const Color &color,
                         const std::string &materialName)
{
	if (initialized)
	{
		billboardSet = getGameObject()->getParentScene()->getOgreScene()->createBillboardSet(gameObject->getName() + std::string("lightbbs"), 1);
		//billboardSet->setMaterialName("Examples/Flare");
		billboardSet->setMaterialName(materialName);
		billboard = billboardSet->createBillboard(0,0,0,Convert::toOgre(color));
		billboard->setDimensions(width, height);

		mLight->getParentSceneNode()->attachObject(billboardSet);

		billboardSet->setQueryFlags(mLight->getQueryFlags());
	}
	else
	{
		mLightDescMap[LD_BillboardWidth] = Convert::toString(width);
		mLightDescMap[LD_BillboardHeight] = Convert::toString(height);
		mLightDescMap[LD_BillboardColor] = Convert::toString(color);
		mLightDescMap[LD_BillboardMaterial] = materialName;
		mLightDescMap[LD_BillboardCreate] = Convert::toString(true);
	}
}

void Light::setVisibilityFlags(int flags)
{
	if (initialized)
	{
		mLight->setVisibilityFlags(flags);
	}
	else
	{
		mLightDescMap[LD_VisibilityFlags] = Convert::toString(flags);
	}
}

void Light::setCastShadows(bool enable)
{
	if (initialized)
	{
		mLight->setCastShadows(enable);
	}
	else
	{
		mLightDescMap[LD_CastShadows] = Convert::toString(enable);
	}
}

bool Light::getCastShadows()
{
	return mLight->getCastShadows();
}

void Light::setAttenuation(double range, double constant, double linear, double quadratic)
{
	mLight->setAttenuation(range, constant, linear, quadratic);
}

void Light::setSpotlightNearClipDistance(double r)
{
	mLight->setSpotlightNearClipDistance(r);
}

void Light::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case LD_LightType:
		setType(Convert::parseInt(value));
		break;

	case LD_DiffuseColor:
		setDiffuseColor(Convert::parseColor(value));
		break;

	case LD_SpecularColor:
		setSpecularColor(Convert::parseColor(value));
		break;

	case LD_SpotInnerAngle:
		setSpotlightInnerAngle(Convert::parseReal(value));
		break;

	case LD_SpotOuterAngle:
		setSpotlightOuterAngle(Convert::parseReal(value));
		break;

	case LD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;

	case LD_BillboardCreate:
		setBillboard(Convert::parseReal(mLightDescMap.at(LD_BillboardWidth)), 
					 Convert::parseReal(mLightDescMap.at(LD_BillboardHeight)), 
					 Convert::parseColor(mLightDescMap.at(LD_BillboardColor)), 
				  	 mLightDescMap.at(LD_BillboardMaterial));
		break;

	case LD_VisibilityFlags:
		setVisibilityFlags(Convert::parseInt(value));
		break;

	case LD_CastShadows:
		setCastShadows(Convert::parseBool(value));
		break;
	
	case LD_QueryFlags:
		setLayer(Convert::parseUnsignedInt(value));
		break;

	default:
		break;
	}
}

void Light::setLayer(unsigned flags)
{
	if (initialized)
	{
		mLight->setQueryFlags(flags);

		if (billboardSet)
			billboardSet->setQueryFlags(flags);
	}
	else
	{
		mLightDescMap[LD_QueryFlags] = Convert::toString(flags);
	}
}

unsigned Light::getLayer()
{
	if (initialized)
	{
		return mLight->getQueryFlags();
	}

	return 1;
}