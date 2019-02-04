#ifndef SKINNEDMESHRENDERER_H
#define SKINNEDMESHRENDERER_H

#include "KernelPrerequisites.h"

#include "Renderer.h"
#include "components/miscellaneous/Animation.h"
#include "math/Vector3.h"

class KERNEL_VS_EXPORT SkinnedMeshRenderer : public Renderer
{
	friend class Animation;
public:
	
	SkinnedMeshRenderer();
	SkinnedMeshRenderer(const ComponentDescMap& d);
	virtual ~SkinnedMeshRenderer();

	virtual void nextInit();

	Vector3 getBonePosition(const std::string& name);
	Quaternion getBoneOrientation(const std::string& name);

private:
	Animation::AnimationMap getAnimations();
};

#endif