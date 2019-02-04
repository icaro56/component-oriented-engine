#include "UITerrainControl.h"
//#include "moc_UITerrainControl.cpp"

#include "UITerrain.h"

#include "system/ProjectConfig.h"
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

#include "surface/SurfaceManager.h"
#include "gizmo/GizmoControl.h"

UITerrainControl* UITerrainControl::instance = 0;

//-----------------------------------------------------------------------------
UITerrainControl::UITerrainControl()
	:UiBasic(Ui_Terrain)	
{	
	view = new UITerrain();		
}

//-----------------------------------------------------------------------------
UITerrainControl::~UITerrainControl()
{
}

//-----------------------------------------------------------------------------
UITerrainControl* UITerrainControl::getInstance()
{
	if (!instance)
	{
		instance = new UITerrainControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void UITerrainControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void UITerrainControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		act = new QAction(QObject::tr("&Terreno"),menu);
		act->setShortcut(Qt::CTRL + Qt::Key_O);
		act->setStatusTip(QObject::tr("Terreno"));
		act->setCheckable(true);
		act->setChecked(false);

		menu->addAction(act);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void UITerrainControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(act);
	}			
}

//-----------------------------------------------------------------------------
void UITerrainControl::setEnabledUi(bool cond)
{
	if(!act->isVisible()) return;

	act->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void UITerrainControl::setCheckedUi(bool cond)
{	
	if(!act->isVisible()) return;

	act->setChecked(cond);
}

//-----------------------------------------------------------------------------
void UITerrainControl::setVisibleUi(bool cond)
{		
	act->setVisible(cond);
}

//-----------------------------------------------------------------------------
void UITerrainControl::connects()
{	
	QObject::connect(act, SIGNAL(toggled(bool)), 
					 view, SLOT(setVisible(bool)) );	

	QObject::connect(view, SIGNAL(visibilityChanged(bool)), 
					 act, SLOT(setChecked(bool)) );

	QObject::connect(act, SIGNAL(toggled(bool)), 
					 this, SLOT(setVisible(bool)) );

	//--------------	

	QObject::connect(view, SIGNAL(positionGizmo(int,int)), 
					 this, SLOT(positionGizmo(int,int)) );
}


//-----------------------------------------------------------------------------
void UITerrainControl::setVisible(bool cond)
{
	double v = (cond)?1:0;
	SurfaceManager::getInstance()->setOpacity(v);
}

//-----------------------------------------------------------------------------
void UITerrainControl::positionGizmo(int x,int y)
{
	GizmoControl::getInstance()->setPosition(x,y);
}