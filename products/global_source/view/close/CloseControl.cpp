#include "CloseControl.h"
//#include "moc_CloseControl.cpp"

#include "Close.h"

#include "system/ProjectConfig.h"
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QMainWindow>

#include "Core.h"

CloseControl* CloseControl::instance = 0;

//-----------------------------------------------------------------------------
CloseControl::CloseControl()
	: UiBasic(Ui_Close),
	view(0)
{	
	view = new Close();	
}

//-----------------------------------------------------------------------------
CloseControl::~CloseControl()
{

	if (view)
	{
		delete view;
		view =0;
	}	
}

//-----------------------------------------------------------------------------
CloseControl* CloseControl::getInstance()
{
	if (!instance)
	{
		instance = new CloseControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void CloseControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void CloseControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		actVisible = new QAction(QObject::tr("&Fechar"),menu);
		actVisible->setShortcut(Qt::Key_Escape);
		actVisible->setStatusTip(QObject::tr("Fecha a aplicação"));	
		
		menu->addAction(actVisible);

		connects();
	}	
}

//-----------------------------------------------------------------------------
void CloseControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(actVisible);
	}			
}

//-----------------------------------------------------------------------------
void CloseControl::setEnabledUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	actVisible->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void CloseControl::setCheckedUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	actVisible->setChecked(cond);
}

//-----------------------------------------------------------------------------
void CloseControl::setVisibleUi(bool cond)
{	
	actVisible->setVisible(cond);
}

//-----------------------------------------------------------------------------
void CloseControl::connects()
{	
	QObject::connect(actVisible, 
				     SIGNAL(triggered(bool)), 
					 view, 
					 SLOT(show(bool)) );

	connect(view,
			SIGNAL(closeApplication()),
			this,
			SLOT(closeApplication()) );
}

//-----------------------------------------------------------------------------
bool CloseControl::close()
{
	return view->closeView();
}

//-----------------------------------------------------------------------------
void CloseControl::closeApplication()
{
	//qApp->quit();

	if (Core::getCore())
		Core::getCore()->quit();
}