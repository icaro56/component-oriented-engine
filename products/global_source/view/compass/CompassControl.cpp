#include "CompassControl.h"
//#include "moc_CompassControl.cpp"

#include "Compass.h"

#include "system/ProjectConfig.h"
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>

CompassControl* CompassControl::instance = 0;

//-----------------------------------------------------------------------------
CompassControl::CompassControl()
	: UiBasic(Ui_Compass),
	view(0)
{	
	view = new Compass();	
}

//-----------------------------------------------------------------------------
CompassControl::~CompassControl()
{
	if (view)
	{
		delete view;
		view =0;
	}		
}

//-----------------------------------------------------------------------------
CompassControl* CompassControl::getInstance()
{
	if (!instance)
	{
		instance = new CompassControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void CompassControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void CompassControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		actVisible = new QAction(QObject::tr("&Bússola"),menu);
		actVisible->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_bussola.png"));
		actVisible->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
		actVisible->setStatusTip(QObject::tr("Exibe a Bússola"));
		actVisible->setCheckable(true);
		actVisible->setChecked(false);		

		menu->addAction(actVisible);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void CompassControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(actVisible);
	}			
}

//-----------------------------------------------------------------------------
void CompassControl::setEnabledUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	if(!cond)
	{
		//view->close();
	}
	actVisible->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void CompassControl::setCheckedUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	actVisible->setChecked(cond);
}

//-----------------------------------------------------------------------------
void CompassControl::setVisibleUi(bool cond)
{	
	if(!cond)
	{
		//view->close();
	}
	actVisible->setVisible(cond);
}

//-----------------------------------------------------------------------------
void CompassControl::connects()
{	
	QObject::connect(actVisible, SIGNAL(toggled(bool)), 
					view, SLOT(setVisible(bool)) );
		
	QObject::connect(view, SIGNAL(visibilityChanged(bool)), 
					 actVisible, SLOT(setChecked(bool)) );
}

void CompassControl::updateCompass()
{
	if (view)
	{
		view->updateRepresentation();
	}
}