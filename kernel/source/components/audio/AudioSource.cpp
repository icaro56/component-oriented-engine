#include "audio\AudioSource.h"

#include "transform/Transform.h"
#include "gameobject/GameObject.h"

#include "engine/SoundEngine.h"
#include "OgreALSound.h"
#include "utils/KernelConverter.h"

AudioSource::AudioSource()
	: Component("AudioSource", "AudioSource"),
	  sound(nullptr),
	  lastVolBeforeMute(1),
	  mIsMute(false),
	  enabled(true)
{

}

AudioSource::AudioSource(std::string idComponent)
	: Component(idComponent, "AudioSource"),
	  sound(nullptr),
	  lastVolBeforeMute(1),
	  mIsMute(false),
	  enabled(true)
{

}

AudioSource::AudioSource(const ComponentDescMap &d)
	: Component("AudioSource", "AudioSource"),
	  sound(nullptr),
	  mAudioSourceDescMap(d),
	  lastVolBeforeMute(1),
	  mIsMute(false),
	  enabled(true)
{

}

AudioSource::~AudioSource()
{
	if (sound)
	{
		sound->detachFromParent();
		SoundEngine::getInstance()->destroySound(sound);
		sound = nullptr;
	}
}

void AudioSource::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void AudioSource::init(const std::string& name)
{
	if (!initialized)
	{
		if (Ogre::ResourceGroupManager::getSingleton().resourceExists( "General", getAudioFileName() ))
		{
			sound = SoundEngine::getInstance()->createSound(name + "_" + componentID(), getAudioFileName());
		
			if (transform())
			{
				transform()->_attachObject(sound);
				initialized = true;

				if (mAudioSourceDescMap.size())
					applyDescription(mAudioSourceDescMap);
			}
		}
	}
}

OgreAL::Sound* AudioSource::getOgreALSound()
{
	return sound;
}

bool AudioSource::isPlaying()
{
	return sound->isPlaying();
}

void AudioSource::setLoop(bool loop)
{
	if (initialized)
	{
		sound->setLoop(loop);
	}
	else
	{
		mAudioSourceDescMap[ASD_Loop] = Convert::toString(loop);
	}
}

bool AudioSource::isLoop()
{
	return sound->isLooping();
}

void AudioSource::setMute(bool mute)
{
	mIsMute = mute;
	if (initialized)
	{
		if (mute)
		{
			lastVolBeforeMute = sound->getGain();
			sound->setGain(0);
		}
		else
		{
			sound->setGain(lastVolBeforeMute);
		}
	}
}

bool AudioSource::isMute()
{
	return mIsMute;
}

void AudioSource::setPitch(double p)
{
	if (initialized)
	{
		sound->setPitch(p);
	}
	else
	{
		mAudioSourceDescMap[ASD_Pitch] = Convert::toString(p);
	}
}

double AudioSource::getPitch()
{
	return sound->getPitch();
}

void AudioSource::setVolume(float v)
{
	if (initialized)
	{
		sound->setGain(v);
	}
	else
	{
		mAudioSourceDescMap[ASD_Volume] = Convert::toString(v);
	}
}

float AudioSource::getVolume()
{
	return sound->getGain();
}

void AudioSource::pause()
{
	if (initialized)
	{
		sound->pause();
	}
}

void AudioSource::play()
{
	if (initialized)
	{
		sound->play();
	}
}

void AudioSource::stop()
{
	if (initialized)
	{
		sound->stop();
	}
}

void AudioSource::setAudioFileName(const std::string& name)
{
	filename = name;
}

std::string AudioSource::getAudioFileName()
{
	return filename;
}

void AudioSource::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case ASD_Loop:
		setLoop(Convert::parseBool(value));
		break;

	case ASD_Pitch:
		setPitch(Convert::parseReal(value));
		break;

	case ASD_Volume:
		setVolume(Convert::parseReal(value));
		break;

	default:
		break;
	}
}

void AudioSource::setEnabled(bool enable)
{
	if (enable)
		play();
	else
		pause();

	enabled = enable;
}

bool AudioSource::isEnabled()
{
	return enabled;
}