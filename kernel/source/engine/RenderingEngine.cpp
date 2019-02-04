#include "engine/RenderingEngine.h"

#include "engine/RenderView.h"
#include "engine/Engine.h"
#include "engine/Scene.h"
#include "engine/callback/SceneCallback.h"
#include "engine/StereoManager.h"


#include "components/rendering/Camera.h"
#include "gameobject/LayerMask.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreMaterialManager.h>
#include <OgreConfigFile.h>
#include <OgreParticleSystemManager.h>
#include <OgreOverlayElement.h>
#include "OgreOverlaySystem.h"

#include "utils/DebugClass.h"
/*#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>*/

using namespace OgreMax;

RenderingEngine* RenderingEngine::instance = nullptr;

RenderingEngine* RenderingEngine::getInstance()
{
	if (!instance)
	{
		instance = new RenderingEngine();
	}

	return instance;
}

RenderingEngine::RenderingEngine()
	: mIsDeviceLost(false),
	  mStereoMgr(false),
	  mRoot(nullptr)
	  /*mGUI(0),
	  mPlatform(0),
	  boolMyGUI(false)*/
{

}

RenderingEngine::~RenderingEngine()
{
	mRenderViewList.clear();

	OgreMaxList::iterator it = mOMaxSceneList.begin();
    for (it = mOMaxSceneList.begin(); it != mOMaxSceneList.end(); )
    {
		OgreMax::OgreMaxScene* scene = (*it);
		delete scene;
      	it = mOMaxSceneList.erase(it);
    }
	mOMaxSceneList.clear();

	if (mStereoMgr)
	{
		delete mStereoMgr;
		mStereoMgr = nullptr;
	}

	/* if (mGUI)
    {
        mGUI->shutdown();
        delete mGUI;
        mGUI = 0;
    }

    if (mPlatform)
    {
        mPlatform->shutdown();
        delete mPlatform;
        mPlatform = 0;
    }*/

	if (mRoot)
	{
		delete mRoot;
		mRoot = nullptr;
	}
}

void RenderingEngine::init(const std::string& logName)
{
	mStereoMgr = new StereoManager();

	// create the root object
#ifdef NDEBUG
	mRoot = new Ogre::Root("plugins.cfg","", logName);
#else
	mRoot = new Ogre::Root("plugins.cfg","Ogre.log");
#endif

	if (mRoot == NULL)
	{
		throw (char *)"Fail to initialise the render engine.";
	}


}

void RenderingEngine::destroy()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void RenderingEngine::update()
{
	mRoot->_fireFrameStarted();

	Debug->updateDrawing();

	RenderViewList::iterator i = mRenderViewList.begin();
	while (i != mRenderViewList.end()) 
	{
		RenderView* v = i.value();

		if (v && v->isVisible())
			v->update();

		++i;
	}

	mRoot->_fireFrameRenderingQueued();
	mRoot->_fireFrameEnded();

	Ogre::WindowEventUtilities::messagePump();

	// update OgreMax scene. Não necessário aqui.
    /*if (mOMaxScene != NULL)
    {
        mOMaxScene->Update(dt*mAnimSpeed);
    }*/
}

void RenderingEngine::setup()
{
	// set animation modes
	Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_LINEAR);
	Ogre::Animation::setDefaultRotationInterpolationMode(
		Ogre::Animation::RIM_LINEAR);

	//antes era 8
	Ogre::TextureManager::getSingletonPtr()->setDefaultNumMipmaps(5);

	//padrão BILINEAR
	//Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);

	
	Ogre::MovableObject::setDefaultVisibilityFlags(VF_DEFAULT);

	Ogre::MovableObject::setDefaultQueryFlags(LayerMask::nameToLayer("Default"));
}

//-----------------------------------------------------------------------------
void RenderingEngine::addResourceLocation(const std::string &local,
								 const std::string &type,
								 const std::string &groupName,
								 const bool recursive)
{
    Ogre::ResourceGroupManager::getSingleton().
    addResourceLocation(local, type, groupName, recursive);
}

//-----------------------------------------------------------------------------
void RenderingEngine::initialiseAllResourceGroups()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//-----------------------------------------------------------------------------
void RenderingEngine::initialiseResourceGroup(const std::string &groupName)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(groupName);
}

