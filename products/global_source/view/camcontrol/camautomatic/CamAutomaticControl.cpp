#include "CamAutomaticControl.h"
//#include "moc_CamAutomaticControl.cpp"

#include "system/ProjectConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"

#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

CamAutomaticControl* CamAutomaticControl::instance = 0;

//-----------------------------------------------------------------------------
CamAutomaticControl::CamAutomaticControl()
	:UiBasic(Ui_CamAutomatic)
{
	
}

//-----------------------------------------------------------------------------
CamAutomaticControl::~CamAutomaticControl()
{	
}

//-----------------------------------------------------------------------------
CamAutomaticControl* CamAutomaticControl::getInstance()
{
	if (!instance)
	{
		instance = new CamAutomaticControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		act = new QAction(QObject::tr("&Automática"),menu);
		act->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_camera_automatica.png"));
		act->setShortcut(Qt::CTRL + Qt::Key_6);
		act->setStatusTip(QObject::tr("Câmera Automática"));
		act->setCheckable(true);
		act->setChecked(false);

		menu->addAction(act);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(act);
	}			
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::setEnabledUi(bool cond)
{
	if(!act->isVisible()) return;
	
	act->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::setCheckedUi(bool cond)
{	
	if(!act->isVisible()) return;

	act->setChecked(cond);
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::setVisibleUi(bool cond)
{	
	act->setVisible(cond);
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::connects()
{	
	connect
	(
		act, SIGNAL(toggled(bool)), 
		this, SLOT(camActive(bool)) 
	);
}

//-----------------------------------------------------------------------------
void CamAutomaticControl::camActive(bool active)
{
	//falta
	//EquipmentManager * em = EquipmentManager::getInstance();

	if (active /* &&  !em->getEquipmentSelected()*/)
	{
		CameraControllerManager::getInstance()->getMainCameraController()->setCameraBehavior(CBT_AUTOMATIC);
		
		//falta
		//SceneNode* t = em->getEquipmentSelected()->getEntity()->getView()->getEntity();
		//CameraControllerManager::getInstance()->getMainCameraController()->setTarget(t);

		informWasActived(getType());
	}
	else
	{

	}
}