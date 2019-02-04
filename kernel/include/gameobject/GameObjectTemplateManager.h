
#ifndef __OBJTEMPLATEMGR_HPP__
#define __OBJTEMPLATEMGR_HPP__

#include "KernelPrerequisites.h"
#include <vector>
#include <string>

#include "GameObject.h"
#include "GameObjectTemplate.h"

class KERNEL_VS_EXPORT GameObjectTemplateManager 
{
protected:
	typedef std::vector<GameObjectTemplate*> GameObjectTemplateList;

	GameObjectTemplateManager();
public:
	~GameObjectTemplateManager();

	static GameObjectTemplateManager *getInstance();

	void registerTemplate(GameObjectTemplate *templ);

	void clearTemplates();

	GameObject *createObject(std::string templateName, const std::string &objectID);

protected:
	GameObjectTemplateList mObjectTemplates;

	static GameObjectTemplateManager *mInstance;
};

#endif
