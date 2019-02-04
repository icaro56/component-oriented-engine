

#include <Core.h>
#include <engine/Engine.h>
#include <input/InputManager.h>
#include <time/Time.h>
#include <QResource.h>
#include <QString.h>
#include "MyMainWindow.h"
#include "MyLogic.h"
#include "MyOgreResources.h"
#include "system/GlobalConfig.h"
#include "system/IniConfig.h"
#include "gameobject/LayerMask.h"
#include "gameobject/Tag.h"


int main(int argc, char *argv[])
{
	//Core::setStyle("cleanlooks");

	// iniciando o Core
    Core *core = new Core(argc, argv);
	core->setStyle("cleanlooks");

	//criação das pastas do usuario
	GlobalConfig::createDatFolders();

	// lendo arquivos de configuração
	IniConfig::getInstance()->read();

	//Iniciando Tag e Layers
	TagManager::getInstance()->init();
	LayerMask::initDefault();
	//LayerMask::setLayerCollision("Default", "Water", false);

	// iniciando GameEngine
	core->initGameEngine(DIRECT3D, "Ogre.log", true, "0", true);

	// iniciando MainWindow
	MyMainWindow* mainWindow = MyMainWindow::getInstance();
	core->initMainWindow(mainWindow);

	// iniciando Input Manager
	core->initInputManager(mainWindow, false, false);
    Input->setRenderView(Rendering->getRenderView("render1"));


	QResource::registerResource(GlobalConfig::GMEDIAPATH + "/themes/qrc_resources.rcc");
	QFile file(GlobalConfig::GMEDIAPATH + "/themes/blue_black.qss");
			file.open(QFile::ReadOnly);
			QString styleSheet = QLatin1String(file.readAll());
			core->setStyleSheet(styleSheet);

	// iniciando recursos
	initResources();

	// iniciando lógica
	MyLogic* mLogic = new MyLogic();
	core->initLogic(mLogic);

	mainWindow->loadMyStatus(false, true);

	//loop
	core->exec();

	core->destroy();
	
	return 0;
}
