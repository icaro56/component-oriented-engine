#include "Gizmo3d.h"
//#include "moc_Gizmo3d.cpp"

#include "view/UiUserLog.h"

//#include <engine/EntityOld.h>
#include <OgreEntity.h>
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"
#include "components/mesh/MeshFilter.h"
#include "components/mesh/MeshRenderer.h"
#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"

//-----------------------------------------------------------------------------
Gizmo3d::Gizmo3d(QObject *parent)
	: QObject(parent), 
	  ent(0)   
{
	axisObj = new AxisObject();
	ent = new GameObject("Gizmo");
	MeshFilter* meshFilter = new MeshFilter();
	meshFilter->setFileName("GizmoAxesMesh.mesh");
	ent->addComponent(meshFilter);
	ent->addComponent(new MeshRenderer());

	ent->renderer->setEnabled(false);
	unsigned layer = LayerMask::nameToLayer("IgnoreRaycast");
	ent->renderer->setLayer(layer);

	//ent->getOgreEntity()->setRenderingDistance(0);
}

//-----------------------------------------------------------------------------
Gizmo3d::~Gizmo3d()
{
	if (ent)
	{
		GameObjectManager::getInstance()->destroy(ent);
		ent = 0;
	}
}

//-----------------------------------------------------------------------------
void Gizmo3d::setPosition(const double&x, const double& y, const double& z)
{
	ent->renderer->setEnabled(true);
	ent->transform->setLocalPosition(Vector3(x,y,z));
}

//-----------------------------------------------------------------------------
void Gizmo3d::setScale(const double& v)
{
	//ent->transform->setLocalScale(Vector3(v+100,v+100,v+100));
	ent->transform->setLocalScale(Vector3(v,v,v));
}

