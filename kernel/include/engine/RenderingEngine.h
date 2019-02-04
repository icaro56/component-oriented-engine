#ifndef RENDERING_ENGINE_H
#define RENDERING_ENGINE_H

#include "KernelPrerequisites.h"

#include "OgreCommon.h"
#include <vector>
#include <string>
#include <QHash>
#include <QString>
#include "OgreRenderSystem.h"
#include <OgreSceneManager.h>
#include <OgreOverlayElement.h>

#include "math/Ray.h"
#include "buffers/RaycastHit.h"
#include "PhysicsEngine.h"

class RenderView;
class StereoManager;
class Scene;

/**
* @namespace OgreMax
* @brief add OgreMaxScene
*/
namespace OgreMax
{
	class OgreMaxScene;
}

/*namespace MyGUI
{
    class Gui;
    class OgrePlatform;
	class UString;
}*/

namespace MOC
{
	class CollisionTools;
}

/**
* @enum RenderSystem
*/
enum RenderSystem
{
	OPENGL,
	DIRECT3D
};

#define Rendering RenderingEngine::getInstance()

///////////////////////////////////////////////////////////
// CLASS RenderingEngine
///////////////////////////////////////////////////////////

/**
 * @brief Gerencia o processo de renderiza��o.
 *
 * @details Esta classe � o centro do sistema de renderiza��o.
 � respons�vel por inicializar
 * a janela de renderiza��o, manter controle sobre c�meras e entidades,
 carregar terreno,
 * skybox, cenas a partir de arquivos .scene.
 */
class KERNEL_VS_EXPORT RenderingEngine : public Ogre::RenderSystem::Listener
{
public:
	typedef QHash<QString, OgreMax::OgreMaxScene*> OgreMaxList;
	typedef QHash<QString, RenderView*> RenderViewList;

	static RenderingEngine* getInstance();

	void init(const std::string& logName);
	void destroy();
	void update();

	// Define o sistema de renderiza��o (Direct3D, OpenGL)
    /**
    * @brief Define o sistema de renderiza��o
    * @remarks Cria o sistema de renderiza��o usando o Ogre, os sistemas
    * disponiveis s�o o Direct3D e o OpenGL
    * @param rs DIRECT3D ou OPENGL
    */
    void setupRenderSystem(RenderSystem rs, const bool vsync = true,
							const std::string& fsaa = "0", 
							bool allowPerfHud = false);

    /**
    * @brief adiciona um local para inicia os recursos da ogre
    * @param local string o endere�o no sistema da pasta ou zip
    * @param type string "FyleSystem" ou "Zip" para definir
    * se ser� carregado uma pasta ou um arquivo zipado.
    * @param groupName string nome do grupo de recursos
    * utilizado para carrega-lo e descarrega-lo se preciso.
    * @param recursive bool diz se a busca de recurso ser� recursiva
    * adicionando pastas que estejam dentro do endere�o adicionado.
    */
    void addResourceLocation(const std::string &local,
                                            const std::string &type,
                                            const std::string &groupName,
                                            const bool recursive = false);
    /**
    * @brief inicia todos os grupos de recurso.
    */
    void initialiseAllResourceGroups();

    /**
    * @brief inicia determinado grupo de recurso.
    * @param groupName string o nome do grupo a ser carregado.
    */
    void initialiseResourceGroup(const std::string &groupName);

    /**
    * @brief recarrega os resoures da aplica��o, utilizado
    * para atualizar recursos que foram modificados na execu��o
    do software.
    * @param groupName string o nome do grupo a ser recarregado.
    */
    void reloadResources(const std::string &groupName);

    // Cria uma �rea de renderiza��o.
    /**
     * @brief Cria uma �rea de renderiza��o.
     * @remarks Esse metodo come�a inicializando um objeto RenderView e
     o adiciona
     * em um Layout (objeto QLayout), a janela de renderiza��o � um objeto
     da classe
     * Ogre::RenderWindow criada nesse metodo, aqui tambem � feita a
     chamada aos metodos
     * initialiseResources() e setupScene() dessa classe, e por ultimo
     e setado o objeto
     * Ogre::RenderWindow ao RenderView do TerraVR.
     * @param name Nome (precisa ser �nico).

     * @param vsync Ativa sincroniza��o vertical.
     * @param pai da janela
     * @return Objeto RenderView (Janela de Renderiza��o)
     */
    void addRenderView(RenderView* renderview);


    RenderView* getRenderView(const std::string& name);
    // Desativa e remove um RenderView.
    /**
     * @brief Desativa e remove um RenderView
     * @remarks Percorre a lista de rederes e remove o view passado
     * como argumento
     * @param view Ponteiro para RenderView
    */
    void removeRenderView(RenderView *view);
    void removeRenderView(const std::string& name);

    // Taxa media de quadros por segundos.
    /**
     * @brief Obtem a media de frames por segundos.
     * @remarks Para ser usado deve-se passar o nome de um
     * Ogre::RenderTarget, por exemplo, o nome do RenderView
     * da aplica��o
     * @param nome do target
     * @return double media de quadros por segundos
    */
    double getAvgFPS(const std::string& targetName);

