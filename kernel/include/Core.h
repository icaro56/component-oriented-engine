#ifndef CORE_H
#define CORE_H

/**
* @def Inicial a classe Core
* @return Ponteiro Core
*/

class MainWindow;
class Engine;
class InputManager;
class Logic;
class QWidget;

#include <OgreSceneManager.h>
#include <QString>
#include "KernelPrerequisites.h"
#include <QApplication>

///////////////////////////////////////////////////////////////////////////////
// CLASS Core
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief desencadeia todos o componentes da aplicação
 *
 * @details Classe central responsavel por iniciar os
 * principais componentes para uma aplicação
 */
class KERNEL_VS_EXPORT Core : public QApplication
{
    // PUBLIC ///////////////////////////////////////////////
	Q_OBJECT

public:

    /**********************************************************************************************//**
     * \fn	Core::Core( int & argc, char ** argv );
     *
     * \brief	Constructor.
     *
     * \author	Admin
     * \date	06/02/2014
     *
     * \param [in,out]	argc	The argc.
     * \param [in,out]	argv	If non-null, the argv.
     **************************************************************************************************/

    Core( int & argc, char ** argv );

	static Core* getCore();

	/*!
		\brief inicializa MainWindow
	*/
	void initMainWindow(MainWindow* app);

	/*!
		\brief inicializa Lógica
	*/
	void initLogic(Logic* l);

	/*!
		\brief Inicializa RenderEngine (Ogre)
	*/
	void initGameEngine(int rs, 
						  const QString& log = QString(""),
						  const bool vsync = true,
						  const std::string& fsaa = "0", 
						  bool allowPerfHud = false,
						  Ogre::SceneType typeMask = Ogre::ST_EXTERIOR_CLOSE);

	/*!
		\brief Inicializa inputManager
	*/
	void initInputManager(QWidget * widget, const bool isExclusive = false, const bool isForeground = true);

	/*!
		\brief retorna ponteiro singleton da classe core
	*/
    static Core *getInstance();

    // Destroy the Core object.
    /**
    * @brief destrutor do objeto Core
    */
    static void destroy();

	static int exec();

    // Loads and execute a Lua script
    /**
    * @brief Mostra a janela
    * @remarks chama o metodo showMaximized() da classe QMainWindow
    *
    */
    void show();

	/*
	* @brief Usado pra abrir a janela, ativar eventos de resize e esconder janela.
	* @remarks Foi preciso criá-lo pra evitar lost device no directX e abrir o renderView da forma correta
	*/
	void captureResizeEvents();

    // Update Core
    /**
    * @brief Atualiza a cena
    * @remarks Atualiza chamando os metodos InputHandler::processEvents(),
    * terraVRVM::updateWorld(), GAudio::update() e GRender::updateWorld()
    * @return true se os metodos terraVRVM::updateWorld() e
    GRender::updateWorld()
    * executaram sem problema
    */
    //bool update();

    /**
    * @brief Obtem o ponteiro da MainWindow
    */
     MainWindow* getMainWindow();

	 /**
    * @brief Obtem o ponteiro da Logic
    */
     Logic* getLogic();

    /**
    * @brief Obtem o ponteiro da RenderEngine
    */
     Engine* getGameEngine();

    /**
    * @brief Obtem o ponteiro do InputHandler
    */
     InputManager* getInputHandler();

private slots:
	void updateSizeMainWindow(int w, int h);
	void setMousePosition(bool active);

protected:
	/// Trata um evento do tipo QTimerEvent
    void timerEvent(QTimerEvent* evt);

// PRIVATE //////////////////////////////////////////////////////////////////
private:

    virtual ~Core();

	bool updateKernel();

	void myConnects();

    /// Static pointer to Core.
	static Core *sCore;

    MainWindow* mWindow;
    Engine* mEngine;
    InputManager* mInput;
	Logic* mLogic;

	bool isAllInitialized;
};

#endif
