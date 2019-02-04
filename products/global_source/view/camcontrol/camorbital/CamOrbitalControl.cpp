#include "CamOrbitalControl.h"
//#include "moc_CamOrbitalControl.cpp"

#include "system/ProjectConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"

#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

CamOrbitalControl* CamOrbitalControl::instance = 0;

//-----------------------------------------------------------------------------
CamOrbitalControl::CamOrbitalControl()
	:UiBasic(Ui_CamOrbital)
{	
}

//-----------------------------------------------------------------------------
CamOrbitalControl::~CamOrbitalControl()
{	
}

//-----------------------------------------------------------------------------
CamOrbitalControl* CamOrbitalControl::getInstance()
{
	if (!instance)
	{
		instance = new CamOrbitalControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		act = new QAction(QObject::tr("&Orbital"),menu);
		act->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_camera_orbital.png"));
		act->setStatusTip(QObject::tr("Câmera Orbital"));
		act->setShortcut(Qt::CTRL + Qt::Key_8);
		act->setCheckable(true);
		act->setChecked(false);

		menu->addAction(act);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(act);
	}			
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::setEnabledUi(bool cond)
{
	if(!act->isVisible()) return;

	act->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::setCheckedUi(bool cond)
{	
	if(!act->isVisible()) return;

	act->setChecked(cond);
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::setVisibleUi(bool cond)
{	
	act->setVisible(cond);
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::connects()
{	
	connect
	(
		act, SIGNAL(toggled(bool)), 
		this, SLOT(camActive(bool)) 
	);
}

//-----------------------------------------------------------------------------
void CamOrbitalControl::camActive(bool active)
{
	//falta
	//EquipmentManager * em = EquipmentManager::getInstance();

	if (active /* &&  !em->getEquipmentSelected()*/)
	{
		CameraControllerManager::getInstance()->getMainCameraController()->setCameraBehavior(CBT_ORBIT);
		
		//falta
		//SceneNode* t = em->getEquipmentSelected()->getEntity()->getView()->getEntity();
		//CameraControllerManager::getInstance()->getMainCameraController()->setTarget(t);

		informWasActived(getType());
	}
	else
	{

	}
}