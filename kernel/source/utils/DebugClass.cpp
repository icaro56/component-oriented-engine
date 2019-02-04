
#include "utils/DebugClass.h"

#include "stdafx.h"
#include "utils/KernelConverter.h"
#include "OgreManualObject.h"
#include "engine/Scene.h"
#include "engine/Engine.h"
#include "OgreMesh.h"
#include "OgreMeshManager.h"
#include "OgreResourceGroupManager.h"
#include "components/transform/Transform.h"

#include "time/time.h"

LineDebug::LineDebug()
	:start(Vector3::ZERO),
	end(Vector3::ZERO),
	color(Color::White),
	duration(0.0f),
	id(0),
	updated(false),
	death(false),
	initialized(false)
{
}

RayDebug::RayDebug()
	:start(Vector3::ZERO),
	dir(Vector3::ZERO),
	color(Color::White),
	duration(0.0f),
	id(0),
	updated(false),
	death(false),
	initialized(false)
{
}


DebugClass* DebugClass::instance = nullptr;

DebugClass* DebugClass::getInstance()
{
	if (!instance)
	{
		instance = new DebugClass();
	}

	return instance;
}

DebugClass::DebugClass(void)
:mesh(nullptr)
{

}

 DebugClass::~DebugClass(void)
{
	instance = 0;
	mesh->clear();
	mesh = nullptr;
}

 void  DebugClass::drawLine(const Vector3 &start,const Vector3 &end, const Color &color, float duration, int id, bool update)
 {
	if(id <= lineList.size())
	{
		if (!mesh)
			createMesh();
		else
		{
			mesh->clear();
		}
		
		updateLineIndex(id,start,end,color,duration,update);
		
	
		mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType::OT_LINE_LIST);
		{
			for(int i = 0; i < lineList.size() ; i++)
			{
				if(lineList[i].death == false)
				{
					mesh->position(Convert::toOgre(lineList[i].start));
					mesh->colour(Convert::toOgre(lineList[i].color));
					mesh->position(Convert::toOgre(lineList[i].end));
					mesh->colour(Convert::toOgre(lineList[i].color));
				}

			}
		}
		mesh->end();
	}
	else
	{
		std::cerr<<" 'Aviso!' - Debug nao criado indice maior que a lista existente ;"<<std::endl;
	}
 }

 void  DebugClass::drawRay(const Vector3 &start,const Vector3 &dir,const Color &color, float duration, int id, bool update)
 {
	 if(id <= rayList.size())
	{
		if (!mesh)
			createMesh();
		else
		{
			lineList[id].death = false;
			mesh->clear();
		}
		
		updateRayIndex(id,start,start+dir,color,duration, update);
	
		mesh->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OperationType::OT_LINE_LIST);
		{
			for(int i = 0; i < rayList.size() ; i++)
			{
				if(lineList[i].death == false)
				{
					mesh->position(Convert::toOgre(rayList[i].start));
					mesh->colour(Convert::toOgre(rayList[i].color));
					mesh->position(Convert::toOgre(rayList[i].dir));
					mesh->colour(Convert::toOgre(rayList[i].color));
				}
			}
		}
		mesh->end();
	}
	else 
	{
		logError(" 'Aviso!' - Debug nao criado indice maior que a lista existente");
	}
 
 }

 void DebugClass::log(const std::string &message)
 {
	 std::string aux = "DEBUG: " + message + "\n";
	 std::cerr<<aux<<std::endl;
 }

 void DebugClass::pause()
 {
	 system("pause");
 }

void DebugClass::clearDeveloperConsole()
{
	system("cls");
}

 void DebugClass::logError(const std::string &message)
 {
	 std::string aux = "ERROR: " + message + "\n";
	 std::cerr<<aux<<std::endl;
	 Ogre::LogManager::getSingleton().logMessage(aux);
 }

  void DebugClass::logWarning(const std::string &message)
 {
	 std::string aux = "WARNING: " + message + "\n";
	 std::cerr<<aux<<std::endl;
	 Ogre::LogManager::getSingleton().logMessage(aux);
 }


 void DebugClass::createMesh()
 {
	mesh = GEngine->getCurrentScene()->getOgreScene()->createManualObject("DebugClass");
	GEngine->getCurrentScene()->getRootTransform()->_attachObject(mesh);
	mesh->setCastShadows(false);
	mesh->setDynamic(true);
 }

 void DebugClass::updateLineIndex(int id,const Vector3 &vecOne,const Vector3 &vecTwo,const Color &color, float duration, bool update)
 {
	   
	 if(id < lineList.size())
	 {
		 if(lineList[id].updated)
		 {
			 lineList[id].color = color;
			 lineList[id].start = vecOne;
			 lineList[id].end = vecTwo;
			 lineList[id].updated = update;
		 }
	 }
	 else
	 {
			LineDebug line; 
			line.id = id;
			line.start = vecOne;
			line.end =vecTwo;
			line.color = color;
			line.duration = duration;
			line.updated = update;
			line.initialized = true;
			lineList.push_back(line);

	 }

 }

 void DebugClass::updateRayIndex(int id,const Vector3 &vecOne,const Vector3 &vecTwo,const Color &color, float duration, bool update)
 {
	   
	 if(id < rayList.size())
	 {
		  if(lineList[id].updated)
		 {
			 rayList[id].color = color;
			 rayList[id].start = vecOne;
			 rayList[id].dir = vecTwo;
			 rayList[id].updated = update;
		  }
	 }
	 else
	 {
			RayDebug line; 
			line.id = id;
			line.start = vecOne;
			line.dir =vecTwo;
			line.color = color;
			line.duration = duration;
			line.updated = update;
			line.initialized = true;
			rayList.push_back(line);
	 }

 }

 void DebugClass::updateDrawing()
 { 
	 float dt = Time->getDeltaTime();
	 bool update = haveUpdate();

	 if(update)
	 {
		 if(lineUpdate)
		 {
			 auto it = lineList.begin();

			 for(; it != lineList.end();)
			 {
			      if(update)
				 {
					 if(it->duration > 0)
					 {
						 it->duration -= dt;
						 it++;
					 }
					 else if (it->duration <= 0)
					 {
						 it->death = true;
						 it->updated = false;
						 it = lineList.erase(it);
					 }
			   }
			 }
		 }
		 
		  if(rayUpdate)
		 {
			 auto itj = rayList.begin();

			 for(; itj != rayList.end();)
			 {
				 if(update)
				 {
					  if(itj->duration > 0)
					 {
						  itj->duration -= dt;
						  itj++;
					 }
					  else if (itj->duration <= 0)
					 {
						 itj->death = true;
						 itj->updated = false;
						 itj = rayList.erase(itj);
					 }
				 }
			 }
			 
		 }
	 }
	 else
	 {
		 if (mesh)
		 {
			mesh->clear();
		 }
	 }
 }

 bool DebugClass::haveUpdate()
 {
	 lineUpdate = false;
	 rayUpdate = false;
	 bool have =false;

     for(int i =0; i < lineList.size();i++)
	{
		if(lineList[i].updated)
		{
			lineUpdate = true;
			have =true;
		}
	}

	for(int j =0; j < rayList.size();j++)
	{
		if(rayList[j].updated)
		{
			rayUpdate = true;
			have =true;
		}
	}
	return have;
 }