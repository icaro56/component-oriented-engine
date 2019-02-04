#include "MouseEvents.h"

int MouseEvents::contID = 0;

MouseEvents::MouseEvents()
{
	isToAdd = true;
	contID++;
	iD = QString::number(contID);
}

MouseEvents::~MouseEvents() 
{

}

bool MouseEvents::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

bool MouseEvents::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

bool MouseEvents::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}
