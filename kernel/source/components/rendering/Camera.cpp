#include "rendering/Camera.h"

#include "OgreCamera.h"
#include "OgreMath.h"
#include "OgreStringConverter.h"

#include "engine/Engine.h"
#include "engine/RenderingEngine.h"
#include "engine/Scene.h"
#include "engine/RenderView.h"
#include "gameobject/GameObject.h"
#include "gameobject/LayerMask.h"
#include "transform/Transform.h"
#include "input/InputManager.h"
#include "CollisionTools.h"
#include "utils/KernelConverter.h"

#include <OgreRay.h>

Camera::CameraList Camera::allCameras;

Camera::Camera()
	: Component("Camera", "Camera"),
	  mCamera(nullptr),
	  mViewport(nullptr),
	  horizontalFOV(75.0),
	  /*mRaySceneQuery(nullptr),
	  mVolQuery(nullptr),*/
	  mSelectionBox(nullptr),
	  startedSelection(false)
{
	allCameras.push_back(this);
}

Camera::Camera(const ComponentDescMap& d)
	: Component("Camera", "Camera"),
	  mCamera(nullptr),
	  mCameraDescMap(d),
	  mViewport(nullptr),
	  horizontalFOV(75.0),
	  /*mRaySceneQuery(nullptr),
	  mVolQuery(nullptr),*/
	  mSelectionBox(nullptr),
	  startedSelection(false)
{
	
}

Camera::~Camera()
{
	//destruir camera: remover do gerenciador de cameras
	removeCamera(this);

	mCamera->detachFromParent();
	getGameObject()->getParentScene()->getOgreScene()->destroyCamera(mCamera);
	mCamera = nullptr;

	/*if (mVolQuery)
	{
		getGameObject()->getParentScene()->getOgreScene()->destroyQuery(mVolQuery);
	}*/

	if (mSelectionBox)
	{
		delete mSelectionBox;
	}
}

void Camera::prerequisites()
{
	if (!transform())
	{
		getGameObject()->addComponent(new Transform());
	}
}

void Camera::init(const std::string& name)
{
	if (!initialized)
	{
		if (!mCamera)
		{
			mCamera = getGameObject()->getParentScene()->getOgreScene()->createCamera(name + "_Camera");

			mSelectionBox = new SelectionBox("SelectionBox" + getGameObject()->getName());
			getGameObject()->getParentScene()->getOgreScene()->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionBox);
		}

		unsigned l = LayerMask::nameToLayer(getGameObject()->getLayer());
		setLayer(l);
		
		if (transform())
		{
			transform()->_attachObject(mCamera);

			initialized = true;

			if (mCameraDescMap.size())
				applyDescription(mCameraDescMap);
		}
	}
}

void Camera::setEnabled(bool enable)
{
	if (initialized)
	{
		mCamera->setVisible(enable);
	}
	else
	{
		mCameraDescMap[CD_Enabled] = Convert::toString(enable);
	}
}

bool Camera::isEnabled()
{
	if (initialized)
	{
		return mCamera->getVisible();
	}

	return true;
}

void Camera::setViewport(Ogre::Viewport* viewport)
{
	mViewport = viewport;
	mViewport->setBackgroundColour(Ogre::ColourValue::Black);

	resizeViewport(mViewport->getActualWidth() , mViewport->getActualHeight());
}

void Camera::resizeViewport(double width, double height)
{
	setAspect(width/height);

	//calculando o FOVy (Field of View vertical)
	float n = getNearClipDistance();

	setVerticalFOV( (horizontalFOV) / getAspect());

	setNearClipDistance(n);
}

Ogre::Viewport* Camera::getViewport()
{
	return mViewport;
}

void Camera::setHorizontalFOV(double f)
{
	horizontalFOV = f;
}

void Camera::setVerticalFOV(double f)
{
	double r = Math::DegreesToRadians(f);
	Radian rad(r);

	mCamera->setFOVy(Convert::toOgre(rad));
}

//---------------------------------------------------------
void Camera::setRange(double nearDist, double farDist)
{
	if (initialized)
	{
		mCamera->setNearClipDistance(Real(nearDist));
    
		if (farDist != 0)
		{
			mCamera->setFarClipDistance(Real(farDist));
		}
	}
	else
	{
		mCameraDescMap[CD_NearDistance] = Convert::toString(nearDist);
		mCameraDescMap[CD_FarDistance] = Convert::toString(farDist);
	}
}

