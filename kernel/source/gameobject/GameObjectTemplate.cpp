#include "GameObjectTemplate.h"

GameObjectTemplate::GameObjectTemplate( const std::string& name) 
	: myComponentSetTemplate(nullptr)
{
	mName = name;
}

GameObjectTemplate::~GameObjectTemplate() 
{

}

void GameObjectTemplate::clearComponents() 
{
	if (myComponentSetTemplate)
	{
		delete myComponentSetTemplate;
		myComponentSetTemplate = nullptr;
	}
}

const std::string& GameObjectTemplate::name() const 
{ 
	return mName; 
}

void GameObjectTemplate::setName(const std::string& name) 
{ 
	mName = name; 
}

ComponentSetTemplate* GameObjectTemplate::componentSetTemplate()
{ 
	return myComponentSetTemplate; 
}
	
void GameObjectTemplate::setCompSetTemplate( ComponentSetTemplate* compTemplate)
{
	myComponentSetTemplate = compTemplate;
}