    // Define a cor de fundo.
    /**
    * @brief Define a cor de fundo.
    * @param r 0 minimo e 1 maximo
    * @param g 0 minimo e 1 maximo
    * @param b 0 minimo e 1 maximo.
    */
    //void setBackgroundColor(double r, double g, double b);

    // Carrega uma cena a partir de um arquivo .scene gerado pelo OgreMax.
    /**
    * @brief Carrega uma cena a partir de um arquivo .scene gerado pelo
    OgreMax.
    * @param file String path do arquivo
    * @param view RenderView da aplica��o
    */
    void loadScene(const std::string &file, RenderView *view);
	QStringList loadSceneReturnNames(const std::string&file, RenderView *view);

	/*!
		\brief M�todo usado para destruir um ogremax scene
	*/
	void destroyScene(const QString& name);

	//Atualiza as anima��es presentes nas cenas carregadas com ogreMax
	/**
	* @brief atualiza as anima��es dos scenes
	*/
	void updateOgreMaxScenes(double dt);

    // Inicializa vis�o estereosc�pica em modo dual (passando 2 RenderView).
    /**
    * @brief Inicializa vis�o estereosc�pica em modo dual (passando 2
                                                          RenderView).
    * @param cam Uma camera
    * @param view1 RenderView 1
    * @param view2 RenderView 2
    */
    //void initStereoDual(Camera *cam, RenderView *view1, RenderView *view2);

    // Inicializa vis�o estereosc�pia em modo simples (an�glifo).
    /**
    * @brief Inicializa vis�o estereosc�pia em modo simples (an�glifo).
    * @param cam Uma camera
    * @param view apenas um RenderView
    */
    //void initStereoSingle(Camera *cam, RenderView *view);

    // Define dist�ncia entre olhos da vis�o estereosc�pica.
    /**
    * @brief Define dist�ncia entre olhos da vis�o estereosc�pica.
    * @param spacing Distancia
    */
    void stereoSetEyesSpacing(double spacing);

    // Define dist�ncia focal da vis�o estereosc�pica.
    /**
    * @brief Define dist�ncia focal da vis�o estereosc�pica.
    * @param length distancia focal
    */
    void stereoSetFocalLength(double length);

    // Define largura de tela da vis�o estereosc�pica.
    /**
    * @brief Define largura de tela da vis�o estereosc�pica.
    * @param width Largura
    */
    void stereoSetScreenWidth(double width);

    void rotOverlayElement(Ogre::OverlayElement* element, float degrees);

    /*!
        \brief Inicializa a biblioteca de interfaces graficas MyGUI
    */
    //void setupMyGUI(const std::string &renderName);

    /*!
        \brief Retorna a interface gr�fica do MyGUI
    */
	//MyGUI::Gui *getMyGUI()const;

	/*!
        \brief Retorna a plataforma da ogre usada pelo MyGUI
	*/
	//MyGUI::OgrePlatform* getOgrePlatform()const;

	/*!
        \brief Retorna se foi inicializado o MyGui.
	*/
	//bool haveMyGUI()const;

	/*!
        \brief usado para converter strings com caracteres especiais.
        \example "� 100�" resultaria em erro na Ogre. Usa-se:
        overlay->setCaption(convertToUTF("� 100�"));
	*/
	Ogre::DisplayString convertToUTF(const std::string& text);
	//MyGUI::UString convertToMyGuiUString(const std::string& text);

	/*!
	\fn	Ogre::Root Engine::*getRoot();
	
	\brief	Gets the root.
	
	\author	Admin
	\date	07/02/2014
	
	\return	null if it fails, else the root.
	 */
	Ogre::Root *getRoot();

	virtual void eventOccurred (const Ogre::String &eventName, const Ogre::NameValuePairList *parameters=0);

	bool renderSystemDeviceLost() { return mIsDeviceLost;}

	bool renderSystemHasCapability(const Ogre::Capabilities c);

	void setup();

	bool isDeviceLost() {return mIsDeviceLost;}

	bool raycast(const Ray& ray, unsigned layerMask = Physics->DefaultRaycastLayers(), Scene* scene = nullptr);
	bool raycast(const Ray& ray, RaycastHit& hitInfo, unsigned layerMask = Physics->DefaultRaycastLayers(), Scene* scene = nullptr);
	RaycastHitList raycastAll(const Ray& ray, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers(), Scene* scene = nullptr);

private:
	RenderingEngine();
	~RenderingEngine();
	

private:
	static RenderingEngine* instance;

	/// Root from Ogre
	Ogre::Root *mRoot;

	///Vari�veis principais para o MyGUI
    /*MyGUI::Gui * mGUI;
	MyGUI::OgrePlatform* mPlatform;
	std::string renderNameSaved;
	bool boolMyGUI;*/

	/// OgreMax scene
    OgreMaxList mOMaxSceneList;

    bool isSceneInitialized;
    //RenderWindowList mRenderWindowList;
    RenderViewList mRenderViewList;
    /// StereoManager
    StereoManager *mStereoMgr;

	bool mIsDeviceLost;
};

#endif