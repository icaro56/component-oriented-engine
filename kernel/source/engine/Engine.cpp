#include "engine/Engine.h"


#include "engine/Scene.h"

#include "../MainWindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include "time/Time.h"

#include "engine/PhysicsEngine.h"
#include "engine/SoundEngine.h"
#include "engine/RenderingEngine.h"


using namespace OgreMax;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
///////////////////////////////////////////////////////////////////////////////

Engine::Engine(QString log)
    : isSceneInitialized(false),
	  currentScene(nullptr),
	  soundEngine(nullptr),
	  physicsEngine(nullptr),
	  renderingEngine(nullptr)
{
	// iniciando rendering engine
	renderingEngine = RenderingEngine::getInstance();
	renderingEngine->init(log.toStdString());

	// iniciando áudio engine
	soundEngine = SoundEngine::getInstance();
	soundEngine->init();
    
	// iniciando física
	physicsEngine = PhysicsEngine::getInstance();
	physicsEngine->init();
}

//-----------------------------------------------------------------------------
Engine::~Engine()
{
	for(auto it = mSceneMap.begin(); it != mSceneMap.end(); it++)
	{
		Scene* s = it->second;
		delete s;
		s = nullptr;
	}

	mSceneMap.clear();
	currentScene = nullptr;

	if (physicsEngine)
	{
		physicsEngine->destroy();
		physicsEngine = nullptr;
	}

	if (soundEngine)
	{
		soundEngine->destroy();
		soundEngine = nullptr;
	}

	if (renderingEngine)
	{
		renderingEngine->destroy();
		renderingEngine = nullptr;
	}
}

//-----------------------------------------------------------------------------
void Engine::setupScene(Ogre::SceneType typeMask)
{
	currentScene = createScene("scenemgr", typeMask);

	renderingEngine->setup();

    isSceneInitialized = true;
}

//-----------------------------------------------------------------------------
bool Engine::updateWorld()
{
	double dt = Time->getDeltaTime();

	if (!renderingEngine->isDeviceLost())
	{
		updateScenes(dt);

		renderingEngine->update();
	}

    return true;
}

Scene* Engine::createScene(const std::string& name, Ogre::SceneType typeMask)
{
	if (!mSceneMap.count(name))
	{
		Scene* scene = new Scene(name, typeMask);
		mSceneMap[name] = scene;
		scene->createRootTransform();

		return scene;
	}
	else
	{
		std::cerr << "\nScene Name is already in use";
		return NULL;
	}
}

Scene* Engine::createSceneAndDestroyCurrent(const std::string& name, Ogre::SceneType typeMask)
{
	if (!mSceneMap.count(name))
	{
		if (currentScene)
		{
			delete currentScene;
			currentScene = 0;
		}

		Scene* scene = new Scene(name, typeMask);
		mSceneMap[name] = scene;
		currentScene = scene;
		return scene;
	}
	else
	{
		std::cerr << "\nScene Name is already in use";
		return NULL;
	}
}

Scene* Engine::getSceneByName(const std::string& name)
{
	if (mSceneMap.count(name))
	{
		return mSceneMap[name];
	}

	return NULL;
}

Scene* Engine::getCurrentScene()
{
	return currentScene;
}

void Engine::destroyScene(const std::string& name)
{
	if (mSceneMap.count(name))
	{
		Scene* scene = mSceneMap[name];
		delete scene;
		mSceneMap.erase(name);
	}
	else
	{
		std::cerr << "\n Scene não existe.";
	}
}

void Engine::setCurrentScene(const std::string& name)
{
	if (mSceneMap.count(name))
	{
		currentScene = mSceneMap[name];
	}
}

void Engine::clearScenes()
{
	SceneMap::iterator iter;
	for(iter = mSceneMap.begin(); iter != mSceneMap.end(); iter++) 
	{
		Scene* s = iter->second;

		if (s)
		{
			s->clear();
		}
	}
}

void Engine::updateScenes(float dt)
{
	SceneMap::iterator iter;
	for(iter = mSceneMap.begin(); iter != mSceneMap.end(); iter++) 
	{
		Scene* s = iter->second;

		if (s)
		{
			s->update(dt);
		}
	}
}

RenderingEngine* Engine::getRenderingEngine()
{
	return renderingEngine;
}

PhysicsEngine* Engine::getPhysicsEngine()
{
	return physicsEngine;
}

SoundEngine* Engine::getSoundEngine()
{
	return soundEngine;
}