//-----------------------------------------------------------------------------
void RenderingEngine::reloadResources(const std::string &groupName)
{
    // clear e destroy resource group para atualizar com o novo codigo
    Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(groupName);
    Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(groupName);
    Ogre::ParticleSystemManager::getSingletonPtr()->removeAllTemplates(true);

    // carrega resource com o novo codigo
    initialiseResourceGroup(groupName);
}

//-----------------------------------------------------------------------------
void RenderingEngine::setupRenderSystem(RenderSystem rs, const bool vsync,
													  const std::string& fsaa, 
													  bool allowPerfHud)
{
    // set RenderSystem
    Ogre::RenderSystemList rList = mRoot->getAvailableRenderers();
    Ogre::RenderSystemList::iterator it = rList.begin();
    Ogre::RenderSystem *rSys;

    std::string toFind;

    if (rs == OPENGL)
    {
        toFind = "OpenGL";
    }
    else if (rs == DIRECT3D)
    {
        toFind = "Direct3D";
    }
    else
        toFind = "";

    while (it != rList.end())
    {
        rSys = *(it++);

        if (rSys->getName().find(toFind) != std::string::npos)
        {
            mRoot->setRenderSystem(rSys);

			//capturando dimensão do espaço disponível na tela principal
			int idScreen = QApplication::desktop()->primaryScreen();
			QRect r = QApplication::desktop()->availableGeometry(idScreen);

			QString dimensions = QString("%1 x %2 @ 32-bit colour").arg( r.width()-15).arg( r.height()-40);
            
			rSys->setConfigOption("Video Mode", dimensions.toStdString());
            rSys->setConfigOption("Full Screen", "No");
            //rSys->setConfigOption("Resource Creation Policy", "Create on active device");  //erro quando há outro renderView em monitor diferente e a janela principal fica passando de um monitor para o outro. Resultando no desaparecimento de recursos gráficos. (materiais, entities, meshes)
            rSys->setConfigOption("Resource Creation Policy", "Create on all devices");  //erro de ponteiro quando passava para o segundo monitor. Parou de dar. Gasta mais Memória RAM quando passa para o segundo monitor
            rSys->setConfigOption("Floating-point mode","Consistent");//"Fastest" Evitar erro de imprecisao ao somar doubles
            rSys->setConfigOption("Multi device memory hint","Auto hardware buffers management");
            //rSys->setConfigOption("Multi device memory hint","Use minimum system memory");
			
			
			rSys->setConfigOption("VSync", vsync ? "Yes" : "No");
			rSys->setConfigOption("FSAA", fsaa);
			rSys->setConfigOption("Allow NVPerfHUD", allowPerfHud ? "Yes" : "No");
			

            break;

        }
    }

    // initialise the render system
    if (mRoot->getRenderSystem() == NULL)
    {
        throw (char *)"Fail to initialise the render system.";
    }

    // initialise root
    mRoot->initialise(false);

    Ogre::ConfigOptionMap map = rSys->getConfigOptions();
    Ogre::ConfigOptionMap::iterator mit = map.begin();

    //just for understanding what is being printed
    std::cout<<"Config options: "<<std::endl;

    for (; mit != map.end(); ++mit)
    {
        std::cout << mit->first <<" = "<< mit->second.currentValue << std::endl;
    }

	rSys->addListener(this);
}

//-----------------------------------------------------------------------------
void RenderingEngine::addRenderView(RenderView* renderview)
{
    //renderView->resizeWindow(parent->width(),parent->height());
	mRenderViewList.insert(renderview->getNameId().c_str() , renderview);
}

//-----------------------------------------------------------------------------
RenderView *RenderingEngine::getRenderView(const std::string& name)
{
    return mRenderViewList.value(QString(name.c_str()));
}

//-----------------------------------------------------------------------------
void RenderingEngine::removeRenderView(RenderView *view)
{
	QString rName = view->getNameId().c_str();

	if (mRenderViewList.contains(rName))
	{
		/*Ogre::RenderWindow * rOgreWindow = mRenderViewList.value(rName)->getRenderWindow();

		if (rOgreWindow)
		{
			rOgreWindow->removeAllListeners();
			rOgreWindow->removeAllViewports();

			if (mRoot)
				mRoot->detachRenderTarget(rOgreWindow);
		}*/

		mRenderViewList.remove(rName);

	}
}

