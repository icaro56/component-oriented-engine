#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include "KernelPrerequisites.h"
#include <string>

namespace OgreAL
{
	class SoundManager;
	class Sound;
	class Listener;
}

class KERNEL_VS_EXPORT SoundEngine
{
public:
	static SoundEngine* getInstance();
	void init();
	void destroy();
	void destroySound(OgreAL::Sound* s);
	OgreAL::Sound* createSound(const std::string& name, const std::string& fileName, bool loop = false, bool stream = false);
	OgreAL::Listener* getListener();

	void pauseAll();
	void resumeAll();

private:
	SoundEngine();
	~SoundEngine();

private:
	static SoundEngine* instance;

	OgreAL::SoundManager* mSoundManager;
};

#endif