#pragma once
#ifndef _AXIS_OBJECT_H_
#define _AXIS_OBJECT_H_

#include <OgreMaterial.h>
#include <OgreMesh.h>
//#include <Ogre.h>

class AxisObject
{
    
private:
	
    void createAxesMesh();
	void createAxesMaterial();
	
public:

	virtual ~AxisObject();
   AxisObject();
   Ogre::MeshPtr mAxesMeshPtr;
   Ogre::MaterialPtr mAxisMatPtr;
};
 
#endif //--_AXIS_OBJECT_H_
