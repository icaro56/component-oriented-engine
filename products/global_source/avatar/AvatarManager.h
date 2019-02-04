#ifndef AVATARMANAGER_H
#define AVATARMANAGER_H

#include <QMap>

class Avatar;

class AvatarManager
{
public:
	static AvatarManager* getInstance();

	void destroy();

	void updateAvatars();

	void createAvatar(const QString& name, bool isMain = false);
	void destroyAllAvatars();

	Avatar* getMainAvatar();
	void setMainAvatar(const QString& name);

	void setActiveMainAvatar(bool a);

private:
	AvatarManager();
	~AvatarManager();

private:
	static AvatarManager* instance;

	QMap<QString, Avatar*> avatars;

	Avatar* mainAvatar;
};

#endif