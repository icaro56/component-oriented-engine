#include "engine/CompositorListeners.h"

#include "OgreTechnique.h"
#include "OgreCamera.h"

///////////////////////////////////////////////////////////////////////////////
//                   GRAPHIC LISTENER
///////////////////////////////////////////////////////////////////////////////
GraphicListener::GraphicListener()
{
    mBright = 1;
    mContrast = 500;
    mColor = 1;
}

void GraphicListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
}

void GraphicListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    fpParams->setNamedConstant( "brilho", mBright );

    fpParams->setNamedConstant( "saturacao", mContrast );

    fpParams->setNamedConstant( "cor", mColor );
}

void GraphicListener::setParams(float bright, float color, float contrast)
{
    mBright = bright;
    mContrast = contrast;
    mColor = color;
}

///////////////////////////////////////////////////////////////////////////////
//                   Ambient Oclusion LISTENER
///////////////////////////////////////////////////////////////////////////////

void AmbientOclusionListener::setCamera(Ogre::Camera* cam)
{
	mCamera = cam;
}

void AmbientOclusionListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    if (pass_id != 42) // not SSAO, return
        return;

    // this is the camera you're using
    //Ogre::Camera *cam = mCamera ;

    // calculate the far-top-right corner in view-space
    Ogre::Vector3 farCorner = mCamera->getViewMatrix(true) * mCamera->getWorldSpaceCorners()[4];

    // get the pass
    Ogre::Pass *pass = mat->getBestTechnique()->getPass(0);

    // get the vertex shader parameters
    Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();

    // set the camera's far-top-right corner
    if (params->_findNamedConstantDefinition("farCorner"))
        params->setNamedConstant("farCorner", farCorner);

    // get the fragment shader parameters
    params = pass->getFragmentProgramParameters();

    // set the projection matrix we need
    static const Ogre::Matrix4 CLIP_SPACE_TO_IMAGE_SPACE(
        0.5,    0,    0,  0.5,
        0,   -0.5,    0,  0.5,
        0,      0,    1,    0,
        0,      0,    0,    1);

    if (params->_findNamedConstantDefinition("ptMat"))
        params->setNamedConstant("ptMat", CLIP_SPACE_TO_IMAGE_SPACE * mCamera->getProjectionMatrixWithRSDepth());

    if (params->_findNamedConstantDefinition("far"))
        params->setNamedConstant("far", mCamera->getFarClipDistance());
}