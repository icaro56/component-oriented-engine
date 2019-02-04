#include "Compass.h"
//#include "moc_Compass.cpp"

#include "view/UiUserLog.h"

#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRenderWindow.h>
#include <OgreTextAreaOverlayElement.h>
#include <engine/RenderView.h>
#include <engine/Engine.h>

#include "gameobject/GameObject.h"
#include "components/transform/Transform.h"

#include <QMainWindow>
#include <QMenu>

#include "system/IniConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"

//-----------------------------------------------------------------------------
Compass::Compass()
	:UiRepresentation(Ui_Representation_Compass)
{
	//degreeSign = GRender->convertToUTF("°");
	degreeSign = "°";

    overlay = Ogre::OverlayManager::getSingleton().getByName("TerraVR/Compass");
    overlay->hide();

    Ogre::OverlayContainer* container = overlay->getChild("TerraVR/CompassPanel");
    compassPanel = container->getChild("TerraVR/CompassPanel/Compass");
	compassPanelReflex = container->getChild("TerraVR/CompassPanel/CompassReflex");
    compassText = static_cast<Ogre::TextAreaOverlayElement*>(container->getChild("TerraVR/CompassPanel/Text"));
    compassText->setCaption("0" + degreeSign);

	//vendo que textura usaremos no overlay
#if (MINE_VERSION == PLA) && !defined(MINE_UG)
	compassPanel->setMaterialName("TerraVR/CompassMaterial_Green");
	compassPanelReflex->setMaterialName("TerraVR/CompassReflexMaterial_Green");
#elif defined(MINE_UG)
	compassPanel->setMaterialName("TerraVR/CompassMaterial_Orange");
	compassPanelReflex->setMaterialName("TerraVR/CompassReflexMaterial_Orange");
#endif

	newNorth = Ogre::Vector2(IniConfig::getInstance()->getAttribute("pref_NorteX").toFloat(), IniConfig::getInstance()->getAttribute("pref_NorteY").toFloat());
	newNorth.normalise();

	offsetDegree = 0;
	calculateOffsetDegree();

	optionMenu = new QMenu();
	actVisible = optionMenu->addAction(tr("Visível"));
	actVisible->setCheckable(true);

	QObject::connect(actVisible, SIGNAL(toggled(bool)), 
		this, SIGNAL(visibilityChanged(bool)) );
}

//-----------------------------------------------------------------------------
Compass::~Compass()
{
	
}

//-----------------------------------------------------------------------------
bool Compass::isActiveRepresentation()
{
	return true;
}

//-----------------------------------------------------------------------------
bool Compass::isVisibledRepresentation()
{
	return overlay->isVisible();
}

//-----------------------------------------------------------------------------
void Compass::setActiveRepresentation(bool)
{
}
	
//-----------------------------------------------------------------------------
void Compass::setVisibledRepresentation(bool cond)
{
	setVisible(cond);
}	
		
//-----------------------------------------------------------------------------
bool Compass::isValidName(const QString& id)
{
	return (id == "TerraVR/CompassPanel/Compass");
}

//-----------------------------------------------------------------------------
void Compass::updateRepresentation()
{
	GameObject* camObject = CameraControllerManager::getInstance()->getMainCameraController()->getCamera();
	Transform* transform = dynamic_cast<Transform*>(camObject->getComponent("Transform"));
	float g = -transform->getWorldYaw();

    double intPart, fractPart;

    fractPart = modf(g, &intPart);

    if (fractPart > 0.6)
        g = Ogre::Math::Ceil(g);
    else
        g = Ogre::Math::Floor(g);

    g = g + offsetDegree;
    Rendering->rotOverlayElement(compassPanel, g);

    int grau = g;

    if (oldDegree != grau)
    {
        if (grau < 0 && grau >= -179)
        {
            grau = 360 + grau;
        }

        grau = Ogre::Math::Abs(grau);

        compassText->setCaption(QString::number(grau).toStdString() + degreeSign);

        oldDegree = grau;
    }
}

//-----------------------------------------------------------------------------
void Compass::startEditing()
{
}

//-----------------------------------------------------------------------------
void Compass::endEditing()
{
}

//-----------------------------------------------------------------------------
bool Compass::keyPressed(const OIS::KeyEvent &arg)
{
	return true;
}

//-----------------------------------------------------------------------------
bool Compass::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

//-----------------------------------------------------------------------------
bool Compass::mouseMoved( const OIS::MouseEvent &arg,const QString& idname)
{
	return true;
}

//-----------------------------------------------------------------------------
bool Compass::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
	const QString& idname)
{
	return true;
}

//-----------------------------------------------------------------------------
bool Compass::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
	const QString& idname)
{
	RenderView * rw = Input->getRenderView();
	if(rw->isOnRender() && rw->isActiveWindow() && !Input->isMouseGrabbed())
	{
		switch(id)
		{
			case OIS::MB_Right:
			{
				if(!optionMenu->isVisible())
				{		
					actVisible->setChecked(true);
					QPoint p = ((QMainWindow*)rw->parentWidget())->pos();
					p += QPoint(arg.state.X.abs+20,arg.state.Y.abs+20);

					optionMenu->popup(p);
					optionMenu->show();		
				}
			}
			break;
			case OIS::MB_Left:
			{			
			}
			break;
		}	
	}
	return true;
}

//-----------------------------------------------------------------------------
void Compass::setVisible(bool cond)
{
	bool aux = overlay->isVisible();
	if(aux != cond)
	{
		if(cond)
		{
			overlay->show();
		}
		else
		{
			overlay->hide();
		}
		emit visibilityChanged(cond);
	}
}

//-----------------------------------------------------------------------------
void Compass::calculateOffsetDegree()
{
    float cosseno, norma_1, norma_2;

    norma_1 = north.length();
    norma_2 = newNorth.length();

    if (norma_1 != 0 && norma_2 != 0)
    {
       cosseno = (north.x * newNorth.x + north.y * newNorth.y) / (norma_1 * norma_2);

       // std::cerr << "\ncos: " << cosseno;

        if (cosseno >= 0 && cosseno < 1)
        {
            offsetDegree = 90 - Ogre::Math::RadiansToDegrees(acos(cosseno));
        }
        else if (cosseno < 0 && cosseno > -1)
        {
            offsetDegree = -(Ogre::Math::RadiansToDegrees(acos(cosseno)) - 90);
        }
        else
        {
            offsetDegree = 0;
        }
    }
}

void Compass::saveNewNorth(const Ogre::Vector2 &dir)
{
    newNorth = dir;
    newNorth.normalise();
    calculateOffsetDegree();
}

