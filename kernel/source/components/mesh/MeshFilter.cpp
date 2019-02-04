#include "mesh/MeshFilter.h"

#include "OgreMesh.h"
#include "OgreMeshManager.h"
#include "OgreResourceGroupManager.h"

#include "components/mesh/MeshRenderer.h"
#include "gameobject/GameObject.h"

MeshFilter::MeshFilter()
	: Component("MeshFilter", "MeshFilter"),
	  mesh(nullptr),
	  meshFileName("")
{

}

MeshFilter::MeshFilter(std::string idComponent)
	: Component(idComponent, "MeshFilter"),
	  mesh(nullptr),
	  meshFileName("")
{

}

MeshFilter::~MeshFilter()
{

}

void MeshFilter::init(const std::string& name)
{
	//if (Ogre::MeshManager::getSingletonPtr()->resourceExists(getFileName()) )
	if ( (Ogre::ResourceGroupManager::getSingleton().resourceExists( "General", getFileName() ) ) ||
		(Ogre::MeshManager::getSingletonPtr()->resourceExists(getFileName())) )
	{
		Ogre::MeshPtr ptr = Ogre::MeshManager::getSingletonPtr()->load(getFileName(), "General");
    
		if (!ptr.isNull())
		{
			mesh = ptr.get();
			initialized = true;
		}
	}
	else
	{
		std::cerr << "\n" << "GameObject " << name << ": Componente MeshFilter não encontrou arquivo de mesh";
	}
}

void MeshFilter::setEnabled(bool enable)
{
	
}

bool MeshFilter::isEnabled()
{
	return true;
}

void MeshFilter::nextInit()
{
	if (renderer())
	{
		renderer()->init(getGameObject()->getName());
	}
}

Ogre::Mesh *MeshFilter::getMesh()
{
	return mesh;
}

void MeshFilter::setFileName(const std::string& fName)
{
	meshFileName = fName;
}

std::string MeshFilter::getFileName()
{
	return meshFileName;
}