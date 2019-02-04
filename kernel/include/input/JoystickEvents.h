#ifndef JOYSTICK_EVENTS_H
#define JOYSTICK_EVENTS_H

#include <OISJoyStick.h>
#include <QString>
#include "KernelPrerequisites.h"

class InputManager;

class KERNEL_VS_EXPORT JoystickEvents
{
public:
	JoystickEvents();

	virtual ~JoystickEvents();

	/** @remarks Joystick button down event */
	virtual bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
		
	/** @remarks Joystick button up event */
	virtual bool buttonReleased( const OIS::JoyStickEvent &arg, int button );

	/** @remarks Joystick axis moved event */
	virtual bool axisMoved( const OIS::JoyStickEvent &arg, int axis );

	//-- Not so common control events, so are not required --//
	//! Joystick Event, and sliderID
	virtual bool sliderMoved( const OIS::JoyStickEvent &, int index);
		
	//! Joystick Event, and povID
	virtual bool povMoved( const OIS::JoyStickEvent &arg, int index);

	virtual bool vector3Moved( const OIS::JoyStickEvent &arg, int index);

	friend class InputManager;
	

private:
	static int contID;

	bool isToAdd;
	QString iD;
};

#endif