#include "CamAvatarControl.h"
//#include "moc_CamAvatarControl.cpp"

#include "system/ProjectConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"
#include "avatar/AvatarManager.h"
#include "avatar/Avatar.h"

#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>


CamAvatarControl* CamAvatarControl::instance = 0;

//-----------------------------------------------------------------------------
CamAvatarControl::CamAvatarControl()
	:UiBasic(Ui_CamAvatar)
{	
}

//-----------------------------------------------------------------------------
CamAvatarControl::~CamAvatarControl()
{	
}

//-----------------------------------------------------------------------------
CamAvatarControl* CamAvatarControl::getInstance()
{
	if (!instance)
	{
		instance = new CamAvatarControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void CamAvatarControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void CamAvatarControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		act = new QAction(QObject::tr("&Personagem"),menu);
		act->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_camera_avatar.png"));
		act->setShortcut(Qt::CTRL + Qt::Key_9);
		act->setStatusTip(QObject::tr("Câmera Personagem"));
		act->setCheckable(true);
		act->setChecked(false);	

		menu->addAction(act);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void CamAvatarControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(act);
	}			
}

//-----------------------------------------------------------------------------
void CamAvatarControl::setEnabledUi(bool cond)
{
	if(!act->isVisible()) return;

	act->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void CamAvatarControl::setCheckedUi(bool cond)
{	
	if(!act->isVisible()) return;

	act->setChecked(cond);
}

//-----------------------------------------------------------------------------
void CamAvatarControl::setVisibleUi(bool cond)
{	
	act->setVisible(cond);
}

//-----------------------------------------------------------------------------
void CamAvatarControl::connects()
{	
	QObject::connect
	(
		act, SIGNAL(toggled(bool)), 
		this, SLOT(camActive(bool)) 
	);
}

//-----------------------------------------------------------------------------
void CamAvatarControl::camActive(bool active)
{
	if (active)
	{
		CameraControllerManager::getInstance()->getMainCameraController()->setCameraBehavior(CBT_AVATAR);
		GameObject* t = AvatarManager::getInstance()->getMainAvatar()->getGameObject();
		
		CameraControllerManager::getInstance()->getMainCameraController()->setTarget(t);
		AvatarManager::getInstance()->setActiveMainAvatar(active);

		informWasActived(getType());
	}
	else
	{
		AvatarManager::getInstance()->setActiveMainAvatar(active);
	}
}
