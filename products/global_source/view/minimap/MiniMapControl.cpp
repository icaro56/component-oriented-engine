#include "MiniMapControl.h"
//#include "moc_MiniMapControl.cpp"

#include "MiniMap.h"

#include "system/ProjectConfig.h"
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

#include "surface/SurfaceManager.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"


MiniMapControl* MiniMapControl::instance = 0;

//-----------------------------------------------------------------------------
MiniMapControl::MiniMapControl()
	: UiBasic(Ui_Minimap),
	view(0)
{		
}

//-----------------------------------------------------------------------------
MiniMapControl::~MiniMapControl()
{
	if (view)
	{
		delete view;
		view =0;
	}	
}

//-----------------------------------------------------------------------------
MiniMapControl* MiniMapControl::getInstance()
{
	if (!instance)
	{
		instance = new MiniMapControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void MiniMapControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void MiniMapControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		view = new MiniMap();		

		actVisible = new QAction(QObject::tr("&Minimapa"),menu);
		actVisible->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_mapa.png"));
		actVisible->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
		actVisible->setStatusTip(QObject::tr("Exibe o Minimapa"));
		actVisible->setCheckable(true);
		actVisible->setChecked(false);
				
		menu->addAction(actVisible);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void MiniMapControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(actVisible);
	}			
}

//-----------------------------------------------------------------------------
void MiniMapControl::setEnabledUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	if(!cond)
	{
		view->close();
	}
	actVisible->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void MiniMapControl::setCheckedUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	actVisible->setChecked(cond);
}

//-----------------------------------------------------------------------------
void MiniMapControl::setVisibleUi(bool cond)
{	
	if(!cond)
	{
		view->close();
	}
	actVisible->setVisible(cond);
}

//-----------------------------------------------------------------------------
void MiniMapControl::connects()
{	
	QObject::connect(actVisible, SIGNAL(toggled(bool)), 
					 view, SLOT(setVisible(bool)) );

	QObject::connect(view, SIGNAL(visibilityChanged(bool)), 
					 actVisible, SLOT(setChecked(bool)) );

	//---------

	QObject::connect(view, SIGNAL(changedTextureTerrain(int)), 
					 this, SLOT(changedTextureTerrain(int)) );

	QObject::connect(view, SIGNAL(changedPosition(const double&,const double&)), 
				     this, SLOT(changedPosition(const double&,const double&)));

	CameraController * cc = CameraControllerManager::getInstance()->getMainCameraController();
	connect(this, SIGNAL(informPosition3D(Vector3&)),
			cc,   SLOT(setWorldPositionToMap(Vector3&)) );
}

//-----------------------------------------------------------------------------
float MiniMapControl::getWidth() const
{
	if (view)
	{
		return view->getWidth();
	}

	return 0;
}


//-----------------------------------------------------------------------------
void MiniMapControl::changedTextureTerrain(int v)
{//falta
	/*switch(v)
	{
		case Satellite:
		{
		}
		break;
		case ShadesGray:
		{
		}
		break;
	}*/
}

//-----------------------------------------------------------------------------
void MiniMapControl::changedPosition(const double& mx,const double& my)
{
	SurfaceManager * sm = SurfaceManager::getInstance();
	double terrainWidth = sm->getWidth();
	double halfWidth = terrainWidth * 0.5;

    double x = -halfWidth + terrainWidth*mx;
    double y = -halfWidth + terrainWidth*my;

	Vector3 v;
	v.x = x;
	v.y = sm->getHeightAt(x,y);
	v.z = y;
	
	informPosition3D(v);
}
