#include "InputManager.h"

#include "../engine/Engine.h"
#include "../engine/RenderView.h"
#include "../MainWindow.h"

//#include <OISInputManager.h>

#include <OgreStringConverter.h>
#include <OgreVector2.h>
//#include <MyGUI.h>
#include "windows.h"
#include "MouseEvents.h"
#include "KeyEvents.h"
#include "JoystickEvents.h"

#include "utils/KernelConverter.h"

//hWnd: the window system identifier of the widget.
//usado para podermos escutar eventos de outra widget
//(nao so da janela principal)
InputManager::InputManager(size_t hWnd, const bool isExclusive, const bool isForeground, const bool isTopWindow)
	: topWindow(isTopWindow),
	  myHWnd( hWnd)
{
	
    mMouseXSpeed = mMouseYSpeed = mMouseZ = mMouseZSpeed = 0;
    mMouseX = mMouseY = 10;

    // create OIS input manager
    OIS::ParamList pl;
    //size_t hWnd = GWindow->getWindowHandler();
    pl.insert(OIS::ParamList::value_type("WINDOW",
                                         Convert::toString(hWnd)));
    
	//funciona apenas com a janela em foco
    //pl.insert(OIS::ParamList::value_type("w32_mouse", "DISCL_FOREGROUND"));
    //funciona mesmo se a janela está sem foco
    //pl.insert(OIS::ParamList::value_type("w32_mouse", "DISCL_BACKGROUND"));

	std::string arg1 = isExclusive ? "DISCL_EXCLUSIVE" : "DISCL_NONEXCLUSIVE";
	std::string arg2 = isForeground ? "DISCL_FOREGROUND" : "DISCL_BACKGROUND";


	pl.insert(OIS::ParamList::value_type("w32_mouse", arg2));
    pl.insert(OIS::ParamList::value_type("w32_mouse", arg1));
	//keyboard background ocasiona em bugs. Acentos não aparecem
	pl.insert(OIS::ParamList::value_type("w32_keyboard", "DISCL_FOREGROUND"));
    pl.insert(OIS::ParamList::value_type("w32_keyboard", arg1));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    // init mouse
    mMouse = NULL;

    if(mInputManager->getNumberOfDevices(OIS::OISMouse) > 0)
    {
        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(
                                              OIS::OISMouse,
                                              true));
        mMouse->setEventCallback(this);
        mouseGrabbed = false;
        oldMouseX = oldMouseY = 10;
		oldMouseZ = 0;
    }

    // init keyboard
    mKeyboard = NULL;

    if(mInputManager->getNumberOfDevices(OIS::OISKeyboard) > 0)
    {
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager
                                                ->createInputObject(OIS::OISKeyboard,
                                                        true));
        mKeyboard->setEventCallback(this);
    }

    // init joystick
    // If possible create all joysticks in buffered mode
    if( mInputManager->getNumberOfDevices(OIS::OISJoyStick) > 0 ) 
	{
        mJoysticks.resize( mInputManager->getNumberOfDevices(OIS::OISJoyStick) );

        itJoystick    = mJoysticks.begin();
        itJoystickEnd = mJoysticks.end();
        for(; itJoystick != itJoystickEnd; ++itJoystick ) 
		{
            (*itJoystick) = static_cast<OIS::JoyStick*>( mInputManager->createInputObject( OIS::OISJoyStick, true ) );
            (*itJoystick)->setEventCallback( this );
        }
    }

    //Pegando o tamanho da barra de titulo padrão do windows para calculo
    //da posição do mouse
    //heightTitlebar = GetSystemMetrics(SM_CYSIZE) + 1;
}

//---------------------------------------------------------
InputManager::~InputManager()
{
	if (mInputManager)
	{
		// destroy mouse
		if( mMouse ) 
		{
            mInputManager->destroyInputObject( mMouse );
            mMouse = nullptr;
        }

        if( mKeyboard ) 
		{
            mInputManager->destroyInputObject( mKeyboard );
            mKeyboard = nullptr;
        }

        if( mJoysticks.size() > 0 ) 
		{
            itJoystick    = mJoysticks.begin();
            itJoystickEnd = mJoysticks.end();
            for(; itJoystick != itJoystickEnd; ++itJoystick ) {
                mInputManager->destroyInputObject( *itJoystick );
            }

            mJoysticks.clear();
        }

		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = nullptr;

		// Clear Listeners
        mKeyListeners.clear();
        mMouseListeners.clear();
        mJoystickListeners.clear();

		mKeyListenersAddDelList.clear();
		mMouseListenersAddDelList.clear();
        mJoystickListenersAddDelList.clear();
	}
}

