#include "miscellaneous/Animation.h"

#include "mesh/Renderer.h"
#include "mesh/SkinnedMeshRenderer.h"

#include "OgreAnimationState.h"
#include "utils/KernelConverter.h"
#include "time/Time.h"

Animation::Animation()
  :	Component("Animation", "Animation"),
	activeAnimationState(nullptr),
	enabled(true),
	speed(1.0)
{

}

Animation::Animation(const ComponentDescMap& d)
  : Component("Animation", "Animation"),
	activeAnimationState(nullptr),
	mAnimationDescMap(d),
	enabled(true),
	speed(1.0)
{

}

Animation::~Animation()
{

}

void Animation::prerequisites()
{

}

void Animation::init(const std::string& name)
{
	if (!initialized && renderer())
	{
		if ( (renderer()->familyID() == "Renderer") && 
			 (renderer()->componentID() == "SkinnedMeshRenderer") )
		{
			SkinnedMeshRenderer* skin = dynamic_cast<SkinnedMeshRenderer*>(renderer());

			requestAnimations(skin);

			initialized = true;

			if (mAnimationDescMap.size())
				applyDescription(mAnimationDescMap);
		}
	}
}

void Animation::nextInit()
{

}

void Animation::setEnabled(bool e)
{
	enabled = e;
}

bool Animation::isEnabled()
{
	return enabled;
}

void Animation::setPlayAutomatically(bool active)
{
	mAnimationDescMap[AD_PlayAutomatically] = Convert::toString(active);
}

void Animation::setAnimationSpeed(double s)
{
	speed = s;
}

void Animation::play(const std::string &name)
{
	if (initialized)
	{
		if (animationMap.count(name))
		{
			if (activeAnimationState)
			{
				stop();
			}

			activeAnimationState = animationMap.at(name);
			activeAnimationState->setEnabled(true);
		}
	}
	else
	{
		mAnimationDescMap[AD_CurrentAnimation] = name;
	}
}

void Animation::stop()
{
	activeAnimationState->setEnabled(false);
}

void Animation::setLoop(bool active)
{
	if (initialized)
	{
		activeAnimationState->setLoop(active);
	}
	else
	{
		mAnimationDescMap[AD_Loop] = Convert::toString(active);
	}
}

void Animation::update()
{
	if (enabled)
	{
		if (activeAnimationState)
		{
			double dt = Time->getDeltaTime();

			activeAnimationState->addTime(dt * speed);
		}
	}
}

bool Animation::isPlaying()
{
	return !activeAnimationState->hasEnded();
}

Ogre::AnimationState* Animation::getAnimation(const std::string& name)
{
	if (animationMap.count(name))
	{
		return animationMap.at(name);
	}

	return NULL;
}

void Animation::applyDescription(ComponentDescMap& mComponentDescMap)
{
	Component::applyDescription(mComponentDescMap);

	if (mComponentDescMap.count(AD_PlayAutomatically))
	{
		play(activeAnimationState->getAnimationName());
	}

	mComponentDescMap.clear();
}

void Animation::requestAnimations(SkinnedMeshRenderer* skin)
{
	animationMap.clear();
	animationMap = skin->getAnimations();

	if (animationMap.size())
		activeAnimationState =  animationMap.begin()->second;

	/*AnimationMap::iterator iter;
	for(iter = animationMap.begin(); iter != animationMap.end(); iter++) 
	{
		//std::cerr << "\nName: " << iter->first;
	}*/
}

void Animation::applyDesc(int key, const std::string& value)
{

	switch (key)
	{
	case AD_Loop:
		setLoop(Convert::parseBool(value));
		break;

	case AD_CurrentAnimation:
		play(value);
		break;

	default:
		break;
	}
}