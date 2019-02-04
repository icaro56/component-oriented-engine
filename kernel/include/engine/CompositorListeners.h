#ifndef COMPOSITORLISTENERS_H
#define COMPOSITORLISTENERS_H

#include <OgreCompositorInstance.h>
#include "KernelPrerequisites.h"

/**
 * @brief Ouvinte de um Compositor de configurações gráficas
 */
class KERNEL_VS_EXPORT GraphicListener : public Ogre::CompositorInstance::Listener
{
public:
    GraphicListener();
    virtual void notifyMaterialSetup(Ogre::uint32 pass_id,
                                     Ogre::MaterialPtr &mat);
    virtual void notifyMaterialRender(Ogre::uint32 pass_id,
                                      Ogre::MaterialPtr &mat);
    void setParams(float bright, float color, float contrast);

private:
    Ogre::GpuProgramParametersSharedPtr fpParams;
    Ogre::Real mBright;
    Ogre::Real mContrast;
    Ogre::Real mColor;
};

/**
 * @brief Ouvinte de um Compositor de ambiente oclusion
 */
class KERNEL_VS_EXPORT AmbientOclusionListener: public Ogre::CompositorInstance::Listener
{
    // this callback we will use to modify SSAO parameters
public:
	void setCamera(Ogre::Camera* cam);

    void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);

	private:
		Ogre::Camera* mCamera;
};


/*class KERNEL_VS_EXPORT ShadowListener: public Ogre::SceneManager::Listener
{
    // this is a callback we'll be using to set up our shadow camera
    void shadowTextureCasterPreViewProj(Ogre::Light *light, Ogre::Camera *cam, size_t)
    {
        // basically, here we do some forceful camera near/far clip attenuation
        // yeah.  simplistic, but it works nicely.  this is the function I was talking
        // about you ignoring above in the Mgr declaration.
        float range = light->getAttenuationRange();
        cam->setNearClipDistance(0.01);
        cam->setFarClipDistance(range);
        // we just use a small near clip so that the light doesn't "miss" anything
        // that can shadow stuff.  and the far clip is equal to the lights' range.
        // (thus, if the light only covers 15 units of objects, it can only
        // shadow 15 units - the rest of it should be attenuated away, and not rendered)
    }

    // these are pure virtual but we don't need them...  so just make them empty
    // otherwise we get "cannot declare of type Mgr due to missing abstract
    // functions" and so on
    void shadowTexturesUpdated(size_t) {}
    void shadowTextureReceiverPreViewProj(Ogre::Light*, Ogre::Frustum*) {}
    void preFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) {}
    void postFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) {}
};*/

#endif