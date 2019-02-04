#include "AvatarManager.h"

#include "Avatar.h"

AvatarManager* AvatarManager::instance = 0;

AvatarManager* AvatarManager::getInstance()
{
	if (!instance)
	{
		instance = new AvatarManager();
	}

	return instance;
}

void AvatarManager::updateAvatars()
{
	if (mainAvatar && mainAvatar->isActived())
	{
		mainAvatar->move();
	}
}

void AvatarManager::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

AvatarManager::AvatarManager()
	: mainAvatar(0)
{

}

AvatarManager::~AvatarManager()
{
	if (mainAvatar)
	{
		mainAvatar = 0;
	}

	destroyAllAvatars();
}

void AvatarManager::destroyAllAvatars()
{
	if (!avatars.empty())
	{
		QMapIterator<QString, Avatar*> i(avatars);
        while (i.hasNext())
        {
            i.next();

            Avatar* temp = i.value();
            delete temp;
            temp = 0;
        }

        avatars.clear();
	}
}

Avatar* AvatarManager::getMainAvatar()
{
	return mainAvatar;
}

void AvatarManager::setMainAvatar(const QString& name)
{
	if (avatars.contains(name))
	{
		//avatar antigo
		if (mainAvatar)
		{
			mainAvatar->setActive(false);
		}

		mainAvatar = avatars.value(name);
	}
}

void AvatarManager::setActiveMainAvatar(bool a)
{
	if (mainAvatar)
	{
		mainAvatar->setActive(a);
	}
}

void AvatarManager::createAvatar(const QString& name, bool isMain)
{
	if (!avatars.contains(name))
	{
		Avatar* newAvatar = new Avatar(name);
		newAvatar->setActive(false);

		avatars.insert(name, newAvatar);

		if (isMain)
		{
			setMainAvatar(name);
		}
	}
	else
	{
		std::cerr << "\nEsse avatar já existe";
	}
}