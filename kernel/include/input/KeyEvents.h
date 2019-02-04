#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H

#include <OISKeyboard.h>
#include <QString>
#include "KernelPrerequisites.h"

class InputManager;

class KERNEL_VS_EXPORT KeyEvents
{
public:
	KeyEvents();
	virtual ~KeyEvents();
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);	

	friend class InputManager;

private:
	static int contID;

	bool isToAdd;
	QString iD;
};

#endif