#include "engine/SoundEngine.h"

#include "OgreALSoundManager.h"
#include "OgreALSound.h"

SoundEngine* SoundEngine::instance = nullptr;

SoundEngine* SoundEngine::getInstance()
{
	if (!instance)
	{
		instance = new SoundEngine();
	}

	return instance;
}

SoundEngine::SoundEngine()
	: mSoundManager(nullptr)
{
	
}

SoundEngine::~SoundEngine()
{
	if (mSoundManager)
	{
		delete mSoundManager;
		mSoundManager = nullptr;
	}
}

void SoundEngine::init()
{
	mSoundManager = new OgreAL::SoundManager();
}

void SoundEngine::destroy()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void SoundEngine::destroySound(OgreAL::Sound* s)
{
	mSoundManager->destroySound(s);
}

OgreAL::Sound* SoundEngine::createSound(const std::string& name, const std::string& fileName, bool loop, bool stream)
{
	return mSoundManager->createSound(name, fileName, loop, stream); 
}

OgreAL::Listener* SoundEngine::getListener()
{
	return mSoundManager->getListener();
}

void SoundEngine::pauseAll()
{
	mSoundManager->pauseAllSounds();
}

void SoundEngine::resumeAll()
{
	mSoundManager->resumeAllSounds();
}