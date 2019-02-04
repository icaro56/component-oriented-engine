#include "Gizmo.h"
//#include "moc_Gizmo.cpp"

//#include <engine/Camera.h>
#include "gameobject/GameObject.h"
#include "components/transform/Transform.h"

#ifdef GIZMO_UNIT_TEST
	#define SurfaceManager SurfaceManagerMock
	#define CameraControllerManager CameraControllerManagerMock
	#define CameraController CameraControllerMock
	#include "mock/SurfaceManagerMock.h"
	#include "mock/CameraControllerManagerMock.h"
	#include "mock/CameraControllerMock.h"

#else
	#include "surface/SurfaceManager.h"
	#include "navigation/CameraControllerManager.h"
	#include "navigation/CameraController.h"
#endif

Gizmo::Gizmo(QObject *parent)
	: QObject(parent)
{
	scale = -166666;
}

Gizmo::~Gizmo()
{

}

//-----------------------------------------------------------------------------
void Gizmo::calculeScale()
{
	GameObject* cam = CameraControllerManager::getInstance()->getMainCameraController()->getCamera();
	Transform* transform = dynamic_cast<Transform*>(cam->getComponent("Transform"));

	double dis = transform->getPosition().distance(Vector3(x,y,z));

	//const double& k = SurfaceManager::getInstance()->kRealTo3D();

	double r = 0;

    if (dis < 30/*k*/)
    {
		r = 0.61/*k*/;//0.2;
    }
    else if( dis < 1500/*k */)
    {
        r = dis*0.02/*k*/;
    }	


	if(r && (scale != r))
	{
		scale = r;
		emit setScale(scale);
	}
}

double Gizmo::getScale()
{
	return scale;
}

//-----------------------------------------------------------------------------
void Gizmo::setPosition(const double& x,const double& y,const double& z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	calculeScale();
}

Vector3 Gizmo::getPosition()
{
	return Vector3(x, y, z);
}