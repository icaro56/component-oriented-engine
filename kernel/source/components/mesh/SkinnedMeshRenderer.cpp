#include "mesh/SkinnedMeshRenderer.h"

#include "OgreEntity.h"
#include "OgreSkeleton.h"
#include "OgreBone.h"
#include "OgreAnimationState.h"

#include "engine/Engine.h"
#include "gameobject/GameObject.h"
#include "transform/Transform.h"
#include "mesh/MeshFilter.h"
#include "rendering/Camera.h"
#include "miscellaneous/Animation.h"
#include "gameObject/GameObject.h"

#include "utils/KernelConverter.h"

SkinnedMeshRenderer::SkinnedMeshRenderer()
	: Renderer("SkinnedMeshRenderer")
{

}

SkinnedMeshRenderer::SkinnedMeshRenderer(const ComponentDescMap& d)
	: Renderer("SkinnedMeshRenderer")
{

}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	
}

Animation::AnimationMap SkinnedMeshRenderer::getAnimations()
{
	Animation::AnimationMap map;

	Ogre::AnimationStateSet* set = mEntity->getAllAnimationStates();
	Ogre::AnimationStateIterator iter = set->getAnimationStateIterator();

	while(iter.hasMoreElements())
	{
		Ogre::AnimationState* animationState = iter.getNext();

		if (!map.count(animationState->getAnimationName()))
		{
			map[animationState->getAnimationName()] = animationState;
		}
	}

	return map;
}

void SkinnedMeshRenderer::nextInit()
{
	if (animation())
	{
		animation()->init(getGameObject()->getName());
	}
}

Vector3 SkinnedMeshRenderer::getBonePosition(const std::string& name)
{
	return Convert::toKernel(mEntity->getSkeleton()->getBone(name)->_getDerivedPosition());
}

Quaternion SkinnedMeshRenderer::getBoneOrientation(const std::string& name)
{
	return Convert::toKernel(mEntity->getSkeleton()->getBone(name)->_getDerivedOrientation());
}