void InputManager::setRenderView(RenderView* rView)
{
    renderView = rView;
}

RenderView* InputManager::getRenderView()
{
	return renderView;
}

//---------------------------------------------------------
void InputManager::processEvents()
{
	updateListenersState();

	//if ((renderView && renderView->isActiveWindow()) || isForeground)
	//{
		//MSG  msg;

		mMouseXDelta = mMouseYDelta = mMouseZDelta = 0;

		/*while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/

		if (mMouse)
		{
			mMouse->capture();
		}

		if (mKeyboard)
		{
			mKeyboard->capture();
		}

		if( mJoysticks.size() > 0 ) 
		{
			itJoystick    = mJoysticks.begin();
			itJoystickEnd = mJoysticks.end();
			for(; itJoystick != itJoystickEnd; ++itJoystick ) {
				(*itJoystick)->capture();
			}
		}

		if (mouseGrabbed)
			restoreOldPosition();

	//}
}

void InputManager::processEventsForInactiveWindow()
{
	mMouseXDelta = mMouseYDelta = mMouseZDelta = 0;

	if (mMouse)
	{
		mMouse->capture();
	}

	if (mKeyboard)
	{
		mKeyboard->capture();
	}

	if( mJoysticks.size() > 0 ) 
	{
		itJoystick    = mJoysticks.begin();
		itJoystickEnd = mJoysticks.end();
		for(; itJoystick != itJoystickEnd; ++itJoystick ) {
			(*itJoystick)->capture();
		}
	}

	if (mouseGrabbed)
		restoreOldPosition();
}

//---------------------------------------------------------
int InputManager::mouseX()
{
    return mMouseX;
}

//---------------------------------------------------------
int InputManager::mouseY()
{
    return mMouseY;
}

//---------------------------------------------------------
int InputManager::mouseZ()
{
    return mMouseZ;
}

//---------------------------------------------------------
int InputManager::mouseOldX()
{
	return oldMouseX;
}

//---------------------------------------------------------
int InputManager::mouseOldY()
{
	return oldMouseY;
}

//---------------------------------------------------------
int InputManager::mouseOldZ()
{
	return oldMouseZ;
}

//---------------------------------------------------------
int InputManager::mouseXSpeed()
{
    int spd = mMouseXSpeed;
    mMouseXSpeed = 0;
    return spd;
}

//---------------------------------------------------------
int InputManager::mouseYSpeed()
{
    int spd = mMouseYSpeed;
    mMouseYSpeed = 0;
    return spd;
}

//---------------------------------------------------------
int InputManager::mouseZSpeed()
{
    int spd = mMouseZSpeed;
    mMouseZSpeed = 0;
    return spd;
}

//---------------------------------------------------------
int InputManager::mouseXDelta()
{
    return mMouseXDelta;
}

//---------------------------------------------------------
int InputManager::mouseYDelta()
{
    return mMouseYDelta;
}

//---------------------------------------------------------
int InputManager::mouseZDelta()
{
    return mMouseZDelta;
}

//---------------------------------------------------------
bool InputManager::getMouseButton(const OIS::MouseButtonID b)
{
	return getMouse()->getMouseState().buttonDown(b);
}

//---------------------------------------------------------
bool InputManager::getMouseButtonDown(const OIS::MouseButtonID b)
{
	if (mMouseState.count(b))
	{
		return mMouseState[b].down;
	}

	return false;
}

//---------------------------------------------------------
bool InputManager::getMouseButtonUp(const OIS::MouseButtonID b)
{
	if (mMouseState.count(b))
	{
		return mMouseState[b].up;
	}

	return false;
}

//---------------------------------------------------------
void InputManager::moveMouse(int x, int y)
{
    mMouseX = x;
    mMouseY = y;

	//checkPosition();

	updatePosition();
}

//---------------------------------------------------------
void InputManager::restoreOldPosition()
{
	moveMouse(oldMouseX, oldMouseY);
}

//---------------------------------------------------------
void InputManager::checkPosition()
{
	//preciso reimplementar essa função, pois a posição que chega é relativa a tela toda
	//GetWindowRect: 

	const OIS::MouseState& ms = mMouse->getMouseState();

    if (mMouseX < 0)
        mMouseX = 0;
    else if (mMouseX >= ms.width)
        mMouseX = ms.width - 1;

    if (mMouseY < 0)
        mMouseY = 0;
    else if (mMouseY >= ms.height)
        mMouseY = ms.height - 1;
}

//---------------------------------------------------------
void InputManager::updatePosition()
{
    SetCursorPos (mMouseX, mMouseY);
}

//---------------------------------------------------------
bool InputManager::mouseMoved(const OIS::MouseEvent &arg)
{
    //if (renderView->isOnRender())
    //{
		const OIS::MouseState &s = arg.state;
		mMouseX = s.X.abs;
		mMouseY = s.Y.abs;
		mMouseZ = s.Z.abs;
		mMouseXSpeed += s.X.rel;
		mMouseYSpeed += s.Y.rel;
		mMouseZSpeed += s.Z.rel;
		mMouseXDelta = s.X.rel;
		mMouseYDelta = s.Y.rel;
		mMouseZDelta = s.Z.rel;

		foreach(MouseEvents* listener, mMouseListeners)
		{
			listener->mouseMoved( arg );
		}

        /*if (GRender->haveMyGUI() && renderView->getHaveMyGUI())
        {
            Ogre::Vector2 pos = renderView->getAbsolutePosition();
            GRender->getMyGUI()->injectMouseMove(arg.state.X.abs - pos.x, arg.state.Y.abs - pos.y, arg.state.Z.abs);
        }*/
      
    //}
    /*else
    {
        if (isMouseGrabbed())
            grabMouse(false);

    }*/
	return true;
}

