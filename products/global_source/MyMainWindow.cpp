#include "MyMainWindow.h"

#include <qapplication.h>
#include <qsettings.h>
#include <qdesktopwidget.h>
#include <qmenu.h>
#include <qtoolbar.h> 
#include <qfile.h>
#include <qevent.h>
#include <qmenubar.h>

#include "system/IniConfig.h"
#include "system/GlobalConfig.h"
#include "view/statusbar/StatusBarControl.h"
#include "view/UiRepresentationManager.h"
#include "view/UiBasicManager.h"
#include "view/close/CloseControl.h"
#include "view/compass/CompassControl.h"
#include "view/camcontrol/CamControl.h"
#include "view/minimap/MiniMapControl.h"
#include "view/configterrain/ConfigTerrainControl.h"
#include "view/terrain/UITerrainControl.h"


MyMainWindow *MyMainWindow::instance = 0;

//-----------------------------------------------------------------------------
MyMainWindow *MyMainWindow::getInstance()
{
    if (instance == 0)
    {
        instance = new MyMainWindow();
    }

    return instance;
}

//-----------------------------------------------------------------------------
void MyMainWindow::destroy()
{
    delete instance;
    instance = 0;
}

MyMainWindow::MyMainWindow(QWidget* parent)
	: MainWindow(parent)
{
	setAcceptDrops(true);

	//Configurando QT para poder usar settings
	QCoreApplication::setOrganizationDomain("http://www.visualvirtual.com/");
    QCoreApplication::setOrganizationName("VisualVirtual");
	QCoreApplication::setApplicationName(ProjectInfo::PROJECT_NAME);
    QCoreApplication::setApplicationVersion("1.0.0");

	setWindowTitle( "Program_X" + tr(" - ") + ProjectInfo::PROJECT_NAME);

	loadMyStatus(true, false);

    setFullScreen(false);
}

MyMainWindow::~MyMainWindow()
{

}

void MyMainWindow::update()
{
	if(mIsInitialised)
	{	
		UiRepresentationManager::getInstance()->updateRepresentation();
	}
}

void MyMainWindow::init()
{
	createMenus();
	createToolBars();
	createStatusBar();
    createDockWidgets();

	UiBasicManager::getInstance()->init();
	UiRepresentationManager::getInstance()->init();

	QByteArray state = saveState();

    QFile windowStateFile(GlobalConfig::DATAPATH + "/WindowState.txt");

    if(windowStateFile.open(QIODevice::WriteOnly))
    {
      windowStateFile.write(state);
      windowStateFile.close();
    }
}

//-----------------------------------------------------------------------------
void MyMainWindow::setFullScreen(bool fullscreen)
{
    if (fullscreen)
    {
        setWindowFlags(Qt::FramelessWindowHint);
    }
    else
    {
        setWindowFlags(Qt::Window);
    }    
}

//-----------------------------------------------------------------------------
void MyMainWindow::loadMyStatus(bool geometry, bool state)
{
	if (geometry)
	{
		//restaurando geometria
		QSettings settings;

		bool hasData = restoreGeometry(settings.value("MainWindowGeometry").toByteArray());

		if (!hasData)
		{
			int idScreen = QApplication::desktop()->primaryScreen();
    
			QRect r = QApplication::desktop()->availableGeometry(idScreen);
			const int width = r.width() - 15;
			const int height = r.height() - 40;
    
			setGeometry(0,0,width, height);
		}
	}

	if (state)
	{
		//falta
		/*
		QSettings settings;

		restoreState(settings.value("MainWindowState").toByteArray());
		*/
		bool isMax = IniConfig::getInstance()->getAttribute("MainWindowMaximized").toInt();

		if (isMax)
		{
			this->showMaximized();
		}
		else
		{
			this->show();
		}
	}
}

//-----------------------------------------------------------------------------
void MyMainWindow::saveMyStatus()
{
	QSettings settings;
	settings.setValue("MainWindowGeometry", saveGeometry());
	settings.setValue("MainWindowState", saveState());

	int v = isMaximized();
	IniConfig::getInstance()->setAttribute("MainWindowMaximized", QString::number(v));
}

//-----------------------------------------------------------------------------
void MyMainWindow::closeEvent(QCloseEvent *e)
{    
	if (CloseControl::getInstance()->close())
    {
		//salvando o estado da janela
		saveMyStatus();

		IniConfig::getInstance()->write();

        e->accept();        
        //exit(EXIT_SUCCESS);
    }
    else
    {
        e->ignore();
    }
}

