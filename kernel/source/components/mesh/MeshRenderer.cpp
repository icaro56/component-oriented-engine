#include "mesh/MeshRenderer.h"


#include "OgreEntity.h"
#include "engine/Engine.h"
#include "gameobject/GameObject.h"
#include "transform/Transform.h"
#include "mesh/MeshFilter.h"
#include "rendering/Camera.h"

MeshRenderer::MeshRenderer()
	: Renderer("MeshRenderer")
{

}

MeshRenderer::MeshRenderer(const ComponentDescMap& d)
	: Renderer("MeshRenderer")
{

}

MeshRenderer::~MeshRenderer()
{
	
}