//---------------------------------------------------------
bool InputManager::mousePressed(const OIS::MouseEvent &arg,
                                OIS::MouseButtonID id)
{   
	foreach(MouseEvents* listener, mMouseListeners)
	{
        listener->mousePressed( arg, id );
    }

    /*if (GRender->haveMyGUI() && renderView->getHaveMyGUI())
    {
        Ogre::Vector2 pos = renderView->getAbsolutePosition();
        GRender->getMyGUI()->injectMousePress(arg.state.X.abs - pos.x, arg.state.Y.abs - pos.y, MyGUI::MouseButton::Enum(id));
	}*/

	mMouseState[id].down = true;

    return true;
}

//---------------------------------------------------------
bool InputManager::mouseReleased(const OIS::MouseEvent &arg,
                                 OIS::MouseButtonID id)
{
	foreach(MouseEvents* listener, mMouseListeners)
	{
        listener->mouseReleased( arg, id );
    }

    /*if (GRender->haveMyGUI() && renderView->getHaveMyGUI())
    {
        Ogre::Vector2 pos = renderView->getAbsolutePosition();
        GRender->getMyGUI()->injectMouseRelease(arg.state.X.abs - pos.x, arg.state.Y.abs - pos.y, MyGUI::MouseButton::Enum(id));
    }*/

	mMouseState[id].up = true;

    return true;
}

//---------------------------------------------------------
bool InputManager::getKey(const OIS::KeyCode k)
{
	return getKeyboard()->isKeyDown(k);
}

//---------------------------------------------------------
bool InputManager::getKeyDown(const OIS::KeyCode k)
{
	if (mKeyState.count(k))
	{
		return mKeyState[k].down;
	}

	return false;
}

//---------------------------------------------------------
bool InputManager::getKeyUp(const OIS::KeyCode k)
{
	if (mKeyState.count(k))
	{
		return mKeyState[k].up;
	}

	return false;
}

//---------------------------------------------------------
void InputManager::addKeyListener( KeyEvents *keyListener ) 
{
    if( mKeyboard ) 
	{
		//checa se há itens duplicados,
		if (!mKeyListenersAddDelList.contains(keyListener->iD))
		{
			keyListener->isToAdd = true;
			mKeyListenersAddDelList.insert(keyListener->iD, keyListener);
		}
		else
		{
			std::cerr << "\nKeylistener duplicado na fila de adicao. Nao adicionado";
		}
    }
}

void InputManager::addMouseListener( MouseEvents *mouseListener) 
{
    if( mMouse ) 
	{
        // Check for duplicate items
		if (!mMouseListenersAddDelList.contains(mouseListener->iD))
		{
			mouseListener->isToAdd = true;
			mMouseListenersAddDelList.insert(mouseListener->iD, mouseListener);
		}
		else
		{
			std::cerr << "\nMouselistener duplicado na fila de adicao. Nao adicionado";
		}
    }
}

