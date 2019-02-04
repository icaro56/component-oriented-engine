#include "CamControl.h"
//#include "moc_CamControl.cpp"

#include <QMenu>
#include <QToolBar> 

#include "camautomatic/CamAutomaticControl.h"
#include "camavatar/CamAvatarControl.h"
#include "camorbital/CamOrbitalControl.h"
#include "camfree/CamFreeControl.h"
#include "camge/CamGEControl.h" 

#include "view/UiBasicManager.h"   
  
CamControl* CamControl::instance = 0;

CamControl* CamControl::getInstance()
{
	if (!instance)
	{
		instance = new CamControl();
	}

	return instance;
}

CamControl::CamControl()
{
	//adicionando ações das cameras
	camerasActions.push_back(CamGEControl::getInstance());
	camerasActions.push_back(CamFreeControl::getInstance());
	camerasActions.push_back(CamOrbitalControl::getInstance());
	camerasActions.push_back(CamAvatarControl::getInstance());
	camerasActions.push_back(CamAutomaticControl::getInstance());

	//connects
	for (int i =0; i < camerasActions.size(); ++i)
	{
		QObject* obj = dynamic_cast<QObject*>(camerasActions[i]);
		connect(obj,
				SIGNAL(informWasActived(unsigned int)),
				this,
				SLOT(processingCamActived(unsigned int)));
	}
}

CamControl::~CamControl()
{
	camerasActions.clear();
}

void CamControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

void CamControl::addActionsInMenu(QMenu* menu)
{
	if (menu)
	{
		for(int i = 0; i < camerasActions.size(); ++i)
		{
			camerasActions[i]->addActionsInMenu(menu);
		}
	}
}

void CamControl::addActionsInToolbar(QToolBar* tbar)
{
	if (tbar)
	{
		for(int i = 0; i < camerasActions.size(); ++i)
		{
			camerasActions[i]->addActionsInToolbar(tbar);
		}

		
	}
}
#include <iostream>
void CamControl::processingCamActived(unsigned int t)
{
	for(int i= 0; i < camerasActions.size(); ++i)
	{
		UiBasic* b = camerasActions[i];

		if (b->getType() != t)
		{
			UiBasicManager::getInstance()->setCheckedUi(b->getType(), false);
		}
	}

	if (t == Ui_CamFree)
	{
		UiBasicManager::getInstance()->setEnableUi(Ui_MoveMine, true);
	}
	else
	{
		UiBasicManager::getInstance()->setEnableUi(Ui_MoveMine, false);
	}
}