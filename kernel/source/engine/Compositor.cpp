#include "engine/Compositor.h"
#include "engine/RenderView.h"
#include "OgreCompositorManager.h"
#include "OgreTechnique.h"
#include "components/rendering/Camera.h"

///////////////////////////////////////////
//COMPOSITOR
///////////////////////////////////////////
Compositor::Compositor(Camera* camera, const std::string& compositor,
                       bool enable)
{
    mCompositor = Ogre::CompositorManager::getSingleton().
                  addCompositor(camera->getViewport(),compositor);

    setEnabled(enable); 

	//setCompositorEnabled(Viewport *vp, const String &compositor, bool value)
    //mListener = new GraphicListener();       
} 

Compositor::~Compositor()
{
	setEnabled(false);
    ///A Ogre destroi sozinha o compositor. Basta destruirmos o listener.
    //delete mCompositor;
    //mCompositor = 0;
}

void Compositor::addListener(Ogre::CompositorInstance::Listener * listener)
{
	mCompositor->addListener(listener);
}

void Compositor::removeListener(Ogre::CompositorInstance::Listener * listener)
{
	mCompositor->removeListener(listener);
}

void Compositor::setEnabled(bool enable)
{
    mCompositor->setEnabled(enable);
	mCompositor->setAlive(enable);
}

bool Compositor::isEnabled()
{
    return mCompositor->getEnabled();
}

/*void Compositor::setParams(float bright, float color, float contrast)
{
    mListener->setParams(bright, color, contrast);
}*/