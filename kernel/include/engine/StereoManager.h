/*
-----------------------------------------------------------------------------
This source is a Stereovision manager for OGRE
    (Object-oriented Graphics Rendering Engine.h)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2007 Mathieu Le Ber, AXYZ-IMAGES

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

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

//#include "Ogre.h"
#include "OgreRenderTargetListener.h"
#include <map>
#include "OgreVector3.h"
#include "KernelPrerequisites.h"

namespace Ogre
{
class Camera;
}


//using namespace Ogre;
/** Stereo vision manager class.
	This manager supports two stereo vision mode :
	 - An anaglyph mode for red/blue glasses
	 - A dual output mode to use with a polarized screen and two projectors
	 or mounted head display

	Note :
	Ogre does not support natively the output in fullscreen on two screens.
	If you want to use dual
	output mode with two fullscreen output, you may want to apply this
	patch to OGRE :
	http://sourceforge.net/tracker/index.php?func=detail&aid=1692979&group_id=2997&atid=302997
	But this manager is completely indepedent from this patch,
	the dual output mode only needs two viewports,
	for example two non-fullscreen windows or a windows with two
	viewports inside.
*/

class KERNEL_VS_EXPORT StereoManager
{
public:
    enum StereoMode
    {
        SM_NONE = 0,
        SM_ANAGLYPH = 1,
        SM_DUALOUTPUT = 2
        //SM_SHUTTERGLASSES = 3
    };
protected:
    class KERNEL_VS_EXPORT StereoCameraListener : public Ogre::RenderTargetListener
    {
        Ogre::Vector3 mPos;
        StereoManager *mStereoMgr;
        Ogre::Camera *mCamera;
        Ogre::Viewport *mViewport;
    public:
        bool mIsLeftEye;
        void init(StereoManager *stereoMgr, Ogre::Viewport *viewport,
                  bool isLeftCamera);
//		void preRenderTargetUpdate (const RenderTargetEvent& evt);
//		void postRenderTargetUpdate  (const RenderTargetEvent& evt);
        void preViewportUpdate (const Ogre::RenderTargetViewportEvent &evt);
        void postViewportUpdate (const Ogre::RenderTargetViewportEvent &evt);
    };


    Ogre::Camera *mCamera;
    Ogre::SceneManager *mSceneMgr;
    StereoCameraListener mLeftCameraListener, mRightCameraListener;
    //RenderTarget *mLeftTarget, *mRightTarget;
    Ogre::Viewport *mLeftViewport, *mRightViewport;
    Ogre::uint32 mLeftMask, mRightMask;

    /* config */
    StereoMode mStereoMode;
    Ogre::Real mEyesSpacing;
    Ogre::Real mFocalLength;
    bool mIsFocalPlaneFixed;
    Ogre::Real mScreenWidth;
    bool mIsInversed;
    Ogre::String mConfigFileName;

    /* members for anaglyph only  */
    bool mAreOverlaysEnabled;
    Ogre::Viewport *mCompositorViewport;

    /* dependencies */
    typedef std::map<Ogre::RenderTarget *, bool> RenderTargetList;
    //typedef std::list<RenderTarget *> RenderTargetList;
    RenderTargetList mRenderTargetList;

    /* debug plane */
    Ogre::Entity *mDebugPlane;
    Ogre::SceneNode *mDebugPlaneNode;
    Ogre::String mLeftMaterialName, mRightMaterialName;

