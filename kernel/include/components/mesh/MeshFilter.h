#ifndef MESHFILTER_H
#define MESHFILTER_H

#include "KernelPrerequisites.h"

#include "../Component.h"

namespace Ogre
{
	class Mesh;
}

class KERNEL_VS_EXPORT MeshFilter : public Component
{
public:
	
	MeshFilter();
	MeshFilter(std::string idComponent);
	virtual ~MeshFilter();

	virtual void prerequisites(){}
	virtual void init(const std::string& name);
	virtual void nextInit();

	virtual void setEnabled(bool enable);
	virtual bool isEnabled();

	Ogre::Mesh *getMesh();
	void setFileName(const std::string& fName);
	std::string getFileName();

private:
	Ogre::Mesh* mesh;
	std::string meshFileName;
};

#endif