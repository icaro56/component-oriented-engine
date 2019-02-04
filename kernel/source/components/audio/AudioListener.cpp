#include "audio\AudioListener.h"

#include "engine/SoundEngine.h"
#include "transform/Transform.h"
#include "gameobject/GameObject.h"

#include "OgreALListener.h"

AudioListener::AudioListener()
	: Component("AudioListener", "AudioListener")
{

}

AudioListener::AudioListener(std::string idComponent)
	: Component(idComponent, "AudioListener")
{

}

AudioListener::~AudioListener()
{

}

void AudioListener::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void AudioListener::init(const std::string& name)
{
	if (!initialized)
	{	
		if (transform())
		{
			enabled = true;
			transform()->_attachObject(SoundEngine::getInstance()->getListener());
			initialized = true;

			/*if (createdByDesc)
				applyDescription();*/
		}
	}
}

void AudioListener::setEnabled(bool enable)
{
	if (enable)
		SoundEngine::getInstance()->resumeAll();
	else
		SoundEngine::getInstance()->pauseAll();

	enabled = enable;
}

bool AudioListener::isEnabled()
{
	return enabled;
}
