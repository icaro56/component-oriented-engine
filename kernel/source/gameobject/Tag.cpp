#include "gameobject/Tag.h"

TagManager* TagManager::instance = nullptr;

TagManager* TagManager::getInstance()
{
	if (!instance)
	{
		instance = new TagManager();
	}

	return instance;
}

TagManager::TagManager()
{

}

TagManager::~TagManager()
{

}

void TagManager::init()
{
	createDefaultTag("Untagged", Tag::Untagged);
	createDefaultTag("Respawn", Tag::Respawn);
	createDefaultTag("Finish", Tag::Finish);
	createDefaultTag("MainCamera", Tag::MainCamera);
	createDefaultTag("Player", Tag::Player);
	createDefaultTag("GameController", Tag::GameController);
}

void TagManager::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

void TagManager::createTag(const std::string& name)
{
	if (!allTags.count(name))
	{
		allTags[name] = Tag(1 << (allTags.size()-1));
	}
}

void TagManager::destroyTag(const std::string& name)
{
	if (allTags.count(name))
	{
		allTags.erase(name);
	}
}

void TagManager::createDefaultTag(const std::string& name, Tag tag)
{
	allTags[name] = tag;
}

Tag TagManager::getTagByName(const std::string& name)
{
	if (allTags.count(name))
	{
		return allTags[name];
	}

	return Tag::Untagged;
}

