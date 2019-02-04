#ifndef ENGINE_H
#define ENGINE_H


#include "KernelPrerequisites.h"
#include <vector>
#include <string>
#include <QHash>
#include <QString>

#include "../Core.h"


/**
* @def GRender
* @brief Instancia um objeto RenderEngine
* @details inicia um ponteiro para a classe RenderEngine uma unica
* vez, caso esse ponteiro seja NULL senão ele apenas retorna o ponteiro.
* @return Ponteiro RenderEngine
*/
#define GEngine		Core::getInstance()->getGameEngine()

class Scene;
class PhysicsEngine;
class SoundEngine;
class RenderingEngine;


class KERNEL_VS_EXPORT Engine
{
    // PUBLIC ///////////////////////////////////////////////
public:

	typedef std::map<std::string, Scene*> SceneMap;

    /// Constructor
    Engine(QString log);
    /// Destructor
    ~Engine();

    // Finaliza o RenderEngine.
    /**
    * @brief Finaliza o RenderEngine.
    */
    static void destroy();

    // Inicializa a cena.
    /**
    * @brief Inicializa a cena
    */
    void setupScene(Ogre::SceneType typeMask = Ogre::ST_EXTERIOR_CLOSE);

    // Atualiza a cena.
    /**
    * @brief Atualiza a cena.
    */
    bool updateWorld();

	Scene* createScene(const std::string& name, Ogre::SceneType typeMask = Ogre::ST_EXTERIOR_CLOSE);
	Scene* createSceneAndDestroyCurrent(const std::string& name, Ogre::SceneType typeMask = Ogre::ST_EXTERIOR_CLOSE);
	Scene* getSceneByName(const std::string& name);
	Scene* getCurrentScene();
	void destroyScene(const std::string& name);
	void setCurrentScene(const std::string& name);
	void clearScenes();
	void updateScenes(float dt);

	RenderingEngine* getRenderingEngine();
	PhysicsEngine* getPhysicsEngine();
	SoundEngine* getSoundEngine();
	
    // PRIVATE //////////////////////////////////////////////
private:

	//Current Scene and Map
	Scene* currentScene;
	SceneMap mSceneMap;

    bool isSceneInitialized;

	//**********************************  Rendering **************************************
	RenderingEngine* renderingEngine;

	//**********************************  Física *****************************************
	PhysicsEngine* physicsEngine;

	//**********************************  Áudio ******************************************
	SoundEngine* soundEngine;
};

#endif