void InputManager::addJoystickListener( JoystickEvents *joystickListener) 
{
    if( mJoysticks.size() > 0 ) 
	{
        // Check for duplicate items
		if (!mJoystickListenersAddDelList.contains(joystickListener->iD))
		{
			joystickListener->isToAdd = true;
			mJoystickListenersAddDelList.insert(joystickListener->iD, joystickListener);
		}
		else
		{
			std::cerr << "\nJoysticklistener duplicado na fila de adicao. Nao adicionado";
		}
    }
}

void InputManager::removeKeyListener( KeyEvents *keyListener)
{
	if (mKeyListeners.contains(keyListener->iD) && !mKeyListenersAddDelList.contains(keyListener->iD))
	{
		keyListener->isToAdd = false;
		mKeyListenersAddDelList.insert(keyListener->iD, keyListener);
	}
	else
	{
		std::cerr << "\nKeylistener duplicado na fila de remoção. Nao adicionado";
	}
}

void InputManager::removeMouseListener( MouseEvents *mouseListener )
{
	if (mMouseListeners.contains(mouseListener->iD) && !mMouseListenersAddDelList.contains(mouseListener->iD))
	{
		mouseListener->isToAdd = false;
		mMouseListenersAddDelList.insert(mouseListener->iD, mouseListener);
	}
	else
	{
		std::cerr << "\nMouselistener duplicado na fila de remoção. Nao adicionado";
	}
}

void InputManager::removeJoystickListener( JoystickEvents *joystickListener)
{
	if (mJoystickListeners.contains(joystickListener->iD) && !mJoystickListenersAddDelList.contains(joystickListener->iD))
	{
		joystickListener->isToAdd = false;
		mJoystickListenersAddDelList.insert(joystickListener->iD, joystickListener);
	}
	else
	{
		std::cerr << "\nJoysticklistener duplicado na fila de remoção. Nao adicionado";
	}
}

void InputManager::removeAllListeners( void ) 
{
    mKeyListeners.clear();
    mMouseListeners.clear();
    mJoystickListeners.clear();

	mKeyListenersAddDelList.clear();
    mMouseListenersAddDelList.clear();
    mJoystickListenersAddDelList.clear();
}

void InputManager::removeAllKeyListeners( void ) 
{
    mKeyListeners.clear();
	mKeyListenersAddDelList.clear();
}

void InputManager::removeAllMouseListeners( void ) 
{
    mMouseListeners.clear();
	mMouseListenersAddDelList.clear();
}

void InputManager::removeAllJoystickListeners( void ) 
{
    mJoystickListeners.clear();
	mJoystickListenersAddDelList.clear();
}

OIS::Mouse* InputManager::getMouse( void ) 
{
    return mMouse;
}

OIS::Keyboard* InputManager::getKeyboard( void ) 
{
    return mKeyboard;
}

OIS::JoyStick* InputManager::getJoystick( unsigned int index ) 
{
    // Make sure it's a valid index
    if( index < mJoysticks.size() ) {
        return mJoysticks[ index ];
    }

    return 0;
}

int InputManager::getNumOfJoysticks( void ) 
{
    // Cast to keep compiler happy ^^
    return (int) mJoysticks.size();
}

//---------------------------------------------------------
bool InputManager::keyPressed(const OIS::KeyEvent &arg)
{
	foreach(KeyEvents* listener, mKeyListeners)
	{
		listener->keyPressed( arg );
	}

    /*if (GRender->haveMyGUI())
        GRender->getMyGUI()->injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);*/

	mKeyState[arg.key].down = true;

    return true;
}

//---------------------------------------------------------
bool InputManager::keyReleased(const OIS::KeyEvent &arg)
{
	foreach(KeyEvents* listener, mKeyListeners)
	{
		listener->keyReleased( arg );
	}

    /*if (GRender->haveMyGUI())
        GRender->getMyGUI()->injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));*/

	mKeyState[arg.key].up = true;

    return true;
}

//----------------------------------------------------------
void InputManager::resizeWindow(int w, int h)
{
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = w;
    ms.height = h;
}

//----------------------------------------------------------
void InputManager::grabMouse(bool grab)
{
    mouseGrabbed = grab;
    ShowCursor(!grab);
    
	if (!topWindow)
	{
		POINT p;
		GetCursorPos(&p);
		oldMouseX = p.x;
		oldMouseY = p.y;
	}
	else
	{
		oldMouseX = mMouseX;
		oldMouseY = mMouseY;
	}

	oldMouseZ = mMouseZ;

    mMouseXSpeed = mMouseYSpeed = mMouseZSpeed = 0;
}

