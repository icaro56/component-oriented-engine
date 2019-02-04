/*
-----------------------------------------------------------------------------
This source is a Stereovision manager for OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2007 Mathieu Le Ber, AXYZ-IMAGES

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

// Last update 04/03/2008

#include "engine/StereoManager.h"
#include <vector>
#include "OgreCamera.h"
#include "OgreRenderTarget.h"
#include "OgreRoot.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreCompositorManager.h"
#include "OgreMeshManager.h"
#include "OgreMaterialManager.h"
#include "OgreEntity.h"
#include "OgreConfigFile.h"

//-------------- Stereo Camera listener -------------------------------
void StereoManager::StereoCameraListener::init(StereoManager *stereoMgr,
        Ogre::Viewport *viewport,
        bool isLeftEye)
{
    mStereoMgr = stereoMgr;
    mCamera = mStereoMgr->getCamera();
    mIsLeftEye = isLeftEye;
    mViewport = viewport;
}

void StereoManager::StereoCameraListener::preViewportUpdate(
    const Ogre::RenderTargetViewportEvent& evt)
{
    if(evt.source != mViewport)
    {
        return;
    }

    Real offset = mStereoMgr->getEyesSpacing()/2;

    if(mIsLeftEye)
    {
        offset = -offset;
    }

    mCamera->setFrustumOffset(-offset,0);
    mPos = mCamera->getPosition();
    Ogre::Vector3 pos = mPos;
    pos += offset * mCamera->getRight();
    mCamera->setPosition(pos);
    mStereoMgr->updateAllDependentRenderTargets(mIsLeftEye);
    mStereoMgr->chooseDebugPlaneMaterial(mIsLeftEye);
}

void StereoManager::StereoCameraListener::postViewportUpdate(
    const Ogre::RenderTargetViewportEvent& evt)
{
    if(evt.source != mViewport)
    {
        return;
    }
    mCamera->setFrustumOffset(0,0);
    mCamera->setPosition(mPos);

    //if(mStereoMgr->getStereoMode() == StereoManager::SM_SHUTTERGLASSES)
    //	mStereoMgr->inverseStereo(!mStereoMgr->isStereoInversed());
}

//------------------------ init Stereo Manager --------------------------
StereoManager::StereoManager(void)
{
    mStereoMode = SM_NONE;
    mDebugPlane = NULL;
    mDebugPlaneNode = NULL;
    mLeftViewport = NULL;
    mRightViewport = NULL;
    mIsFocalPlaneFixed = false;
    mScreenWidth = 1;
    mEyesSpacing = 0;
    mFocalLength = 1;
    mConfigFileName = "";
    mIsInversed = false;
    mRightMask = ~((Ogre::uint32)0);
    mLeftMask = ~((Ogre::uint32)0);
}

void StereoManager::init(Ogre::Viewport* leftViewport, Ogre::Viewport* rightViewport)
{
    if(!isStereoModeValid(mStereoMode))
    {
        return;
    }
    if(mStereoMode == SM_ANAGLYPH)
    {
        initAnaglyph(leftViewport);
    }
    else
    {
        initViewports(leftViewport, rightViewport);
    }

    RenderTargetList::iterator it;
    RenderTargetList::iterator end = mRenderTargetList.end();
    for(it = mRenderTargetList.begin(); it != end; ++it)
    {
        it->first->setAutoUpdated(false);
    }

    setFocalLength(mFocalLength);
    if(mIsFocalPlaneFixed)
    {
        updateCamera(0);
    }
}


void StereoManager::init(Ogre::Camera *camera, Ogre::Viewport* leftViewport,
                         Ogre::Viewport* rightViewport, StereoMode mode)
{
    mCamera = camera;
    mStereoMode = mode;
    init(leftViewport, rightViewport);
}

void StereoManager::init(Ogre::Camera *camera, Ogre::Viewport* leftViewport,
                         Ogre::Viewport* rightViewport, const Ogre::String fileName)
{
    mCamera = camera;
    mConfigFileName = fileName;
    loadConfig(fileName);
    init(leftViewport, rightViewport);
}

void StereoManager::initViewports(Ogre::Viewport* leftViewport,
                                  Ogre::Viewport* rightViewport)
{
    mLeftCameraListener.init(this, leftViewport, !mIsInversed);
    mRightCameraListener.init(this, rightViewport, mIsInversed);
    if(leftViewport)
    {
        leftViewport->getTarget()->addListener(&mLeftCameraListener);
    }
    if(rightViewport)
    {
        rightViewport->getTarget()->addListener(&mRightCameraListener);
    }
    mLeftViewport = leftViewport;
    mRightViewport = rightViewport;
}


void StereoManager::initAnaglyph(Ogre::Viewport* viewport)
{
    Ogre::RenderSystem *sys = Ogre::Root::getSingleton().getRenderSystem();

    // Create render textures for the right eye
    Ogre::TexturePtr tex;
    tex = Ogre::TextureManager::getSingleton().createManual( "Stereo/Right",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
            viewport->getActualWidth(), viewport->getActualHeight(),
            0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
    Ogre::RenderTarget *rightTex = tex->getBuffer()->getRenderTarget();
    rightTex->addViewport( mCamera );
    // detach and reattach render target with the good priority
    sys->detachRenderTarget(rightTex->getName());
    rightTex->setPriority(3);
    sys->attachRenderTarget(*rightTex);


    tex = Ogre::TextureManager::getSingleton().createManual( "Stereo/Left",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
            viewport->getActualWidth(), viewport->getActualHeight(),
            0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
    Ogre::RenderTarget *leftTex = tex->getBuffer()->getRenderTarget();
    leftTex->addViewport( mCamera );
    sys->detachRenderTarget(leftTex->getName());
    leftTex->setPriority(3);
    sys->attachRenderTarget(*leftTex);

    mAreOverlaysEnabled = viewport->getOverlaysEnabled();
    rightTex->getViewport(0)->setOverlaysEnabled(mAreOverlaysEnabled);
    leftTex->getViewport(0)->setOverlaysEnabled(mAreOverlaysEnabled);
    viewport->setOverlaysEnabled(false);

    mCompositorViewport = viewport;
    initViewports(leftTex->getViewport(0), rightTex->getViewport(0));

    Ogre::CompositorManager::getSingleton().addCompositor(viewport,
            "Stereo/Anaglyph");
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport,
            "Stereo/Anaglyph",
            true);
}

void StereoManager::shutdown(void)
{
    if(mStereoMode == SM_NONE)
        return;
    if(mLeftViewport)
    {
        mLeftViewport->getTarget()->removeListener(&mLeftCameraListener);
    }
    if(mRightViewport)
    {
        mRightViewport->getTarget()->removeListener(&mRightCameraListener);
    }
    if(mStereoMode == SM_ANAGLYPH)
    {
        Ogre::CompositorManager::getSingleton().setCompositorEnabled(
            mCompositorViewport,
            "Stereo/Anaglyph",
            false);
        Ogre::CompositorManager::getSingleton().removeCompositor(mCompositorViewport,
                "Stereo/Anaglyph");
        //mRightTarget->removeAllViewports();
        //mLeftTarget->removeAllViewports();
        mCompositorViewport->setOverlaysEnabled(mAreOverlaysEnabled);

        Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName("Stereo/Right");
        Ogre::Root::getSingleton().detachRenderTarget(tex->getBuffer()->getRenderTarget());
        tex = Ogre::TextureManager::getSingleton().getByName("Stereo/Left");
        Ogre::Root::getSingleton().detachRenderTarget(tex->getBuffer()->getRenderTarget());
        //Root::getSingleton().detachRenderTarget(
        Ogre::TextureManager::getSingleton().remove("Stereo/Right");
        Ogre::TextureManager::getSingleton().remove("Stereo/Left");
    }
    mLeftViewport = NULL;
    mRightViewport = NULL;
    if(mDebugPlane)
    {
        Ogre::SceneNode *parent =
            static_cast<Ogre::SceneNode*>(mDebugPlaneNode->getParent());
        parent->removeAndDestroyChild("Stereo/DebugPlaneNode");
        mDebugPlaneNode = NULL;
        //parent->_update(false,false);
        mSceneMgr->destroyEntity("Stereo/Screen");
        mDebugPlane = NULL;
        Ogre::MeshManager::getSingleton().remove("Stereo/ScreenPlane");
        Ogre::MaterialManager::getSingleton().remove("Stereo/Wireframe");
    }

    RenderTargetList::iterator it;
    RenderTargetList::iterator end = mRenderTargetList.end();
    for(it = mRenderTargetList.begin(); it != end; ++it)
    {
        it->first->setAutoUpdated(it->second);
    }

    mStereoMode = SM_NONE;
}
//-------------------------- misc --------------

void StereoManager::setVisibilityMask(Ogre::uint32 leftMask, Ogre::uint32 rightMask)
{
    if(mLeftViewport)
    {
        mLeftViewport->setVisibilityMask(leftMask);
    }
    if(mRightViewport)
    {
        mRightViewport->setVisibilityMask(rightMask);
    }
    mRightMask = rightMask;
    mLeftMask = leftMask;
}

void StereoManager::addRenderTargetDependency(Ogre::RenderTarget *renderTarget)
{
    if(!isStereoModeValid(mStereoMode))
    {
        return;
    }
    if(mRenderTargetList.find(renderTarget) != mRenderTargetList.end())
    {
        return;
    }
    mRenderTargetList[renderTarget] = renderTarget->isAutoUpdated();
    renderTarget->setAutoUpdated(false);
}

void StereoManager::removeRenderTargetDependency(Ogre::RenderTarget *renderTarget)
{
    if(!isStereoModeValid(mStereoMode))
    {
        return;
    }
    if(mRenderTargetList.find(renderTarget) == mRenderTargetList.end())
    {
        return;
    }
    renderTarget->setAutoUpdated(mRenderTargetList[renderTarget]);
    mRenderTargetList.erase(renderTarget);
}

void StereoManager::updateAllDependentRenderTargets(bool isLeftEye)
{
    Ogre::uint32 mask;
    if(isLeftEye)
    {
        mask = mLeftMask;
    }
    else
    {
        mask = mRightMask;
    }

    RenderTargetList::iterator itarg, itargend;
    itargend = mRenderTargetList.end();
    for( itarg = mRenderTargetList.begin(); itarg != itargend; ++itarg )
    {
        Ogre::RenderTarget *rt = itarg->first;

        int n = rt->getNumViewports();
        std::vector<int> maskVector(n);

        for(int i = 0; i<n ; ++i)
        {
            maskVector[i] = rt->getViewport(i)->getVisibilityMask();
            rt->getViewport(i)->setVisibilityMask(maskVector[i] & mask);
        }

        rt->update();

        for(int i = 0; i<n ; ++i)
        {
            rt->getViewport(i)->setVisibilityMask(maskVector[i]);
        }
    }
}

bool StereoManager::isStereoModeValid(StereoMode mode)
{
    switch(mode)
    {
    case SM_ANAGLYPH:
    case SM_DUALOUTPUT:
        //case SM_SHUTTERGLASSES:
        return true;
    default:
        return false;
    }
}

//---------------------------- Stereo tuning  ------------------------
void StereoManager::setFocalLength(Real l)
{
    Real old = mFocalLength;
    mFocalLength = l;
    if( mCamera )
    {
        mCamera->setFocalLength(mFocalLength);
        if(mIsFocalPlaneFixed)
        {
            updateCamera(mFocalLength - old);
        }
        else if(mDebugPlane)
        {
            updateDebugPlane();
        }
    }
}

void StereoManager::useScreenWidth(Real w)
{
    mScreenWidth = w;
    mIsFocalPlaneFixed = true;
    if( mCamera )
    {
        updateCamera(0);
    }
}

void StereoManager::updateCamera(Real delta)
{
    mCamera->moveRelative(-delta * Ogre::Vector3::UNIT_Z);
    Ogre::Radian a = 2 * Ogre::Math::ATan(mScreenWidth/(2 * mFocalLength * mCamera->getAspectRatio()));
    mCamera->setFOVy(a);
}

void StereoManager::inverseStereo(bool inverse)
{
    mIsInversed = inverse;
    mLeftCameraListener.mIsLeftEye = !mIsInversed;
    mRightCameraListener.mIsLeftEye = mIsInversed;
}

//------------------------------------ Debug focal plane ---------------------------------
void StereoManager::enableDebugPlane(bool enable)
{
    if(!mDebugPlane)
    {
        return;
    }
    mDebugPlane->setVisible(enable);
}

void StereoManager::toggleDebugPlane(void)
{
    if(!mDebugPlane)
    {
        return;
    }
    mDebugPlane->setVisible(!mDebugPlane->isVisible());
}

void StereoManager::createDebugPlane(Ogre::SceneManager *sceneMgr,
                                     const Ogre::String &leftMaterialName,
                                     const Ogre::String &rightMaterialName)
{
    if(mDebugPlane)
    {
        return;
    }

    mSceneMgr = sceneMgr;
    Ogre::Plane screenPlane;
    screenPlane.normal = Ogre::Vector3::UNIT_Z;
    Ogre::MeshManager::getSingleton().createPlane("Stereo/ScreenPlane",
                                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            screenPlane,1,1,10,10);
    mDebugPlane = sceneMgr->createEntity("Stereo/Screen", "Stereo/ScreenPlane");

    Ogre::MaterialPtr mat =
        static_cast<Ogre::MaterialPtr>(
            Ogre::MaterialManager::getSingleton().create("Stereo/Wireframe",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
    Ogre::Pass *pass = mat->getTechnique(0)->getPass(0);
    pass->setPolygonMode(Ogre::PM_WIREFRAME);
    pass->setCullingMode(Ogre::CULL_NONE);
    pass->setLightingEnabled(false);

    if(leftMaterialName == "")
    {
        mLeftMaterialName = "Stereo/Wireframe";
    }
    else
    {
        mLeftMaterialName = leftMaterialName;
    }

    if(rightMaterialName == "")
    {
        mRightMaterialName = "Stereo/Wireframe";
    }
    else
    {
        mRightMaterialName = rightMaterialName;
    }


    mDebugPlaneNode = static_cast<Ogre::SceneNode*>(sceneMgr->getRootSceneNode()->
                      createChild("Stereo/DebugPlaneNode"));
    mDebugPlaneNode->attachObject(mDebugPlane);

    enableDebugPlane(true);
    updateDebugPlane();
}

void StereoManager::updateDebugPlane(void)
{
    if(!mDebugPlaneNode)
    {
        return;
    }

    Ogre::Vector3 pos = mCamera->getPosition();
    pos += mCamera->getDirection() * mFocalLength;
    mDebugPlaneNode->setPosition(pos);
    mDebugPlaneNode->setOrientation(mCamera->getOrientation());
    Ogre::Vector3 scale;
    Real height = mFocalLength * Ogre::Math::Tan(mCamera->getFOVy()/2)*2;
    scale.z = 1;
    scale.y = height;
    scale.x = height * mCamera->getAspectRatio();
    mDebugPlaneNode->setScale(scale);
}

void StereoManager::chooseDebugPlaneMaterial(bool isLeftEye)
{
    if(mDebugPlane)
    {
        if(isLeftEye)
        {
            mDebugPlane->setMaterialName(mLeftMaterialName);
        }
        else
        {
            mDebugPlane->setMaterialName(mRightMaterialName);
        }
    }
}


//-------------------------------------- config -------------------------------
void StereoManager::saveConfig(const Ogre::String &filename)
{
    Ogre::String name;
    if (filename.empty())
    {
        if (mConfigFileName.empty())
        {
            return;
        }
        else
            name = mConfigFileName;
    }
    else
    {
        name = filename;
    }

    std::ofstream of(name.c_str());
    if (!of)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE,
                    "Cannot create settings file.",
                    "StereoManager::saveConfig");
    }
    of << "[Stereoscopy]" << std::endl;
    of << "Stereo mode = "  << mStereoMode << std::endl;
    of << "Eyes spacing = " << mEyesSpacing << std::endl;
    of << "Focal length = " << mFocalLength << std::endl;
    of << "Screen width = " << mScreenWidth << std::endl;
    of << "Fixed screen = " << (mIsFocalPlaneFixed ? "true" : "false")
    << std::endl;
    of << "Inverse stereo = " << (mIsInversed ? "true" : "false") << std::endl;

    of.close();
}

void StereoManager::loadConfig(const Ogre::String &filename)
{
    Ogre::String name;
    if (filename.empty())
    {
        if (mConfigFileName.empty ())
            return;
        else
            name = mConfigFileName;
    }
    else
        name = filename;

    Ogre::ConfigFile cf;
    //, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,"\x09",true);
    cf.load(name.c_str());
    mStereoMode = static_cast<StereoMode>(Ogre::StringConverter::parseInt(
            cf.getSetting("Stereo mode",
                          "Stereoscopy")));

    fixFocalPlanePos(Ogre::StringConverter::parseBool(
                         cf.getSetting("Fixed screen","Stereoscopy")));
    setFocalLength(Ogre::StringConverter::parseReal(
                       cf.getSetting("Focal length","Stereoscopy")));
    setEyesSpacing(Ogre::StringConverter::parseReal(
                       cf.getSetting("Eyes spacing","Stereoscopy")));
    setScreenWidth(Ogre::StringConverter::parseReal(
                       cf.getSetting("Screen width","Stereoscopy")));
    inverseStereo(Ogre::StringConverter::parseBool(
                      cf.getSetting("Inverse stereo","Stereoscopy")));

}
