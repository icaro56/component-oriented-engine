#include "GizmoControl.h"

#include "Gizmo.h"
#include "Gizmo3d.h"

#include "system/ProjectConfig.h"
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

#include "surface/Surface.h"
#include "surface/SurfaceManager.h"

#include <input/InputManager.h>

#include "view/statusbar/StatusBarControl.h"

//#include "moc_GizmoControl.cpp"

GizmoControl* GizmoControl::instance = 0;

//-----------------------------------------------------------------------------
GizmoControl::GizmoControl()
	:viewMain(0)
{	

	model = new Gizmo();
	viewMain = new Gizmo3d();

	connects();
}

//-----------------------------------------------------------------------------
GizmoControl::~GizmoControl()
{
	if (viewMain)
	{
		delete viewMain;
		viewMain = 0;
	}

	if (model)
	{
		delete model;
		model = 0;
	}
}

//-----------------------------------------------------------------------------
GizmoControl* GizmoControl::getInstance()
{
	if (!instance)
	{
		instance = new GizmoControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void GizmoControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void GizmoControl::connects()
{	
	QObject::connect(model, SIGNAL(setScale(const double&)), 
					 viewMain, SLOT(setScale(const double&)) );
}

//-----------------------------------------------------------------------------
void GizmoControl::setPosition(const double& x, const double& y, 
	const double& z)
{
	viewMain->setPosition(x,y,z);
	model->setPosition(x,y,z);	

	StatusBarControl::getInstance()->setGizmoCoordinated(x,y,z);
}

//-----------------------------------------------------------------------------
void GizmoControl::setPosition(int mx, int my)
{
	double x;
	double y;
	double z;

	if(SurfaceManager::getInstance()->mousePosition(mx,my,x,y,z))
	{
		viewMain->setPosition(x,y,z);
		model->setPosition(x,y,z);		
		StatusBarControl::getInstance()->setGizmoCoordinated(x,y,z);
	}
}

//-----------------------------------------------------------------------------
void GizmoControl::calculateScale()
{
	if (model)
	{
		model->calculeScale();
	}
}

/*
//-----------------------------------------------------------------------------
bool GizmoControl::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if((GInput->getRenderView()->isActiveWindow()) && (GInput->getRenderView()->isOnRender()) )
	{
		if(id == OIS::MB_Left)
		{
			double x;
			double y;
			double z;

			if(SurfaceManager::getInstance()->
				mousePosition(arg.state.X.abs,arg.state.Y.abs,x,y,z))
			{
				viewMain->setPosition(x,y,z);
				model->setPosition(x,y,z);			
			}		
		}
	}
	return true;
}

*/