    void updateCamera(Ogre::Real delta);
    void init(Ogre::Viewport* leftViewport, Ogre::Viewport* rightViewport);
    void initAnaglyph(Ogre::Viewport* leftViewport);
    void initViewports(Ogre::Viewport* leftViewport,
                       Ogre::Viewport* rightViewport);
    void updateAllDependentRenderTargets(bool isLeftEye);
    void chooseDebugPlaneMaterial(bool isLeftEye);
    bool isStereoModeValid(StereoMode mode);

public:
    //--------------init--------------
    /*StereoManager::*/
    StereoManager(void);
    /** The manager should be initialized with the camera and two viewports
    if you want dual output stereo (SM_DUALOUTPUT).
    	If you want red/blue anaglyph stereo (SM_ANAGLYPH) you only need
    	one viewport, just set the rightViewport to NULL.
    	The left eye and the right eye will be composited on the
    	leftViewport.

    	You can also use a configuration file that will store the
    	stereo mode, the focal length, the eye spacing and
    	the screen width. */
    void init(Ogre::Camera *camera, Ogre::Viewport* leftViewport,
              Ogre::Viewport* rightViewport, const Ogre::String fileName);
    void init(Ogre::Camera *camera, Ogre::Viewport* leftViewport,
              Ogre::Viewport* rightViewport, StereoMode mode);

    /**	Shutdown and re-init the stereo manager to change stereo mode */
    void shutdown(void);

    //--------------dependencies--------------
    /**	RenderTargets added with this method will no longer be
    auto-updated but will be updated
    	one time before the right viewport is updated and one time before
    	the left viewport is updated.
    	It is useful if you have a render texture whose content depends
    	on the camera position,
    	for example a render texture for water reflection */
    void addRenderTargetDependency(Ogre::RenderTarget *renderTarget);
    void removeRenderTargetDependency(Ogre::RenderTarget *renderTarget);

    //--------------debug plane--------------
    /**	Create a representation of the focal plane in the scene.
    If no material name is given,
    	a default wireframe material is used. */
    void createDebugPlane(Ogre::SceneManager *sceneMgr,
                          const Ogre::String &leftMaterialName = "",
                          const Ogre::String &rightMaterialName = "");
    void enableDebugPlane(bool enable);
    void toggleDebugPlane(void);
    void updateDebugPlane(void);

    //--------------accessors--------------
    inline Ogre::Camera *getCamera(void) const
    {
        return mCamera;
    }

    inline Ogre::Real getEyesSpacing(void) const
    {
        return mEyesSpacing;
    }
    inline void setEyesSpacing(Ogre::Real l)
    {
        mEyesSpacing = l;
    }

    inline Ogre::Real getFocalLength(void) const
    {
        return mFocalLength;
    }
    void setFocalLength(Ogre::Real l);

    /**	The focal plane represents the screen in the world space.
    As the screen is not and infinite plane but a rectangle, what I
    call the focal plane is in fact	a rectangle.
    	When the position of the plane is fixed, its position and size
    	doesn't change if
    	you change the focal length. Instead the camera is moved to
    	reflect the focal length.
    	The FOV angle of the camera is also adjusted in order to keep
    	the focal rectangle
    	covering the whole field view.

    	If you set the screen width in the manager you can achieve a 1:1
    	scale effect if the
    	observer's distance to the screen is equal to the focal distance.*/
    inline void fixFocalPlanePos(bool fix)
    {
        mIsFocalPlaneFixed = fix;
    }
    inline void setScreenWidth(Ogre::Real w)
    {
        mScreenWidth = w;
    }
    void useScreenWidth(Ogre::Real w);

    inline StereoMode getStereoMode(void)
    {
        return mStereoMode;
    }

    /** Inverse the left eye and the right eye viewports */
    void inverseStereo(bool inverse);
    bool isStereoInversed(void)
    {
        return mIsInversed;
    }

    /** Only objects matching the following flags will be rendered.
    	This method sets the visibility mask for the right and the left
    	viewports and
    	the according mask for each dependent render target. */
    void setVisibilityMask(Ogre::uint32 leftFlags, Ogre::uint32 rightFlags);

    //--------------config--------------
    /**	You can save and load the stereo configuration (mode, eyes spacing,
                                                focal length and screen width)
    	to a file. Then this file can be used to initialize the manager.  */
    void /*StereoManager::*/saveConfig(const Ogre::String &filename = "");
    void /*StereoManager::*/loadConfig(const Ogre::String &filename = "");
};
