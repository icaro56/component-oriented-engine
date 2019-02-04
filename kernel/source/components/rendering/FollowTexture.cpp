#include "components\rendering\FollowTexture.h"

#include "engine/Engine.h"

#include "OgreOverlayContainer.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayManager.h"
#include "OgreWindowEventUtilities.h"

#include "OgreCamera.h"

#include "gameobject/GameObject.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"
#include "utils/KernelConverter.h"

FollowTexture::FollowTexture(void)
	: Component("FollowTexture", "FollowTexture"),
	mCamera(nullptr)

{
}
FollowTexture::FollowTexture(const ComponentDescMap & d)
	: Component("FollowTexture", "FollowTexture"),
	mCamera(nullptr),
	followTextureDescMap(d)
{  
}

FollowTexture::~FollowTexture(void)
{
	Ogre::OverlayManager *overlayManager =
    Ogre::OverlayManager::getSingletonPtr();
	foreach(Ogre::OverlayElement* element, elements)
	{
		overlayManager->destroyOverlayElement(element);
	}
	overlayManager->destroyOverlayElement(mContainer);
    overlayManager->destroy(mOverlay);
}

 void FollowTexture::init(const std::string& name)
{
	if (!initialized)
	{
		
		mName = name.c_str();
		// create an overlay that we can use for later
		mOverlay = Ogre::OverlayManager::getSingleton().create(mName+"FollowTexture");
		mContainer = static_cast<Ogre::OverlayContainer*>(
						 Ogre::OverlayManager::getSingleton().createOverlayElement(
							 "Panel", mName + "_container_TextureFollow"));

		mOverlay->add2D(mContainer);
		
		mOverlay->show();
		initialized = true;

		if (followTextureDescMap.size())
		{
			applyDescription(followTextureDescMap);
		}
	}
	
}
 void FollowTexture::destroy()
{
	Ogre::OverlayManager *overlayManager =
    Ogre::OverlayManager::getSingletonPtr();
	foreach(Ogre::OverlayElement* element, elements)
	{
		overlayManager->destroyOverlayElement(element);
	}
	overlayManager->destroyOverlayElement(mContainer);
    overlayManager->destroy(mOverlay);
}

 void FollowTexture::setEnabled(bool enabled)
{
	if (initialized)
	{
		if (enabled)
		{
			mOverlay->show();
		}
		else
		{
			mOverlay->hide();
		}
	}
	else
	{
		followTextureDescMap[FTXD_Enabled] = Convert::toString(enabled);
	}
}

 bool FollowTexture::isEnabled()
{
	if (initialized)
		return mOverlay->isVisible();

	return true;
}

void FollowTexture::update()
{
    Ogre::WindowEventUtilities::messagePump();
    if (!mOverlay->isVisible() && !mCamera)
        return;

    Vector4 projectedPosition = mCamera->getProjectionMatrix()
                                      * mCamera->getViewMatrix()
                                      * Vector4(transform()->getPosition());
    projectedPosition = projectedPosition/projectedPosition.w;

	mContainer->setPosition(projectedPosition.x * 0.5,
                            1-((projectedPosition.y+1) * 0.5));

    if(projectedPosition.z > 1)
    {
		projX = projY = -1;	

        mOverlay->hide();	
	}
    else
    {
        projX = projectedPosition.x * 0.5f;
		projY = 1 - ((projectedPosition.y+1) * 0.5f);
			
		mOverlay->show();
    }
}

void FollowTexture::setCamera(Camera* cam)
{
     mCamera = cam;
}

double FollowTexture::getProjectionX()
{
	return projX;
}

double FollowTexture::getProjectionY()
{
	return projY;
}

void FollowTexture::addElement(QString name, Ogre::OverlayElement* element)
{
	element->setHorizontalAlignment(Ogre::GHA_CENTER);
	mContainer->addChild(element);
	elements.insert(name, element);
}

Ogre::OverlayElement* FollowTexture::createElement(QString name, QString material, Ogre::Vector2 dimensions, Ogre::Vector2 offset)
{
	if(initialized)
	{
		Ogre::OverlayElement* element =
			Ogre::OverlayManager::getSingleton().createOverlayElement("Panel",
			name.toStdString() + "_element_Texture");

		element->setMetricsMode(Ogre::GMM_PIXELS);
		element->setDimensions(dimensions.x, dimensions.y);
		element->setPosition(offset.x, offset.y);
	
		element->setMaterialName(material.toStdString());
		element->setHorizontalAlignment(Ogre::GHA_CENTER);

		mContainer->addChild(element);
		elements.insert(name, element);

		return element;
	}

	else
	{
		std::cerr<<"Component não fora atribuido a um GameObject";
	}
}


Ogre::OverlayElement* FollowTexture::getElement(QString name)
{
	return elements.value(name);
}

Ogre::OverlayContainer* FollowTexture::getContainer(QString name)
{
	return static_cast<Ogre::OverlayContainer*>(elements.value(name));
}

void FollowTexture::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void FollowTexture::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case FTXD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;
	default:
		break;
	}
}
