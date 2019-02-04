#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "OgreCompositorInstance.h"
#include "KernelPrerequisites.h"


class RenderView;
class Camera;

class KERNEL_VS_EXPORT Compositor
{
public:
    Compositor(Camera* camera,const std::string& compositor,
               bool enable = false);
    ~Compositor();

	void addListener(Ogre::CompositorInstance::Listener * listener);
	void removeListener(Ogre::CompositorInstance::Listener * listener);
    void setEnabled(bool enable);
    bool isEnabled();
    //void setParams(float bright, float color, float contrast);

private:
    Ogre::CompositorInstance* mCompositor;
    //GraphicListener* mListener;
};



#endif
