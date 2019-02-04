
#pragma warning (disable : 4786)

#include "GameObjectTemplateManager.h"

#include "ComponentTemplate.h"

/*static*/ GameObjectTemplateManager *GameObjectTemplateManager::mInstance = nullptr;

GameObjectTemplateManager::GameObjectTemplateManager() 
{
	//criando templates default
	GameObjectTemplate* gt1 = new GameObjectTemplate("BoxPrefab");
	BoxPrefab* box = new BoxPrefab();
	gt1->setCompSetTemplate(box);
	registerTemplate(gt1);

	gt1 = new GameObjectTemplate("SpherePrefab");
	SpherePrefab* s = new SpherePrefab();
	gt1->setCompSetTemplate(s);
	registerTemplate(gt1);

	gt1 = new GameObjectTemplate("CapsulePrefab");
	CapsulePrefab* c = new CapsulePrefab();
	gt1->setCompSetTemplate(c);
	registerTemplate(gt1);
}

GameObjectTemplateManager::~GameObjectTemplateManager() 
{

}

GameObjectTemplateManager *GameObjectTemplateManager::getInstance() 
{
	if(mInstance == NULL)
		mInstance = new GameObjectTemplateManager();

	return mInstance;
}

void GameObjectTemplateManager::registerTemplate(GameObjectTemplate *templ) 
{
	mObjectTemplates.push_back(templ);
}

void GameObjectTemplateManager::clearTemplates() 
{
	for(int i=0; i<mObjectTemplates.size(); i++) 
	{
		mObjectTemplates[i]->clearComponents();
		delete mObjectTemplates[i];
	}
}


GameObject *GameObjectTemplateManager::createObject(std::string templateName, const std::string &objectID) 
{
	int i = 0;
	while(i<mObjectTemplates.size()) 
	{
		if(mObjectTemplates[i]->name() == templateName)
			break;
		++i;
	}

	if(i==mObjectTemplates.size())
		return NULL;
		
	GameObject *newObj = new GameObject(objectID);
	ComponentSetTemplate* components = mObjectTemplates[i]->componentSetTemplate();
	components->makeComponent(newObj);

	return newObj;
}
