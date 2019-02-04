#ifndef SCENECALLBACK_H
#define SCENECALLBACK_H

#include "OgreMaxScene.hpp"
#include "KernelPrerequisites.h"

class KERNEL_VS_EXPORT SceneCallback : public OgreMax::OgreMaxSceneCallback
{
public:
    SceneCallback();
    virtual ~SceneCallback();
    virtual void CreatedEntity(const OgreMax::OgreMaxScene *scene,
                               Ogre::Entity *entity);
    virtual void CreatedCamera(const OgreMax::OgreMaxScene* scene,
                               Ogre::Camera* camera);

protected:

private:
};

#endif // SCENECALLBACK_H
