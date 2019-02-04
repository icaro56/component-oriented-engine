#include "Core.h"

#include "input/InputManager.h"
#include "engine/Engine.h"
#include "engine/RenderingEngine.h"
#include "time/Time.h"
#include <QWidget>

#include "MainWindow.h"
#include "Logic.h"

#include <OgreRoot.h>
#include <OgreTimer.h>

#include "gameobject/LayerMask.h"
#include "gameobject/Tag.h"

#include "moc_Core.cpp"

Core* Core::sCore = NULL;
///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

Core* Core::getCore()
{
	return sCore;
}

Core::Core( int & argc, char ** argv )
	: QApplication(argc, argv),
	  mWindow(nullptr),
	  mEngine(nullptr),
	  mInput(nullptr),
	  isAllInitialized(false),
	  mLogic(nullptr)
{
	if (!sCore)
		sCore = this;
	else
		std::cerr << "\nCore já foi inicializado.";
}

void Core::initMainWindow(MainWindow* app)
{
	if (!mWindow)
	{
		mWindow = app;
		mWindow->createRenderView();		
	}
	else
		std::cerr << "\nA janela já foi inicializada";
}

void Core::initLogic(Logic* l)
{
	if (!mLogic)
	{
		mLogic = l;
		mLogic->init();
		mWindow->init();
	}
	else
	{
		std::cerr << "\nA lógica já foi inicializada";
	}

	isAllInitialized = true;
}

void Core::initGameEngine(  int rs, 
						    const QString& log,
						    const bool vsync,
						    const std::string& fsaa, 
						    bool allowPerfHud,
							Ogre::SceneType typeMask)
{
	if (!mEngine)
	{
		mEngine = new Engine(log);

		//iniciando render System
		mEngine->getRenderingEngine()->setupRenderSystem(RenderSystem(rs), vsync, fsaa, allowPerfHud);

		//iniciando o sceneManager
		mEngine->setupScene(typeMask);
	}
	else
		std::cerr << "\nRender Engine já foi inicializado";
}

void Core::initInputManager(QWidget * widget,const bool isExclusive, const bool isForeground)
{
	if (!mInput)
		mInput = new InputManager((size_t)widget->winId(), isExclusive, isForeground);
	else
		std::cerr << "\nInput Manager já foi inicializado";

	
}

//-----------------------------------------------------------------------------
Core* Core::getInstance()
{
	if (!sCore)
	{
		std::cerr << "\nCore não foi inicializado ainda.";
	}

    return sCore;
}

//-----------------------------------------------------------------------------
void Core::destroy()
{
	if (sCore)
	{
		delete sCore;
		sCore = nullptr;
	}
}

//-----------------------------------------------------------------------------
int Core::exec()
{
	sCore->myConnects();
	sCore->startTimer(0);

	//configurando classe Time. Ela precisa do Ogre::Root iniciado
	Time->init(sCore->mEngine->getRenderingEngine()->getRoot()->getTimer());

	sCore->updateSizeMainWindow(sCore->getMainWindow()->width(), 
								sCore->getMainWindow()->height());
	
	return QApplication::exec();
}

void Core::myConnects()
{
	connect(mWindow,
			SIGNAL(resizeEventOcurred(int, int)),
			this,
			SLOT(updateSizeMainWindow(int, int)) );

	connect(mWindow,
			SIGNAL(activationWindow(bool)),
			this,
			SLOT(setMousePosition(bool)) );
}

//-----------------------------------------------------------------------------
void Core::show()
{
	if (mWindow)
		mWindow->showMaximized();
}

void Core::captureResizeEvents()
{
	if (mWindow)
	{
		mWindow->showMaximized();
		mWindow->hide();
	}
}

//-----------------------------------------------------------------------------
bool Core::updateKernel()
{
	Time->update();

	if (mInput)
		mInput->processEvents();

	mLogic->updateLogic();

	//GAudio->update();

	mWindow->update();

	if (!mEngine->updateWorld())
	{
		mWindow->close();
	}

	processEvents();

    return true;
}

//-----------------------------------------------------------------------------
MainWindow* Core::getMainWindow()
{
    return mWindow;
}

//-----------------------------------------------------------------------------
Logic* Core::getLogic()
{
	return mLogic;
}

//-----------------------------------------------------------------------------
Engine* Core::getGameEngine()
{
    return mEngine;
}

//-----------------------------------------------------------------------------
InputManager* Core::getInputHandler()
{
    return mInput;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
Core::~Core()
{
	isAllInitialized = false;

	if (mInput)
	{
		delete mInput;
		mInput = nullptr;
	}

	if (mLogic)
	{
		delete mLogic;
		mLogic = nullptr;
	}

	if (mWindow)
	{
		delete mWindow;
		mWindow = nullptr;
	}

	if (mEngine)
	{
		delete mEngine;
		mEngine = nullptr;
	}
}

//-----------------------------------------------------------------------------
void Core::timerEvent(QTimerEvent* e)
{
    if (isAllInitialized)
	{
		updateKernel();
	}
}

void Core::updateSizeMainWindow(int w, int h)
{
	mInput->resizeWindow(w, h);
}

void Core::setMousePosition(bool active)
{
	/*if (active)
	{
		POINT p;
		GetCursorPos(&p);
		//simula um mouse move setando 2 vezes a posição do mouse
		//QPoint p = QCursor::pos();

		GInput->moveMouse(p.x,p.y);
		GInput->moveMouse(p.x,p.y);
	}
	else*/
	if (!active)
	{
		if (mInput->isMouseGrabbed())
		{
			mInput->grabMouse(false);
		}
	}

	mInput->processEvents();
}