#include "CamGEControl.h"
//#include "moc_CamGEControl.cpp"

#include "system/ProjectConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"

#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

CamGEControl* CamGEControl::instance = 0;

//-----------------------------------------------------------------------------
CamGEControl::CamGEControl()
	:UiBasic(Ui_CamGE)
{	
}

//-----------------------------------------------------------------------------
CamGEControl::~CamGEControl()
{	
}

//-----------------------------------------------------------------------------
CamGEControl* CamGEControl::getInstance()
{
	if (!instance)
	{
		instance = new CamGEControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void CamGEControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void CamGEControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{		
		act = new QAction(QObject::tr("&Google Earth"),menu);
		act->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_camera_avatar.png"));
		act->setShortcut(Qt::CTRL + Qt::Key_9);
		act->setStatusTip(QObject::tr("Câmera Google Earth"));
		act->setCheckable(true);
		act->setChecked(false);	

		menu->addAction(act);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void CamGEControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(act);
	}			
}

//-----------------------------------------------------------------------------
void CamGEControl::setEnabledUi(bool cond)
{
	if(!act->isVisible()) return;

	act->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void CamGEControl::setCheckedUi(bool cond)
{	
	if(!act->isVisible()) return;

	act->setChecked(cond);
}

//-----------------------------------------------------------------------------
void CamGEControl::setVisibleUi(bool cond)
{	
	act->setVisible(cond);
}

//-----------------------------------------------------------------------------
void CamGEControl::connects()
{	
	connect
	(
		act, SIGNAL(toggled(bool)), 
		this, SLOT(camActive(bool)) 
	);
}


//-----------------------------------------------------------------------------
void CamGEControl::camActive(bool active)
{
	if (active)
	{
		CameraControllerManager::getInstance()->getMainCameraController()->setCameraBehavior(CBT_GOOGLE_EARTH);

		informWasActived(getType());
	}
}