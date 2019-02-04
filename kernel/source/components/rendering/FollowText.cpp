#include "components\rendering\FollowText.h"
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

FollowText::FollowText()
	: Component("FollowText", "FollowText"),
	  mColor(1.0, 1.0, 1.0),
	  mCamera(nullptr)
{
}

FollowText::FollowText(const ComponentDescMap & d)
	: Component("FollowText", "FollowText"),
	  mColor(1.0, 1.0, 1.0),
	  mCamera(nullptr),
	  folowTextDescMap(d)
{

}

FollowText::~FollowText(void)
{
    Ogre::OverlayManager *overlayManager =
    Ogre::OverlayManager::getSingletonPtr();
    overlayManager->destroyOverlayElement(mElementText);
    overlayManager->destroyOverlayElement(mContainer);
    overlayManager->destroy(mOverlay);
}

void FollowText::destroy()
{
	Ogre::OverlayManager *overlayManager =
    Ogre::OverlayManager::getSingletonPtr();
	overlayManager->destroyOverlayElement(mContainer);
    overlayManager->destroy(mOverlay);
}

void FollowText::setEnabled(bool enabled)
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
		folowTextDescMap[FTD_Enabled] = Convert::toString(enabled);
	}
}

bool FollowText::isEnabled()
{
	if (initialized)
		return mOverlay->isVisible();

	return true;
}

void FollowText::setText(const std::string &text)
{

	if (initialized)
	{
	
		mElementText->setCaption((text));
	}
	else
	{
		folowTextDescMap[FTD_Text] = text;
	}
}

void FollowText::update()
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

void FollowText::setColor(const float r, const float g, const float b)
{
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
    mElementText->setColour(Ogre::ColourValue(r,g,b));
}

void FollowText::setCamera(Camera* cam)
{
    mCamera = cam;
}

Vector3 FollowText::getColor()
{
    return mColor;
}

void FollowText::setTextAlign(Ogre::TextAreaOverlayElement::Alignment a)
{
    ((Ogre::TextAreaOverlayElement*)mElementText)->setAlignment(a);
}

Ogre::TextAreaOverlayElement::Alignment FollowText::setTextAlign()
{
    return ((Ogre::TextAreaOverlayElement*)mElementText)->getAlignment();
}

void FollowText::setHeight(float height)
{
	if(initialized)
	{
		((Ogre::TextAreaOverlayElement*)mElementText)->setCharHeight(height);
	}
	else
	{
		folowTextDescMap[FTD_Height] = Convert::toString(height);
	}
}

float FollowText::getHeight()
{
    return ((Ogre::TextAreaOverlayElement*)mElementText)->getCharHeight();
}

double FollowText::getProjectionX()
{
	return projX;
}

double FollowText::getProjectionY()
{
	return projY;
}

void FollowText::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case FTD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;

	case FTD_Text:
		setText(value);
		break;

	case FTD_Height:
		setHeight(Convert::parseReal(value));
		break;

	default:
		break;
	}
}

void FollowText::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void FollowText::init(const std::string& name)
{
	if (!initialized)
	{
		// create an overlay that we can use for later
		mOverlay = Ogre::OverlayManager::getSingleton().create(name);
		mContainer = static_cast<Ogre::OverlayContainer*>(
						 Ogre::OverlayManager::getSingleton().createOverlayElement(
							 "Panel", name + "_container"));

		mOverlay->add2D(mContainer);

		mElementText =
			Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea",
					name + "_element");
		mElementText->setDimensions(1.0, 1.0);
		mElementText->setMetricsMode(Ogre::GMM_PIXELS);
		mElementText->setPosition(0, 0);

		mElementText->setParameter("font_name", "blue16");
		mElementText->setParameter("char_height", "16");
		mElementText->setParameter("horz_align", "center");
		mElementText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0));

		mContainer->addChild(mElementText);
		mOverlay->show();

		initialized = true;

		if (folowTextDescMap.size())
		{
			applyDescription(folowTextDescMap);
		}
	}
}