#include "stdafx.h"
#include "components/mesh/ManualObject.h"
#include "OgreManualObject.h"
#include "gameobject/GameObject.h"
#include "engine/Scene.h"
#include "transform/Transform.h"
#include "OgreMesh.h"
#include "OgreMeshManager.h"
#include "OgreResourceGroupManager.h"
#include "utils/KernelConverter.h"

ManualObject::ManualObject()
	: Component("ManualObject", "ManualObject"),
	  mesh(nullptr)
{
}

ManualObject::ManualObject(const ComponentDescMap& d)
	: Component("ManualObject", "ManualObject"),
	mesh(nullptr)
{

}

ManualObject::~ManualObject(void)
{
	if (mesh)
	{
		mesh->detachFromParent();
		mesh = nullptr;
	}
}

void ManualObject::init(const std::string& name)
{
	
	if (!initialized)
	{
		ManualObjectName = "ManualObject__"+name;
		mesh = getGameObject()->getParentScene()->getOgreScene()->createManualObject(ManualObjectName);

		if (mesh)
		{
				initialized = true;
	
		}
	}
	
}

Ogre::ManualObject *ManualObject::getManual()
{
	return mesh;
}

void ManualObject::makeObject(QList<Vector3> lPoints,QList<Color> lColors,QList<int> lIndex,RenderType type)
{
	points = lPoints;
	colors = lColors;
	index = lIndex;

	 mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType(type));
	{
		for(int i=0; i < lPoints.size(); i++)
		{
				mesh->position(lPoints[i].x,lPoints[i].y,lPoints[i].z);
				mesh->colour(Convert::toOgre(lColors[i]));
		}

		for(int j = 0; j < lIndex.size(); j++)
		{
				mesh->index(lIndex[j]);
		}
	}

	mesh->end();
	transform()->_attachObject(mesh);
	initialized = true;
}

void ManualObject::makeObject(QList<Vector3> lPoints,QList<Color> lColors,RenderType type)
{
	points = lPoints;
	colors = lColors;

	 mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType(type));
	{
		for(int i=0; i < lPoints.size(); i++)
		{
				mesh->position(lPoints[i].x,lPoints[i].y,lPoints[i].z);
				mesh->colour(Convert::toOgre(lColors[i]));
		}
	}
	mesh->end();
	transform()->_attachObject(mesh);
	initialized = true;
}

void ManualObject::makeObject(QList<Vector3> lPoints,RenderType type)
{
	points = lPoints;
	 mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType(type));
	{
		for(int i=0; i < lPoints.size(); i++)
		{
				mesh->position(lPoints[i].x,lPoints[i].y,lPoints[i].z);
		}
	}
	mesh->end();
	transform()->_attachObject(mesh);
	initialized = true;
}

void ManualObject::makeObject(QList<Vector3> lPoints,QList<int> lIndex,RenderType type)
{
	points = lPoints;
	index = lIndex;

	 mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType(type));
	{
		for(int i=0; i < lPoints.size(); i++)
		{
				mesh->position(lPoints[i].x,lPoints[i].y,lPoints[i].z);
		}

		for(int j = 0; j < lIndex.size(); j++)
		{
				mesh->index(lIndex[j]);
		}
	}

	mesh->end();
	transform()->_attachObject(mesh);
	initialized = true;
}

void ManualObject::makeObject(QList<Vector3> lPoints,QList<Vector3> ltriangles,RenderType type)
{
	points = lPoints;
	triangles = ltriangles;

	 mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType(type));
	{
		for(int i=0; i < lPoints.size(); i++)
		{
				mesh->position(lPoints[i].x,lPoints[i].y,lPoints[i].z);
		}

		for(int j = 0; j < ltriangles.size(); j++)
		{
				int x = ltriangles[j].x;
				int y = ltriangles[j].y;
				int z = ltriangles[j].z;
				mesh->triangle(x,y,z);
		}
	}

	mesh->end();
	transform()->_attachObject(mesh);
	initialized = true;
}

void ManualObject::makeObject(QList<Vector3> lPoints,QList<Vector3> ltriangles,QList<Vector3> lnormals,QList<Vector2> ltextureCoord,RenderType type)
{
	points = lPoints;
	triangles = ltriangles;
	normals = lnormals;
	textureCoord = ltextureCoord;

	 mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType(type));
	{
		for(int i=0; i < lPoints.size(); i++)
		{
				mesh->position(lPoints[i].x,lPoints[i].y,lPoints[i].z);
				mesh->normal(lnormals[i].x,lnormals[i].y,lnormals[i].z);
				mesh->textureCoord(ltextureCoord[i].x,ltextureCoord[i].y);
		}

		for(int j = 0; j < ltriangles.size(); j++)
		{
				int x = ltriangles[j].x;
				int y = ltriangles[j].y;
				int z = ltriangles[j].z;
				mesh->triangle(x,y,z);
		}
	}

	mesh->end();
	transform()->_attachObject(mesh);
	initialized = true;
}

QList<Vector3> ManualObject::getPoints()
{
	return points;
}

QList<Color> ManualObject::getColors()
{
	return colors;
}

QList<int> ManualObject::getIndexs()
{
	return index;
}


 void ManualObject::setEnabled(bool enable)
{
	if (initialized)
	{
		mesh->setVisible(enable);
	}

	else
	{
		modesc[MO_Enabled] = enable;
	}
	
}
 bool ManualObject::isEnabled()
{
	return mesh->getVisible();
}


void ManualObject::setPoints(QList<Vector3> Points)
{
	if (initialized)
	{
		points = Points ;
	}
}

void ManualObject::setTriangles(QList<Vector3> Triangles)
{
	if (initialized)
	{
	    triangles = Triangles;
	}
}

void ManualObject::setColors(QList<Color> Colors)
{
	if (initialized)
	{
	  colors = Colors;
	}
}

void ManualObject::setIndex(QList<int> Index)
{
	if (initialized)
	{
		ManualObject::index = Index;
	}
}

void ManualObject::setRenderOperation(RenderType ro)
{
	if (initialized)
	{
	  renderOperation = ro; 
	}
	else
	{
		modesc[MO_RenderOperation] = ro;
	}
}