//----------------------------------------------------------
bool InputManager::isMouseGrabbed()
{
    return mouseGrabbed;
}

bool InputManager::vector3Moved( const OIS::JoyStickEvent &arg, int index)
{
	foreach(JoystickEvents* listener, mJoystickListeners)
	{
		listener->vector3Moved( arg, index );
	}

    return true;
}

bool InputManager::povMoved( const OIS::JoyStickEvent &e, int pov ) 
{
	foreach(JoystickEvents* listener, mJoystickListeners)
	{
		listener->povMoved( e, pov );
	}

    return true;
}

bool InputManager::axisMoved( const OIS::JoyStickEvent &e, int axis ) 
{
	foreach(JoystickEvents* listener, mJoystickListeners)
	{
		listener->axisMoved( e, axis );
	}

    return true;
}

bool InputManager::sliderMoved( const OIS::JoyStickEvent &e, int sliderID ) 
{
	foreach(JoystickEvents* listener, mJoystickListeners)
	{
		listener->sliderMoved( e, sliderID );
	}

    return true;
}

bool InputManager::buttonPressed( const OIS::JoyStickEvent &e, int button ) 
{
	foreach(JoystickEvents* listener, mJoystickListeners)
	{
		listener->buttonPressed( e, button );
	}

    return true;
}

bool InputManager::buttonReleased( const OIS::JoyStickEvent &e, int button ) 
{
	foreach(JoystickEvents* listener, mJoystickListeners)
	{
		listener->buttonReleased( e, button );
	}

    return true;
}

void InputManager::updateListenersState()
{
	//teclado
	if (!mKeyListenersAddDelList.empty())
	{
		QMapIterator<QString, KeyEvents*> it(mKeyListenersAddDelList);

		while(it.hasNext())
		{
			it.next();

			if (it.value()->isToAdd)
			{
				// Check for duplicate items
				if (!mKeyListeners.contains(it.key()))
				{
					mKeyListeners.insert(it.key(), it.value());
				}
				else 
				{
					// Duplicate Item
					std::cerr << "\nKeyListener duplicado na hora e adicionar";
				}
			}
			else
			{
				// Check for duplicate items
				if (mKeyListeners.contains(it.key()))
				{
					mKeyListeners.remove(it.key());
				}
				else 
				{
					// Duplicate Item
					std::cerr << "\nKeyListener nao encontrado para remocao";
				}
			}
		}

		mKeyListenersAddDelList.clear();
	}

	//mouse
	if (!mMouseListenersAddDelList.empty())
	{
		QMapIterator<QString, MouseEvents*> it(mMouseListenersAddDelList);

		while(it.hasNext())
		{
			it.next();

			if (it.value()->isToAdd)
			{
				// Check for duplicate items
				if (!mMouseListeners.contains(it.key()))
				{
					mMouseListeners.insert(it.key(), it.value());
				}
				else 
				{
					// Duplicate Item
					std::cerr << "\nMouseListener duplicado na hora e adicionar";
				}
			}
			else
			{
				// Check for duplicate items
				if (mMouseListeners.contains(it.key()))
				{
					mMouseListeners.remove(it.key());
				}
				else 
				{
					// Duplicate Item
					std::cerr << "\nMouseListener nao encontrado para remocao";
				}
			}
		}

		mMouseListenersAddDelList.clear();
	}

	//joystick
	if (!mJoystickListenersAddDelList.empty())
	{
		QMapIterator<QString, JoystickEvents*> it(mJoystickListenersAddDelList);

		while(it.hasNext())
		{
			it.next();

			if (it.value()->isToAdd)
			{
				// Check for duplicate items
				if (!mJoystickListeners.contains(it.key()))
				{
					mJoystickListeners.insert(it.key(), it.value());
				}
				else 
				{
					// Duplicate Item
					std::cerr << "\nJoystickListener duplicado na hora e adicionar";
				}
			}
			else
			{
				// Check for duplicate items
				if (mJoystickListeners.contains(it.key()))
				{
					mJoystickListeners.remove(it.key());
				}
				else 
				{
					// Duplicate Item
					std::cerr << "\nJoystickListener nao encontrado para remocao";
				}
			}
		}

		mJoystickListenersAddDelList.clear();
	}

	if (!mKeyState.empty())
	{
		mKeyState.clear();
	}

	if (!mMouseState.empty())
	{
		mMouseState.clear();
	}
}