void RenderingEngine::removeRenderView(const std::string& name)
{
	QString rName = name.c_str();
	if (mRenderViewList.contains(rName))
	{
		/*Ogre::RenderWindow * rOgreWindow = mRenderViewList.value(rName)->getRenderWindow();

		if (rOgreWindow)
		{
			rOgreWindow->removeAllViewports();

			if (mRoot)
				mRoot->detachRenderTarget(rOgreWindow);
		}*/

		mRenderViewList.remove(rName);
	}
}

void RenderingEngine::updateOgreMaxScenes(double dt)
{
	OgreMaxList::iterator it = mOMaxSceneList.begin();
    for (it = mOMaxSceneList.begin(); it != mOMaxSceneList.end(); ++it)
    {
		OgreMax::OgreMaxScene* scene = (*it);
		scene->Update(dt);
    }
}

//-----------------------------------------------------------------------------
double RenderingEngine::getAvgFPS(const std::string& targetName)
{
    Ogre::RenderTarget *target = mRoot->getRenderTarget(targetName);
    return target->getStatistics().avgFPS;
}

//-----------------------------------------------------------------------------
void RenderingEngine::loadScene(const std::string &file, RenderView *view)
{
	OgreMax::OgreMaxScene* mOMaxScene = new OgreMaxScene();
    mOMaxScene->Load(file.c_str(), view->getRenderWindow(), 0,
                           GEngine->getCurrentScene()->getOgreScene(), 0, new SceneCallback());

	mOMaxSceneList.insert(file.c_str(), mOMaxScene);
	//mOMaxSceneList.push_back(mOMaxScene);

    /*std::cout << "ANIMATIONS" << std::endl;
    Ogre::AnimationStateIterator it = GEngine->getCurrentScene()->getOgreScene()->getAnimationStateIterator();

    while (it.hasMoreElements())
    {
        Ogre::AnimationState *anim = it.getNext();
        std::cout << anim->getAnimationName() << std::endl;
    }

    std::cout << "ENTITIES" << std::endl;
    Ogre::SceneManager::MovableObjectIterator mit =
                                GEngine->getCurrentScene()->getOgreScene()->getMovableObjectIterator("Entity");

    while (mit.hasMoreElements())
    {
        Ogre::MovableObject *ent = mit.getNext();
        std::cout << ent->getName() << std::endl;
    }*/
}

QStringList RenderingEngine::loadSceneReturnNames(const std::string&file, RenderView *view)
{
	OgreMax::OgreMaxScene* mOMaxScene = new OgreMaxScene();
    mOMaxScene->Load(file.c_str(), view->getRenderWindow(), 0,
                           GEngine->getCurrentScene()->getOgreScene(), 0, new SceneCallback());

	mOMaxSceneList.insert(file.c_str(), mOMaxScene);
	//mOMaxSceneList.push_back(mOMaxScene);

    std::map<Ogre::String, Ogre::MovableObject*> map = mOMaxScene->GetLoadedObjects();

    std::map<Ogre::String, Ogre::MovableObject*>::iterator it = map.begin();

    QStringList list;

    for (; it != map.end(); ++it)
    {
        list.push_back(it->first.c_str());
    }

    return list;
}

void RenderingEngine::destroyScene(const QString& name)
{
	if (mOMaxSceneList.contains(name))
	{
		OgreMax::OgreMaxScene* ogremaxscene = mOMaxSceneList.value(name);

		if (ogremaxscene)
		{
			delete ogremaxscene;
			ogremaxscene = 0;
		}

		mOMaxSceneList.remove(name);
	}
}

//-----------------------------------------------------------------------------
/*void RenderEngine::initStereoDual(Camera *cam, RenderView *view1, RenderView *view2)
{
    mStereoMgr->init(cam->getOgreCamera(),
                     view1->getRenderWindow()->getViewport(0),
                     view2->getRenderWindow()->getViewport(0),
                     StereoManager::SM_DUALOUTPUT);
}

//-----------------------------------------------------------------------------
void RenderEngine::initStereoSingle(Camera *cam, RenderView *view)
{
    mStereoMgr->init(cam->getOgreCamera(),
                     view->getRenderWindow()->getViewport(0),
                     NULL,
                     StereoManager::SM_ANAGLYPH);
}*/

//-----------------------------------------------------------------------------
void RenderingEngine::stereoSetEyesSpacing(double spacing)
{
    mStereoMgr->setEyesSpacing(spacing);
}

