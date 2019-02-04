#include "UiRepresentationManager.h"
#include "UiRepresentation.h"
#include "UiException.h"

#include "utils/Functions.h"
#include <input/InputManager.h>

#include <iostream>

UiRepresentationManager* UiRepresentationManager::instance = 0;

//-----------------------------------------------------------------------------
UiRepresentationManager::UiRepresentationManager()
{
	repEdition = 0;
	Input->addKeyListener(this);
	Input->addMouseListener(this);
}

//-----------------------------------------------------------------------------
UiRepresentationManager::~UiRepresentationManager()
{
	repEdition = 0;
	Input->removeKeyListener(this);
	Input->removeMouseListener(this);
}

//-----------------------------------------------------------------------------
UiRepresentationManager* UiRepresentationManager::getInstance()
{
	if (!instance)
	{
		instance = new UiRepresentationManager();
	}
	return instance;
}

//-----------------------------------------------------------------------------
void UiRepresentationManager::init()
{
	
}

//-----------------------------------------------------------------------------
void UiRepresentationManager::destroy()
{
	listUiRep.clear();
}

//-----------------------------------------------------------------------------
void UiRepresentationManager::addUi(UiRepresentation* v)
{
	if(!listUiRep.contains(v->getType()))
	{
		listUiRep.insert(v->getType(),v);
	}
	else
	{
		QString str = QObject::tr("Enumerador replicado.");
		std::cout << str.toStdString() << std::endl;
		UiException e;
		e.setMessage(str);
		throw e;
	}
}

//-----------------------------------------------------------------------------
void UiRepresentationManager::removeUi(UiRepresentation* v)
{
	if(listUiRep.contains(v->getType()))
	{
		listUiRep.remove(v->getType());
	}
}

//-----------------------------------------------------------------------------
bool UiRepresentationManager::keyPressed(const OIS::KeyEvent &arg)
{
	if(repEdition)
	{
		return repEdition->keyPressed(arg);
	}
	else
	{
		foreach(UiRepresentation * rep,listUiRep)
		{
			if(rep->isActiveRepresentation() && rep->isVisibledRepresentation())
			{
				rep->keyPressed(arg);
			}
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
bool UiRepresentationManager::keyReleased(const OIS::KeyEvent &arg)
{
	if(repEdition)
	{
		return repEdition->keyReleased(arg);
	}
	else
	{
		foreach(UiRepresentation * rep,listUiRep)
		{
			if(rep->isActiveRepresentation() && rep->isVisibledRepresentation())
			{
				rep->keyReleased(arg);
			}
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
bool UiRepresentationManager::mouseMoved( const OIS::MouseEvent &arg )
{
	if(repEdition)
	{
		return repEdition->mouseMoved(arg);
	}
	else
	{
		foreach(UiRepresentation * rep,listUiRep)
		{
			if(rep->isActiveRepresentation() && rep->isVisibledRepresentation())
			{
				rep->mouseMoved(arg);
			}
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
bool UiRepresentationManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	QString idName;
	bool cond = Collision2::hasCollision(arg.state.X.abs,arg.state.Y.abs,idName);

	if(repEdition)
	{
		return repEdition->mouseMoved(arg);
	}
	else
	{
		foreach(UiRepresentation * rep,listUiRep)
		{
			if(rep->isActiveRepresentation() && 
				rep->isVisibledRepresentation() &&
				rep->isValidName(idName))
			{
				return rep->mousePressed(arg,id,idName);				
			}
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
bool UiRepresentationManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	QString idName;	
	bool cond = Collision2::hasCollision(arg.state.X.abs,arg.state.Y.abs,idName);

	if(cond)
	{
		if(repEdition)
		{
			repEdition->mouseReleased(arg,id);
			return true;
		}
		else
		{
			foreach(UiRepresentation * rep,listUiRep)
			{		
				if(rep->isActiveRepresentation() && 
					rep->isVisibledRepresentation() &&
					rep->isValidName(idName))
				{
					return rep->mouseReleased(arg,id,idName);
				}
			}
		}		
	}
	else
	{
	}

	return true;
}

//-----------------------------------------------------------------------------
void UiRepresentationManager::updateRepresentation()
{
	foreach(UiRepresentation * rep,listUiRep)
	{
		if(rep->isActiveRepresentation())
		{
			rep->updateRepresentation();
		}
	}
}

//-----------------------------------------------------------------------------
void UiRepresentationManager::setEdition(UiRepresentation* rep,bool cond)
{
	if(cond)
	{
		if(repEdition != rep)
		{
			repEdition->endEditing();
			rep->startEditing();
		}
	}
	else
	{
		if(repEdition == rep)
		{
			repEdition->endEditing();
			repEdition = 0;
		}
	}
}

