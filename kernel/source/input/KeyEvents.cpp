#include "KeyEvents.h"

int KeyEvents::contID = 0;

KeyEvents::KeyEvents()
{
	isToAdd = true;
	contID++;
	iD = QString::number(contID);
}

KeyEvents::~KeyEvents()
{

}

bool KeyEvents::keyPressed(const OIS::KeyEvent &arg)
{
	return true;
}

bool KeyEvents::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}