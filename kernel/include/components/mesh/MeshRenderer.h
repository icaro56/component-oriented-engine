#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "KernelPrerequisites.h"

#include "Renderer.h"

class KERNEL_VS_EXPORT MeshRenderer : public Renderer
{
public:
	
	MeshRenderer();
	MeshRenderer(const ComponentDescMap& d);
	virtual ~MeshRenderer();
};

#endif