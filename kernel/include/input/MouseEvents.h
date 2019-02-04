#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include <OISMouse.h>
#include <QString>
#include "KernelPrerequisites.h"

class InputManager;

class KERNEL_VS_EXPORT MouseEvents
{
public:
	MouseEvents();
	virtual ~MouseEvents();
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	friend class InputManager;

private:
	static int contID;

	bool isToAdd;
	QString iD;
};

#endif