void Camera::setNearClipDistance(double n)
{
	if (initialized)
	{
		mCamera->setNearClipDistance(n);
	}
	else
	{
		mCameraDescMap[CD_NearDistance] = Convert::toString(n);
	}
}

void Camera::setFarClipDistance(double f)
{
	if (initialized)
	{
		if (f != 0)
		{
			mCamera->setFarClipDistance(Real(f));
		}
	}
	else
	{
		mCameraDescMap[CD_FarDistance] = Convert::toString(f);
	}
}

double Camera::getNearClipDistance()
{
	return mCamera->getNearClipDistance();
}

//---------------------------------------------------------
void Camera::setLodBias(float d)
{
	if (initialized)
	{
		mCamera->setLodBias(d);
	}
	else
	{
		mCameraDescMap[CD_LodBias] = Convert::toString(d);
	}
}

//---------------------------------------------------------
std::string Camera::getName()
{
	return mCamera->getName();
}

//---------------------------------------------------------
void Camera::wireframe(bool enable)
{
	if (initialized)
	{
		if (enable)
		{
			mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else
		{
			mCamera->setPolygonMode(Ogre::PM_SOLID);
		}
	}
	else
	{
		mCameraDescMap[CD_WireFrameActive] = Convert::toString(enable);
	}
}

//---------------------------------------------------------
void Camera::setVisibilityMask(int mask)
{
	if (initialized)
	{
		mCamera->getViewport()->setVisibilityMask(mask);

		if (mSelectionBox)
		{
			mSelectionBox->setVisibilityFlags(mask);
		}
	}
	else
	{
		mCameraDescMap[CD_CullingMask] = Convert::toString(mask);
	}
}

int Camera::getVisibilityMask()
{
	return mCamera->getViewport()->getVisibilityMask();
}

bool Camera::isVisibilityFlags(int flags)
{
	if (mCamera->getViewport())
	{
		int f = mCamera->getViewport()->getVisibilityMask();
		if (f & flags)
			return true;
		else
			return false;
	}

	return false;
}

void Camera::setLayer(unsigned flags)
{
	layer = flags;
}

unsigned Camera::getLayer()
{
	return layer;
}

bool Camera::isLayer(unsigned flags)
{
	if (layer & flags)
		return true;
	else
		return false;
}

//---------------------------------------------------------
double Camera::getAspect()
{
    return mCamera->getAspectRatio();
}

//---------------------------------------------------------
void Camera::setAspect(double ar)
{
	if (initialized)
	{
		mCamera->setAspectRatio(ar);
	}
	else
	{
		mCameraDescMap[CD_Aspect] = Convert::toString(ar);
	}
}

//---------------------------------------------------------
bool Camera::isOnView(const Bounds& bounds)
{
	return mCamera->isVisible(Convert::toOgre(bounds));
}

Ogre::Camera* Camera::getOgreCamera()
{
	return mCamera;
}

void Camera::removeCamera(Camera* camera)
{
	CameraList::iterator it;

    for (it = allCameras.begin(); it != allCameras.end(); ++it)
    {
        Camera *thisCam = (*it);

		if(thisCam->getName() == camera->getName())
        {
            allCameras.erase(it);
            break;
        }
    }
}

Ray Camera::viewportPointToRay(const Vector2 &position)
{
	Ogre::Ray ray;

	Vector2 viewPosClamp = position;

	viewPosClamp.x = Math::Clamp(viewPosClamp.x, 0.0, 1.0);
	viewPosClamp.y = Math::Clamp(viewPosClamp.y, 0.0, 1.0);

	ray = mCamera->getCameraToViewportRay(viewPosClamp.x, viewPosClamp.y);

	return Convert::toKernel(ray);
}

Ray Camera::screenPointToRay(const Vector2 &position, bool isTopWindow)
{
	Ogre::Ray ray;

	Vector2 pos = Vector2::ZERO;
	if (!isTopWindow)
	{
		pos = Rendering->getRenderView("render1")->getAbsolutePosition();
	}
    
    float largura = float(mViewport->getActualWidth());
    float altura = float(mViewport->getActualHeight());
    int ratoX = position.x;
    int ratoY = position.y;

    double mx = (ratoX - pos.x) / largura;
    double my = (ratoY - pos.y) / altura;

    //Ogre::Entity *tmpE = NULL;
    //Ogre::Vector3 result = Ogre::Vector3::ZERO;
    //float distToColl;
    ray = mViewport->getCamera()->getCameraToViewportRay(mx,my);

	return Convert::toKernel(ray);
}

Vector3 Camera::worldToViewportPoint(const Vector3& worldPos)
{
	Ogre::Vector3 worldview = mCamera->getViewMatrix() * Convert::toOgre(worldPos);
	//homogenous clip space, between -1, 1 is in frustum
	Ogre::Vector3 hcsposition = mCamera->getProjectionMatrix() * worldview;

	if ( ( isinfrustum( hcsposition ) ) && ( isinfront( worldview ) ) )
	{
		/* relative */
		hcsposition.x = 0.5f + ( 0.5f * hcsposition.x );
		hcsposition.y = 0.5f + ( 0.5f * -hcsposition.y );
	}
	else
	{
		hcsposition.x = hcsposition.y = -1;
	}

	return Convert::toKernel(hcsposition);
}

Vector3 Camera::worldToScreenPoint(const Vector3& worldPos, bool isTopWindow)
{
	Ogre::Vector3 worldview = mCamera->getViewMatrix() * Convert::toOgre(worldPos);
	//homogenous clip space, between -1, 1 is in frustum
	Ogre::Vector3 hcsposition = mCamera->getProjectionMatrix() * worldview;

	if ( ( isinfrustum( hcsposition ) ) && ( isinfront( worldview ) ) )
	{
		Vector2 pos = Vector2::ZERO;
		if (!isTopWindow)
		{
			pos = Rendering->getRenderView("render1")->getAbsolutePosition();
		}

		float largura = float(mViewport->getActualWidth());
		float altura = float(mViewport->getActualHeight());

		/* absolute */
		int x = largura / 2;
		int y = altura / 2;
		x += ( x * hcsposition.x ) + pos.x;
		y += ( y * -hcsposition.y ) + pos.y;

		hcsposition.x = x;
		hcsposition.y = y;
	}
	else
	{
		hcsposition.x = hcsposition.y = -1;
	}

	return Convert::toKernel(hcsposition);
}

Vector3 Camera::viewportToWorldPoint(const Vector3& viewPos)
{
	Ogre::Ray ray;

	Vector3 viewPosClamp = viewPos;

	viewPosClamp.x = Math::Clamp(viewPosClamp.x, 0.0, 1.0);
	viewPosClamp.y = Math::Clamp(viewPosClamp.y, 0.0, 1.0);

	ray = mCamera->getCameraToViewportRay(viewPosClamp.x, viewPosClamp.y);

	Vector3 worldPos = Convert::toKernel(ray.getPoint(viewPosClamp.z));

	return worldPos;
}

Vector3 Camera::viewportToScreenPoint(const Vector3& viewPos, bool isTopWindow)
{
	Vector3 screenPos = Vector3::ZERO;
	Vector3 viewPosClamp = viewPos;

	viewPosClamp.x = Math::Clamp(viewPosClamp.x, 0.0, 1.0);
	viewPosClamp.y = Math::Clamp(viewPosClamp.y, 0.0, 1.0);

	Vector2 pos = Vector2::ZERO;
	if (!isTopWindow)
	{
		pos = Rendering->getRenderView("render1")->getAbsolutePosition();
	}

	float largura = float(mViewport->getActualWidth());
	float altura = float(mViewport->getActualHeight());

	screenPos.x = (viewPosClamp.x * largura) + pos.x;
	screenPos.y = (viewPosClamp.y * altura) + pos.y;
	screenPos.z = viewPosClamp.z;

	return screenPos;
}

Vector3 Camera::screenToWorldPoint(const Vector3& screenPos, bool isTopWindow)
{
	Ogre::Ray ray;
	Vector2 pos = Vector2::ZERO;
	if (!isTopWindow)
	{
		pos = Rendering->getRenderView("render1")->getAbsolutePosition();
	}

	float largura = float(mViewport->getActualWidth());
	float altura = float(mViewport->getActualHeight());
	int ratoX = screenPos.x;
	int ratoY = screenPos.y;

	double mx = (ratoX - pos.x) / largura;
	double my = (ratoY - pos.y) / altura;

	//Ogre::Entity *tmpE = NULL;
	//Ogre::Vector3 result = Ogre::Vector3::ZERO;
	//float distToColl;
	ray = mViewport->getCamera()->getCameraToViewportRay(mx,my);

	Vector3 worldPos = Convert::toKernel(ray.getPoint(screenPos.z));

	return worldPos;
}

Vector3 Camera::screenToViewportPoint(const Vector3& screenPos, bool isTopWindow)
{
	Vector3 viewpoint;
	Vector2 pos = Vector2::ZERO;
	if (!isTopWindow)
	{
		pos = Rendering->getRenderView("render1")->getAbsolutePosition();
	}

	float largura = float(mViewport->getActualWidth());
	float altura = float(mViewport->getActualHeight());

	viewpoint.x = ((screenPos.x - pos.x) / largura);
	viewpoint.y = ((screenPos.y - pos.y) / altura);
	viewpoint.z = screenPos.z;

	viewpoint.x = Math::Clamp(viewpoint.x, 0.0, 1.0);
	viewpoint.y = Math::Clamp(viewpoint.y, 0.0, 1.0);

	return viewpoint;
}

void Camera::startVolPosition(Vector2 s)
{
	if (!startedSelection)
	{
		startedSelection = true;
		mStart.x = s.x / mViewport->getActualWidth();
		mStart.y = s.y / mViewport->getActualHeight();
		mStop = mStart;

		if (mSelectionBox)
		{
			mSelectionBox->clear();
			mSelectionBox->setVisible(true);
		}
	}
}

RaycastHitList Camera::stopVolPosition(Vector2 s)
{
	if (startedSelection)
	{
		startedSelection = false;
		mSelectionBox->setVisible(false);

		return performSelection(mStart, mStop);
	}
}

void Camera::updateVolPosition(Vector2 _stop)
{
	if (startedSelection)
	{
		mStop.x = _stop.x / mViewport->getActualWidth();
		mStop.y = _stop.y / mViewport->getActualHeight();
 
		mSelectionBox->setCorners(Convert::toOgre(mStart), Convert::toOgre(mStop));
	}
}

RaycastHitList Camera::performSelection(const Vector2& first, const Vector2& second)
{
	float left = first.x, right = second.x;
	float top = first.y, bottom = second.y;
 
	if(left > right)
	{
		Math::swap(left, right);
	}
	if(top > bottom)
	{
		Math::swap(top, bottom);
	}
 
	if((right - left) * (bottom - top) < 0.0001)
	{
		return RaycastHitList();
	}
 
	Ray topLeft = Convert::toKernel(mViewport->getCamera()->getCameraToViewportRay(left, top));
	Ray topRight = Convert::toKernel(mViewport->getCamera()->getCameraToViewportRay(right, top)); 
	Ray bottomLeft = Convert::toKernel(mViewport->getCamera()->getCameraToViewportRay(left, bottom));
	Ray bottomRight = Convert::toKernel(mViewport->getCamera()->getCameraToViewportRay(right, bottom));
 
	return getGameObject()->getParentScene()->volumecastRendering(topLeft, topRight, bottomLeft, bottomRight);
}

/*QStringList Camera::getVolSelectionList()
{
	return volSelectionList;
}*/

/*void Camera::cameraPickPositions(Vector3& posTop, Vector3& posDown, double dist, double dY1, double dY2)
{
    double mx = 0.5; //(largura / 2) / largura;
    //double my = 1 ; //altura/altura;

	// create ray
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mx, dY1);
    Ogre::Ray mouseRay2 = mCamera->getCameraToViewportRay(mx, dY2);

	posTop = Convert::toKernel(mouseRay.getPoint(dist));
	posDown = Convert::toKernel(mouseRay2.getPoint(dist));
}*/

//----------------------------------------------------------
/*char *Camera::mousePickEntity(bool isTopWindow)
{
	Vector2 pos = Vector2::ZERO;
	if (!isTopWindow)
	{
		pos = Rendering->getRenderView("render1")->getAbsolutePosition();
	}
    
    float largura = float(mViewport->getActualWidth());
    float altura = float(mViewport->getActualHeight());
    int ratoX = Input->mouseX();
    int ratoY = Input->mouseY();

    double mx = (ratoX - pos.x) / largura;
    double my = (ratoY - pos.y) / altura;

    Ogre::Entity *tmpE = NULL;
    Ogre::Vector3 result = Ogre::Vector3::ZERO;
    float distToColl;
    Ogre::Ray mouseRay = mViewport->getCamera()->getCameraToViewportRay(mx,my);
    
	if (getGameObject()->getParentScene()->getCollisionTools()->raycastFromCamera(mouseRay, result, tmpE, distToColl, getLayer()))
    {
        Ogre::SceneNode* node = tmpE->getParentSceneNode();
        return (char*)node->getName().c_str();
    }

    return NULL;
}*/

/*char *Camera::mousePickEntity(int mouseX, int mouseY)
{
    float largura = float(mViewport->getActualWidth());
    float altura = float(mViewport->getActualHeight());
    int ratoX = mouseX;
    int ratoY = mouseY;

    double mx = (ratoX) / largura;
    double my = (ratoY) / altura;

    Ogre::Entity *tmpE = NULL;
    Ogre::Vector3 result = Ogre::Vector3::ZERO;
    float distToColl;
    Ogre::Ray mouseRay = mViewport->getCamera()->getCameraToViewportRay(mx,my);
    
	if (getGameObject()->getParentScene()->getCollisionTools()->raycastFromCamera(mouseRay, result, tmpE, distToColl, getLayer()))
    {
        Ogre::SceneNode* node = tmpE->getParentSceneNode();
        return (char*)node->getName().c_str();
    }

    return NULL;
}*/


//----------------------------------------------------------
/*bool Camera::mousePick(const QString &findName,bool cond,QStringList &res,const double& dis, bool isTopWindow)
{
    res.clear();
	
    Vector2 pos = Vector2::ZERO;
	if (!isTopWindow)
	{
		pos = Rendering->getRenderView("render1")->getAbsolutePosition();
	}

    float largura = float(mViewport->getActualWidth());
    float altura = float(mViewport->getActualHeight());
    int ratoX = Input->mouseX();
    int ratoY = Input->mouseY();

    double mx = (ratoX - pos.x) / largura;
    double my = (ratoY - pos.y) / altura;

    // create ray
    Ogre::Ray mouseRay = mViewport->getCamera()->getCameraToViewportRay(mx,my);
    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);

    // execute query
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); ++itr)
    {
        bool visible = (cond == itr->movable->isVisible());
        if (itr->movable && visible && itr->distance <= dis)
        {
            QString str (itr->movable->getName().c_str());
            if(str.contains(findName))
            {
                res << str;
            }
        }
    }

    return (res.size())?true:false;
}*/

//----------------------------------------------------------
/*bool Camera::mousePick(const QString &findName, QString &res, bool isTopWindow)
{
    Vector2 pos = Vector2::ZERO;
	if (!isTopWindow)
	{
		pos = Rendering->getRenderView("render1")->getAbsolutePosition();
	}

    float largura = float(mViewport->getActualWidth());
    float altura = float(mViewport->getActualHeight());
    
	int ratoX = Input->mouseX();
    int ratoY = Input->mouseY();

    double mx = (ratoX - pos.x) / largura;
    double my = (ratoY - pos.y) / altura;

    // create ray
    Ogre::Ray mouseRay = mViewport->getCamera()->getCameraToViewportRay(mx,my);
    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);

    // execute query
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); ++itr)
    {
        if (itr->movable && itr->movable->isVisible())
        {
            QString str (itr->movable->getName().c_str());
            if(str.contains(findName))
            {
                res  = str;
                return true;
            }
        }
    }

    return false;
}*/

void Camera::setBackgroundColor(const Color& col)
{
	mViewport->setBackgroundColour(Convert::toOgre(col));
}

Color Camera::getBackgroundColor()
{
	return Convert::toKernel(mViewport->getBackgroundColour());
}

void Camera::setOrthographic(bool enable)
{
	if (enable)
		mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	else
		mCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
}

void Camera::setOrthoWindow(double w, double h)
{
	mCamera->setOrthoWindow(w, h);
}

void Camera::setOrthoWidth(double v)
{
	mCamera->setOrthoWindowWidth(v);
}

void Camera::setOrthoHeight(double v)
{
	mCamera->setOrthoWindowHeight(v);
}

void Camera::setOgreCamera(Ogre::Camera* cam)
{
	mCamera = cam;
}

const Matrix4& Camera::getProjectionMatrix() const
{
	return Convert::toKernel(mCamera->getProjectionMatrix());
}

const Matrix4& Camera::getViewMatrix() const
{
	return Convert::toKernel(mCamera->getViewMatrix());
}

void Camera::applyDesc(int key, const std::string& value)
{
	switch (key)
	{
	case CD_NearDistance:
		setNearClipDistance(Convert::parseReal(value));
		break;

	case CD_FarDistance:
		setFarClipDistance(Convert::parseReal(value));
		break;

	case CD_LodBias:
		setLodBias(Convert::parseReal(value));
		break;

	case CD_WireFrameActive:
		wireframe(Convert::parseBool(value));
		break;

	case CD_CullingMask:
		setVisibilityMask(Convert::parseInt(value));
		break;

	case CD_Aspect:
		setVisibilityMask(Convert::parseReal(value));
		break;
	
	case CD_Enabled:
		setEnabled(Convert::parseBool(value));
		break;

	default:
		break;
	}
}