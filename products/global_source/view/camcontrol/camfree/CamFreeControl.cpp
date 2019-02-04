#include "CamFreeControl.h"
//#include "moc_CamFreeControl.cpp"


#include "system/ProjectConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"
#include "view/UiBasic.h"
#include "view/UiBasicManager.h"

#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

CamFreeControl* CamFreeControl::instance = 0;

//-----------------------------------------------------------------------------
CamFreeControl::CamFreeControl()
	:UiBasic(Ui_CamFree)
{	
}

//-----------------------------------------------------------------------------
CamFreeControl::~CamFreeControl()
{	
}

//-----------------------------------------------------------------------------
CamFreeControl* CamFreeControl::getInstance()
{
	if (!instance)
	{
		instance = new CamFreeControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void CamFreeControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void CamFreeControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		act = new QAction(QObject::tr("&Livre"),menu);
		act->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_camera_livre.png"));
		act->setShortcut(Qt::CTRL + Qt::Key_7);
		act->setStatusTip(QObject::tr("Câmera livre"));
		act->setCheckable(true);
		act->setChecked(false);

		menu->addAction(act);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void CamFreeControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(act);
	}			
}

//-----------------------------------------------------------------------------
void CamFreeControl::setEnabledUi(bool cond)
{
	if(!act->isVisible()) return;

	act->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void CamFreeControl::setCheckedUi(bool cond)
{	
	if(!act->isVisible()) return;

	act->setChecked(cond);
}

//-----------------------------------------------------------------------------
void CamFreeControl::setVisibleUi(bool cond)
{	
	act->setVisible(cond);
}

//-----------------------------------------------------------------------------
void CamFreeControl::connects()
{	
	QObject::connect(act, SIGNAL(toggled(bool)), this, SLOT(camActive(bool)));
}

//-----------------------------------------------------------------------------
void CamFreeControl::camActive(bool active)
{
	if (active)
	{
		CameraControllerManager::getInstance()->getMainCameraController()->setCameraBehavior(CBT_FREE);

		informWasActived(getType());
	}
}
