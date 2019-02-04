#include "UITerrain.h"
//#include "moc_UITerrain.cpp"

#include <OgreRenderWindow.h>
#include <engine/RenderView.h>
#include <input/InputManager.h>

#include <QMenu>
#include <QAction>
#include <QMainWindow>

#include "view/UiUserLog.h"
#include "utils/Functions.h"

//-----------------------------------------------------------------------------
UITerrain::UITerrain()
	:UiRepresentation(Ui_Representation_Terrain)
{	
	optionMenu = new QMenu();
	actVisible = optionMenu->addAction(tr("Visível"));
	actVisible->setCheckable(true);

	QObject::connect(actVisible, SIGNAL(toggled(bool)), 
		this, SIGNAL(visibilityChanged(bool)) );
}

//-----------------------------------------------------------------------------
UITerrain::~UITerrain()
{
	
}

//-----------------------------------------------------------------------------
void UITerrain::close()
{
	setVisible(false);
}

//-----------------------------------------------------------------------------
void UITerrain::setVisible(bool cond)
{	
	actVisible->setChecked(cond);
}

//-----------------------------------------------------------------------------
bool UITerrain::isActiveRepresentation()
{
	return true;
}

//-----------------------------------------------------------------------------
bool UITerrain::isVisibledRepresentation()
{
	return actVisible->isChecked();
}

//-----------------------------------------------------------------------------
void UITerrain::setActiveRepresentation(bool)
{
}

//-----------------------------------------------------------------------------
void UITerrain::setVisibledRepresentation(bool cond)
{
	setVisible(cond);
}

//-----------------------------------------------------------------------------
bool UITerrain::isValidName(const QString& id)
{	
	return (id == "Terrain");
}

//-----------------------------------------------------------------------------
void UITerrain::startEditing()
{	
}

//-----------------------------------------------------------------------------
void UITerrain::endEditing()
{
}

//-----------------------------------------------------------------------------
void UITerrain::updateRepresentation()
{	
}

//-----------------------------------------------------------------------------
bool UITerrain::keyPressed(const OIS::KeyEvent &arg)
{
	return true;
}

//-----------------------------------------------------------------------------
bool UITerrain::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

//-----------------------------------------------------------------------------
bool UITerrain::mouseMoved( const OIS::MouseEvent &arg, const QString& idname)
{
	return true;
}

//-----------------------------------------------------------------------------
bool UITerrain::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
	const QString& idname)
{
	return true;
}

//-----------------------------------------------------------------------------
bool UITerrain::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
	const QString& idname )
{		
	RenderView * rw = Input->getRenderView();
	if(rw->isActiveWindow() && rw->isOnRender() && !Input->isMouseGrabbed())
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
				emit positionGizmo(arg.state.X.abs,arg.state.Y.abs);
			}
			break;
		}	
	}
	return true;
}






