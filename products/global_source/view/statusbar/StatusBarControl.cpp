#include "StatusBarControl.h"

#include "StatusBar.h"

#include "system/ProjectConfig.h"
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>

#include "surface/Surface.h"
#include "surface/SurfaceManager.h"

#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"

#include "gameobject/GameObject.h"
#include "components/transform/Transform.h"

StatusBarControl* StatusBarControl::instance = 0;

//-----------------------------------------------------------------------------
StatusBarControl::StatusBarControl()
	:UiBasic(Ui_StatusBar),
	view(0),act(0)
{	
	view = new StatusBar();

	connects();
}

//-----------------------------------------------------------------------------
StatusBarControl::~StatusBarControl()
{
}

//-----------------------------------------------------------------------------
StatusBarControl* StatusBarControl::getInstance()
{
	if (!instance)
	{
		instance = new StatusBarControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void StatusBarControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void StatusBarControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{			
		act = new QAction(QObject::tr("&Barra de Status"),menu);
		act->setStatusTip(QObject::tr("Ativa/desativa a Barra de Status"));
		act->setCheckable(true);
		act->setChecked(false);

		menu->addAction(act);
	}	
}

//-----------------------------------------------------------------------------
void StatusBarControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(act);
	}			
}

//-----------------------------------------------------------------------------
void StatusBarControl::addStatusBar(QStatusBar* sb)
{
	view->setupUi(sb);

	if (act)
		QObject::connect(act, SIGNAL(toggled(bool)),sb, SLOT(setVisible(bool)) );
}

//-----------------------------------------------------------------------------
void StatusBarControl::setEnabledUi(bool cond)
{
	if(!act->isVisible()) return;

	act->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void StatusBarControl::setCheckedUi(bool cond)
{	
	if(!act->isVisible()) return;

	act->setChecked(cond);
}

//-----------------------------------------------------------------------------
void StatusBarControl::setVisibleUi(bool cond)
{	
	act->setVisible(cond);
}

//-----------------------------------------------------------------------------
void StatusBarControl::connects()
{		
	CameraController* cm = CameraControllerManager::getInstance()->getMainCameraController();
	QObject::connect(cm,
					 SIGNAL(cameraMoved()),
					 this,
					 SLOT(receivingCamInfo()) );
}

//----------------------------------------------------------------------------
void StatusBarControl::receivingCamInfo()
{
	GameObject* cam = CameraControllerManager::getInstance()->getMainCameraController()->getCamera();
	Transform* transform = dynamic_cast<Transform*>(cam->getComponent("Transform"));

	Vector3 pos = transform->getPosition();
	setCameraCoordinated(pos.x, pos.y, pos.z);
}

//-----------------------------------------------------------------------------
void StatusBarControl::setCameraCoordinated(const double& x,const double& y,
																const double& z)
{
	QString type = view->getTypeCoordinated();
	QStringList r = calculateCoordinated(type,x,y,z);
	view->setCameraCoordinated(r[0],r[1],r[2]);
}

//-----------------------------------------------------------------------------
void StatusBarControl::setGizmoCoordinated(const double& x,const double& y,
																const double& z)
{
	QString type = view->getTypeCoordinated();
	QStringList r = calculateCoordinated(type,x,y,z);
	view->setGizmoCoordinated(r[0],r[1],r[2]);
}


//-----------------------------------------------------------------------------
QStringList StatusBarControl::calculateCoordinated(const QString& type,const double& x3d, 
	const double& y3d, 
		const double& z3d)
{
    double x = x3d;
	double y = y3d;
	double z = z3d;
	QStringList r;
	if(type == QObject::tr(" Coordenada Real "))
	{		
		SurfaceManager::getInstance()->convert3DToReal(x,y,z);
		r << QString::number(x,'f',2);
		r << QString::number(y,'f',2);
		r << QString::number(z,'f',2);
	}
	else if(type == QObject::tr(" Coordenada 3D "))
	{
		r << QString::number(x,'f',2);
		r << QString::number(y,'f',2);
		r << QString::number(z,'f',2);
	}
	else
	{
		r << "-";
		r << "-";
		r << "-";
	}

	return r;
}