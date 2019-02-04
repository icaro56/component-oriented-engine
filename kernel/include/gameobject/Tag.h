#ifndef TAG_H
#define TAG_H

#include <map>
#include "KernelPrerequisites.h"

enum class Tag
{
	Untagged = 0,
	Respawn = (1 << 0),
	Finish = (1 << 1),
	MainCamera = (1 << 2),
	Player = (1 << 3),
	GameController = (1 << 4),
};

class KERNEL_VS_EXPORT TagManager
{
public:
	static TagManager* getInstance();
	void init();
	void destroy();

	void createTag(const std::string& name);
	void destroyTag(const std::string& name);
	Tag getTagByName(const std::string& name);

private:
	TagManager();
	~TagManager();

	void createDefaultTag(const std::string& name, Tag tag);

private:
	static TagManager* instance;

	typedef std::map<std::string, Tag> TagMap;
	TagMap allTags;
};

#endif