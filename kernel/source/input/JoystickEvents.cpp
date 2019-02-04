#include "JoystickEvents.h"

int JoystickEvents::contID = 0;

JoystickEvents::JoystickEvents()
{
	isToAdd = true;
	contID++;
	iD = QString::number(contID);
}

JoystickEvents::~JoystickEvents() 
{
}

/** @remarks Joystick button down event */
bool JoystickEvents::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{
	return true;
}
		
/** @remarks Joystick button up event */
bool JoystickEvents::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
	return true;
}

/** @remarks Joystick axis moved event */
bool JoystickEvents::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{
	return true;
}

//-- Not so common control events, so are not required --//
//! Joystick Event, and sliderID
bool JoystickEvents::sliderMoved( const OIS::JoyStickEvent &, int index)
{
	return true;
}
		
//! Joystick Event, and povID
bool JoystickEvents::povMoved( const OIS::JoyStickEvent &arg, int index)
{
	return true;
}

bool JoystickEvents::vector3Moved( const OIS::JoyStickEvent &arg, int index) 
{
	return true;
}
