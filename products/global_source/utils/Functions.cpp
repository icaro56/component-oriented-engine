#include "Functions.h"

#include <QString>
#include <QCursor>

#include <OgreRenderWindow.h>
#include <engine/RenderView.h>
#include <engine/Engine.h>
#include <input/InputManager.h>

#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>

#include "surface/Surface.h"
#include "surface/SurfaceManager.h"

/*#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>*/

//-----------------------------------------------------------------------------
bool Collision2::hasCollision(int mx,int my,QString & r)
{	
	//MyGui
	/*MyGUI::ListBox * viewSM = GRender->getMyGUI()->findWidget<MyGUI::ListBox>("StatusMessages",false);
	if(viewSM && viewSM->isVisible())
    {
        if(viewSM->getAbsolutePosition().top < QCursor::pos().y())
        {
            if(viewSM->getAbsolutePosition().left < QCursor::pos().x())
            {
				 r = "StatusMessages";
                 return true;
            }
        }
    }*/
	
	//Overlay 
	Ogre::OverlayManager &om = Ogre::OverlayManager::getSingleton();
	Ogre::OverlayManager::OverlayMapIterator &it = om.getOverlayIterator();	
	double x;
	double y;
	double z;
	Position::mouseRelativePosition(mx,my,x,y);
	while(it.hasMoreElements())
	{
		Ogre::Overlay *aux = it.getNext();
		Ogre::OverlayElement *oe = aux->findElementAt(x,y);		
		if(oe && oe->isVisible())
		{
			r = oe->getName().c_str();
			return true;
		}
	}

	//Mesh

	//Terreno
	if(SurfaceManager::getInstance()->mousePosition(mx,my,x,y,z))
	{
		r = "Terrain";
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void Position::mouseRelativePosition(int mX,int mY, double& x, double& y)
{
	RenderView * rw = Input->getRenderView();
	Vector2 posRender = rw->getAbsolutePosition();

	double w = double(rw->getRenderWindow()->getWidth());
    double h = double(rw->getRenderWindow()->getHeight());

	x = (mX - posRender.x) / w;
	y = (mY - posRender.y) / h;
}