//-----------------------------------------------------------------------------
void MyMainWindow::dragEnterEvent(QDragEnterEvent *evt)
{
}

//-----------------------------------------------------------------------------
void MyMainWindow::dropEvent(QDropEvent *evt)
{	
}

//-----------------------------------------------------------------------------
void MyMainWindow::createMenus()
{   
	/// Navegação
	navigationMenu = new QMenu(tr("&Navegação"), this);
	
	CamControl::getInstance()->addActionsInMenu(navigationMenu);	

	if(navigationMenu->children().count())
	{
		navigationMenu->addSeparator();
	}

	CloseControl::getInstance()->addActionsInMenu(navigationMenu);
	
	if(navigationMenu->children().count())
	{
		menuBar()->addMenu(navigationMenu);
	}

	/// Visualizar
	visualizeMenu = new QMenu(tr("&Visualizar"), this);
	MiniMapControl::getInstance()->addActionsInMenu(visualizeMenu);
	CompassControl::getInstance()->addActionsInMenu(visualizeMenu);
	visualizeMenu->addSeparator();	
	UITerrainControl::getInstance()->addActionsInMenu(visualizeMenu);
	visualizeMenu->addSeparator();	
	StatusBarControl::getInstance()->addActionsInMenu(visualizeMenu);
	
	if(visualizeMenu->children().count())
	{
		menuBar()->addMenu(visualizeMenu);
	}

	// Menu Ferramentas
	toolMenu = new QMenu(tr("&Ferramentas"), this);
	ConfigTerrainControl::getInstance()->addActionsInMenu(toolMenu);

	//GeologicalBlocksControl::getInstance()->addActionsInMenu(toolMenu);
	
	
	if(toolMenu->children().count())
	{
		menuBar()->addMenu(toolMenu);
	}
}

void MyMainWindow::createToolBars()
{	
	//Visualizar
	visualizeToolBar = new QToolBar(tr("&Program_X"),this);
	visualizeToolBar->setObjectName(tr("&Program_X"));

	MiniMapControl::getInstance()->addActionsInToolbar(visualizeToolBar);
	CompassControl::getInstance()->addActionsInToolbar(visualizeToolBar);
	visualizeToolBar->addSeparator();

	ConfigTerrainControl::getInstance()->addActionsInToolbar(visualizeToolBar);

	if(visualizeToolBar->actions().size())
	{
		addToolBar(visualizeToolBar);
	}

	//Cam�ra
	camToolBar = new QToolBar(tr("&Navegação"),this);
	camToolBar->setObjectName(tr("&Navegação"));
	//MoveMineControl::getInstance()->addActionsInToolbar(camToolBar);
	//camToolBar->addSeparator();
	//camToolBar->addSeparator();	

	CamControl::getInstance()->addActionsInToolbar(camToolBar);

	if(camToolBar->actions().size())
	{
		addToolBar(camToolBar);
	} 	

	/*toolsToolBar = new QToolBar(tr("&Ferramentas de Edição"),this);   
	//GeologicalBlocksControl::getInstance()->addActionsInToolbar(toolsToolBar);

	if(toolsToolBar->actions().size())
	{
		/*toolsToolBar->setOrientation(Qt::Vertical);
		toolsToolBar->setAllowedAreas(Qt::RightToolBarArea|Qt::LeftToolBarArea);
		addToolBar(Qt::RightToolBarArea, toolsToolBar);

		addToolBar(toolsToolBar);
	}*/
}

//-----------------------------------------------------------------------------
void MyMainWindow::createStatusBar()
{
	StatusBarControl::getInstance()->addStatusBar(statusBar());
}

//-----------------------------------------------------------------------------
void MyMainWindow::createDockWidgets()
{
	//GeologicalBlocksControl::getInstance()->addDockWidget(this);

	/*LogAplicationControl::getInstance()->addDockWidget(this);
	DrawControl::getInstance()->addDockWidget(this);
	ModificationTerrainControl::getInstance()->addDockWidget(this);
	TopographicProfileControl::getInstance()->addDockWidget(this);
	EffectivenessMiningControl::getInstance()->addDockWidget(this);
	ControlPanelControl::getInstance()->addDockWidget(this);
	ObjectInspectorControl::getInstance()->addDockWidget(this);
	AnotationControl::getInstance()->addDockWidget(this);
	MensurationControl::getInstance()->addDockWidget(this);*/
}