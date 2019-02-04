#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#define OIS_DYNAMIC_LIB
#include <OIS.h>

/*#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>*/

#include "KernelPrerequisites.h"

#include "../Core.h"

#include <QMap>
#include <map>
#include <vector>

#define Input		Core::getInstance()->getInputHandler()

class MainWindow;
class Engine;
class RenderView;
class MouseEvents;
class KeyEvents;
class JoystickEvents;

/**
* @def GInput
* @brief Instancia um objeto InputHandler
* @details inicia um ponteiro para a classe InputHandler uma unica
* vez, caso esse ponteiro seja NULL senão ele apenas retorna o ponteiro.
* @return Ponteiro InputHandler
*/


namespace Ogre
{
	class Vector2;
}

namespace OIS
{
	class InputManager;
}


///////////////////////////////////////////////////////////
// CLASS InputHandler
///////////////////////////////////////////////////////////

/**
 * Input handler.
 *
 * Manages mouse and keyboard input.
 * @see  OIS::MouseListener
 * @see  OIS::KeyListener
*/
class KERNEL_VS_EXPORT InputManager : public OIS::MouseListener, public OIS::KeyListener, public OIS::JoyStickListener
{
    // PUBLIC ///////////////////////////////////////////////
public:
	typedef QMap<QString, KeyEvents*> KeyMap;
	typedef QMap<QString, MouseEvents*> MouseMap;
	typedef QMap<QString, JoystickEvents*> JoyStickMap;

	struct KeyStatus
	{
		KeyStatus()
		{
			down = false;
			up = false;
		}

		bool down;
		bool up;
	};

	typedef std::map< OIS::KeyCode, KeyStatus> KeyState;
	typedef std::map< OIS::MouseButtonID, KeyStatus> MouseState;

	

    /**
    * Constructors.
		\param isWindow especifica se o input funciona em uma janela ou em um widget.
		Se no (0, 0) da janela está o render ou se o render se encontra transladado em um widget filho
    */
    InputManager(size_t hWnd, const bool isExclusive = false, const bool isForeground = true, const bool isTopWindow = false);
    /**
     * Destructor.
     */
    ~InputManager();

    /*!
        \brief configura o renderView do Input
    */
    void setRenderView(RenderView* rView);
	RenderView* getRenderView();

    /**
     * Proccess input events. 
     */
    void processEvents();

	/**
	* Proccess input events que será usado sem ter condições foreground e background. 
	* Geralmente usado para evitar bugs no mouse na hora de troca de contexto
	*/
	void processEventsForInactiveWindow();

    /**
     * Returns the X position of the mouse.
     */
    int mouseX();


    /**
       * Returns the Y position of the mouse.
       */
    int mouseY();

    /**
       * Returns the Z position of the mouse (the mouse wheel position).
       */
    int mouseZ();

	/**
     * Retorna posição X antes da captura do mouse
     */
    int mouseOldX();


    /**
       * Retorna posição Y antes da captura do mouse
       */
    int mouseOldY();

    /**
       * Retorna posição Y antes da captura do mouse
       */
    int mouseOldZ();

    /**
     * Returns the relative X position (X speed) of the mouse
     * since the last call to mouseXSpeed.
     */
    int mouseXSpeed();

    /**
       * Returns the relative Y position (Y speed) of the mouse
       * since the last call to mouseYSpeed.
       */
    int mouseYSpeed();

    /**
       * Returns the relative Z position - mouse wheel position -
    (Z speed) of the mouse
       * since the last call to mouseZSpeed.
       */
    int mouseZSpeed();

    /**
       * Return the relative X position since last frame
       */
    int mouseXDelta();

    /**
       * Return the relative X position since last frame
       */
    int mouseYDelta();

    /**
       * Return the relative X position since last frame
       */
    int mouseZDelta();

    /**
     * Returns true if the mouse button is currently pressed.
     * \param b Button to check.
     */
	bool getMouseButton(const OIS::MouseButtonID b);

	 /**
     * 	Returns true during the frame the user pressed the given mouse button.
     */
	bool getMouseButtonDown(const OIS::MouseButtonID b);

	 /**
     * Returns true during the frame the user releases the given mouse button.
     */
	bool getMouseButtonUp(const OIS::MouseButtonID b);

    void moveMouse(int x, int y);

	void restoreOldPosition();

	/*!
		\brief Returns true during the frame the user starts pressing down the key identified
	*/
	bool getKeyDown(const OIS::KeyCode k);

	/**
     * Returns true if the key is currently pressed.
     * \param k Key to check.
     */
	bool getKey(const OIS::KeyCode k);

	/*!
		\brief Returns true during the frame the user releases the key identified
	*/
	bool getKeyUp(const OIS::KeyCode k);

	void addKeyListener( KeyEvents *keyListener);
    void addMouseListener( MouseEvents *mouseListener);
    void addJoystickListener( JoystickEvents *joystickListener);

	void removeKeyListener( KeyEvents *keyListener);
    void removeMouseListener( MouseEvents *mouseListener);
    void removeJoystickListener( JoystickEvents *joystickListener);

    void removeAllListeners( void );
    void removeAllKeyListeners( void );
    void removeAllMouseListeners( void );
    void removeAllJoystickListeners( void );

    void resizeWindow(int w, int h);

    OIS::Mouse*    getMouse( void );
    OIS::Keyboard* getKeyboard( void );
    OIS::JoyStick* getJoystick( unsigned int index );

    int getNumOfJoysticks( void );

	void grabMouse(bool grab);

    bool isMouseGrabbed();

    // PRIVATE //////////////////////////////////////////////
private:
	void checkPosition();

	void updatePosition();

    int mMouseX, mMouseXSpeed, mMouseXDelta;
    int mMouseY, mMouseYSpeed, mMouseYDelta;
    int mMouseZ, mMouseZSpeed, mMouseZDelta;

    //
    bool mouseGrabbed;
    int oldMouseX, oldMouseY, oldMouseZ;

    ///RenderView
    RenderView* renderView;

	InputManager( const InputManager& ) { }
    InputManager & operator = ( const InputManager& );

    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

	bool vector3Moved( const OIS::JoyStickEvent &arg, int index);
    bool povMoved( const OIS::JoyStickEvent &e, int pov );
    bool axisMoved( const OIS::JoyStickEvent &e, int axis );
    bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
    bool buttonPressed( const OIS::JoyStickEvent &e, int button );
    bool buttonReleased( const OIS::JoyStickEvent &e, int button );

	//método responsável adicionar e remover listener ao final do loop do input
	void updateListenersState();

	 /// InputManager from OIS.
    OIS::InputManager *mInputManager;
    /// Mouse from OIS.
    OIS::Mouse *mMouse;
    /// Keyboard from OIS.
    OIS::Keyboard *mKeyboard;

	std::vector<OIS::JoyStick*> mJoysticks;
    std::vector<OIS::JoyStick*>::iterator itJoystick;
    std::vector<OIS::JoyStick*>::iterator itJoystickEnd;

    KeyMap mKeyListeners;
    MouseMap mMouseListeners;
    JoyStickMap mJoystickListeners;

	//maps que armazenam quais listeners serão removidos e adicionados
	KeyMap mKeyListenersAddDelList;
    MouseMap mMouseListenersAddDelList;
    JoyStickMap mJoystickListenersAddDelList;

	/*KeyMap mKeyListenersDelList;
    MouseMap mMouseListenersDelList;
    JoyStickMap mJoystickListenersDelList;*/

	KeyState mKeyState;
	MouseState mMouseState;

	size_t myHWnd;

	bool topWindow;
};

#endif