//-----------------------------------------------------------------------------
void RenderingEngine::stereoSetFocalLength(double length)
{
    mStereoMgr->setFocalLength(length);
}

//-----------------------------------------------------------------------------
void RenderingEngine::stereoSetScreenWidth(double width)
{
    mStereoMgr->setScreenWidth(width);
}

//-----------------------------------------------------------------------------
void RenderingEngine::rotOverlayElement(Ogre::OverlayElement* element,
                                     float degrees)
{

    Ogre::Material *mat = element->getMaterial().get();
    // Rotate Texture
    Ogre::TextureUnitState *texture = mat->getTechnique(0)->
                                      getPass(0)->getTextureUnitState(0);
    texture->setTextureRotate(Ogre::Degree(degrees));
}

//-----------------------------------------------------------------------------
/*void RenderEngine::setupMyGUI(const std::string &renderName)
{
    //GUI
	mPlatform = new MyGUI::OgrePlatform();
	Ogre::RenderWindow *win = getRenderView(renderName)->getRenderWindow();
    mPlatform->initialise(win, GEngine->getCurrentScene()->getOgreScene());
    mGUI = new MyGUI::Gui();
    mGUI->initialise();
    boolMyGUI = true;

    //Para informar que apenas o render view principal terá MyGUI
    getRenderView(renderName)->setHaveMyGUI(true);
}*/

//-----------------------------------------------------------------------------
/*MyGUI::Gui *RenderEngine::getMyGUI()const
{
    return mGUI;
}*/

//-----------------------------------------------------------------------------
/*MyGUI::OgrePlatform* RenderEngine::getOgrePlatform()const

{
    return  mPlatform;
}*/

/*bool RenderEngine::haveMyGUI()const
{
    return boolMyGUI;
}*/

Ogre::DisplayString RenderingEngine::convertToUTF(const std::string& text)
{
    const char *sourceString = text.c_str();

    int convertidos = mbstowcs(NULL, sourceString, 0) + 1;

    if (convertidos == 0)
    {
        return "sem conversao";
    }

    if (convertidos > ULONG_MAX/sizeof(wchar_t))
    {
        return "muito grande";
    }

    wchar_t *destString = (wchar_t*)malloc(convertidos * sizeof(wchar_t));
    mbstowcs(destString, sourceString, convertidos);

    Ogre::DisplayString t = destString;
    return t;
}

/*MyGUI::UString RenderEngine::convertToMyGuiUString(const std::string& text)
{
    const char *sourceString = text.c_str();

    int convertidos = mbstowcs(NULL, sourceString, 0) + 1;

    if (convertidos == 0)
    {
        return "sem conversao";
    }

    if (convertidos > ULONG_MAX/sizeof(wchar_t))
    {
        return "muito grande";
    }

    wchar_t *destString = (wchar_t*)malloc(convertidos * sizeof(wchar_t));
    mbstowcs(destString, sourceString, convertidos);

    MyGUI::UString t = destString;
    return t;
}*/

Ogre::Root *RenderingEngine::getRoot()
{
    return mRoot;
}

void RenderingEngine::eventOccurred (const Ogre::String &eventName, const Ogre::NameValuePairList *parameters)
{
   if (eventName == "DeviceLost")
   {
	   //std::cerr << "\nDEVICE LOST 123 ######################################";
	   mIsDeviceLost = true;
   }
   else if (eventName == "DeviceRestored")
   {
	   //std::cerr << "\nDEVICE RESTORED 123 ######################################";
	   mIsDeviceLost = false;
   }
}

bool RenderingEngine::renderSystemHasCapability(const Ogre::Capabilities c)
{
	return getRoot()->getRenderSystem()->getCapabilities()->hasCapability(c);
}

bool RenderingEngine::raycast(const Ray& ray, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycastRendering(ray, layerMask);
}

bool RenderingEngine::raycast(const Ray& ray, RaycastHit& hitInfo, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycastRendering(ray, hitInfo, layerMask);
}

RaycastHitList RenderingEngine::raycastAll(const Ray& ray, float distance, unsigned layerMask, Scene* scene)
{
	if (!scene)
		scene = GEngine->getCurrentScene();

	return scene->raycastRenderingAll(ray, distance, layerMask);
}