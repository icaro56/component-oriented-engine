#include "MyLogic.h"

#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"
#include "surface/SurfaceManager.h"
#include "system/ProjectConfig.h"
#include "gizmo/GizmoControl.h"
#include "avatar/AvatarManager.h"
#include "voxel/VoxelManager.h"
#include "voxel/SurfaceVoxelManager.h"


MyLogic::MyLogic()
{

}

MyLogic::~MyLogic()
{
	CameraControllerManager::getInstance()->destroy();

	AvatarManager::getInstance()->destroy();

	SurfaceManager::getInstance()->destroy();
}

void MyLogic::init()
{
	// Criando controlador da camera principal
	CameraControllerManager::getInstance()->init();

	// Iniciando Terreno
	SurfaceManager::getInstance()->init();

	// Constante de coordenada real para 3d. Atualmente essa constante irá retornar sempre 1.0, pois o terreno 3d está mantendo a proporção do real.
	double constante = SurfaceManager::getInstance()->kRealTo3D();

	// configurando camera principal
	CameraControllerManager::getInstance()->getMainCameraController()->configureMainCamera();

	// configurando velocidade da camera
	CameraControllerManager::getInstance()->setDefaultTopSpeed(GlobalConfig::SPEED_CAM * constante, true);

	// Criando Avatar
	AvatarManager::getInstance()->createAvatar("Avatar", true);

	// Configurando luz ambiente
	GEngine->getCurrentScene()->setAmbientLight(0.5, 0.5, 0.5);

	// Criando skybox
	GEngine->getCurrentScene()->setSkybox("Sunny2.tif", 1500 * constante);	

	// Posicionando gizmo para a posição central
	GizmoControl::getInstance()->setPosition(0,0,0);

	// teste de voxels
	//VoxelManager::getInstance()->init();
	//SurfaceVoxelManager::getInstance()->init();

	//descomentar para esconder terreno
	//SurfaceManager::getInstance()->setOpacity(0);

	SurfaceManager::getInstance()->configureSurfaceMaterial(true, QColor(182, 126, 91));

	// criando objeto Luz
	GameObject* mLightObject = new GameObject("light1");
	mLightObject->addComponent(new Light());
	mLightObject->light->setType(Light::DIRECTIONAL);
    mLightObject->light->setDiffuseColor(0.7, 0.7, 0.7);

	Vector3 dir = Vector3(0, -1, 0);
	dir.normalise();
	mLightObject->transform->setForward(dir);
	mLightObject->light->setVisibilityFlags(VF_RENDER1);

	// criando esfera
	/*GameObject* esfera = new GameObject("EsferaGTeste");
	MeshFilter* filter = new MeshFilter();
	filter->setFileName("Sphere02.mesh");
	esfera->addComponent(filter);
	esfera->addComponent(new MeshRenderer());*/
	/*Billboard* bill = new Billboard();
	bill->setMaterial("TerraVR/SelectBillboardCircle");
	bill->setDimensions(60,60);
	esfera->addComponent(bill);*/
    
	Camera* g2 = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->camera;

	/*esfera->addComponent(new FollowTexture());
	esfera->followTexture->setCamera(g2);
	esfera->followTexture->createElement("jdaodjoad","TerraVR/SelectBillboardCircle",Ogre::Vector2(100,100),Ogre::Vector2(-50,-50));
	esfera->followTexture->createElement("dfjsidi","TerraVR/SelectBillboardCircle",Ogre::Vector2(50,50),Ogre::Vector2(-25,-25));*/

	/*esfera->addComponent(new BoundingBox());
	esfera->boundingBox->setColor(Color(1,0,0));*/

	//Criando um plano
	//GEngine->getCurrentScene()->createPlane();


	// Criando terreno físico
	GameObject* terrainObject = new GameObject("TerrainPhysics");
	TerrainCollider* terrainCollider = new TerrainCollider();

	TerrainData* terrainData = new TerrainData();
	terrainData->setWidth(SurfaceManager::getInstance()->getWidth());
	terrainData->setHeight(SurfaceManager::getInstance()->getSurfaceHeight());
	terrainData->setPageSize(SurfaceManager::getInstance()->getHeightMapWidth());
	terrainData->setData(SurfaceManager::getInstance()->getHeightMapData());

	terrainCollider->setTerrainData(terrainData);
	terrainObject->transform->setPosition(Vector3(-900, 0, -900));
	terrainObject->addComponent(terrainCollider);
	terrainObject->addComponent(new BoundingBox());
	//terrainObject->transform->setPosition(Vector3(0, 0, 0));


	// Classe Manual Object. Usada para desenhar geometrias livres
	/*std::string name = "ManualObjectTest";
	GameObject* MO = new GameObject(name);
	MO->addComponent(new ManualObject());

	QList<Vector3> points;
	QList<Vector3> triangles;
	QList<Vector3> normals;
	QList<Vector2> textureCoord;

	points<<Vector3(1.0,-1.0,1.0)<<Vector3(-1.0,-1.0,-1.0)
		  <<Vector3(1.0,-1.0,-1.0)<<Vector3(-1.0,-1.0,1.0)
		  <<Vector3(1.0,1.0,1.0)<<Vector3(-1.0,-1.0,1.0)
		  <<Vector3(1.0,-1.0,1.0)<<Vector3(-1.0,1.0,1.0)
		  <<Vector3(-1.0,1.0,-1.0)<<Vector3(-1.0,-1.0,-1.0)
		  <<Vector3(-1.0,-1.0,1.0)<<Vector3(1.0,-1.0,-1.0)
	      <<Vector3(1.0,1.0,-1.0)<<Vector3(1.0,-1.0,1.0)
          <<Vector3(1.0,-1.0,-1.0)<<Vector3(-1.0,-1.0,-1.0)
          <<Vector3(-1.0,1.0,1.0)<<Vector3(1.0,1.0,-1.0)
          <<Vector3(-1.0,1.0,-1.0)<<Vector3(1.0,1.0,1.0);

	normals<<Vector3(0.408248,-0.816497,0.408248)<<Vector3(-0.408248,-0.816497,-0.408248)
		   <<Vector3(0.666667,-0.333333,-0.666667)<<Vector3(-0.666667,-0.333333,0.666667)
		   <<Vector3(0.666667,0.333333,0.666667)<<Vector3(-0.666667,-0.333333,0.666667)
		   <<Vector3(0.408248,-0.816497,0.408248)<<Vector3(-0.408248,0.816497,0.408248)
		   <<Vector3(-0.666667,0.333333,-0.666667)<<Vector3(-0.408248,-0.816497,-0.408248)
		   <<Vector3(-0.666667,-0.333333,0.666667)<<Vector3(0.666667,-0.333333,-0.666667)
		   <<Vector3(0.408248,0.816497,-0.408248)<<Vector3(0.408248,-0.816497,0.408248)
		   <<Vector3(0.666667,-0.333333,-0.666667)<<Vector3(-0.408248,-0.816497,-0.408248)
		   <<Vector3(-0.408248,0.816497,0.408248)<<Vector3(0.408248,0.816497,-0.408248)
		   <<Vector3(-0.666667,0.333333,-0.666667)<<Vector3(0.666667,0.333333,0.666667);

	textureCoord<<Vector2(1,0)<<Vector2(0,1)
				<<Vector2(1,1)<<Vector2(0,0)
				<<Vector2(1,0)<<Vector2(0,1)
				<<Vector2(1,1)<<Vector2(0,0)
				<<Vector2(0,1)<<Vector2(1,1)
				<<Vector2(1,0)<<Vector2(0,1)
				<<Vector2(1,1)<<Vector2(0,0)
				<<Vector2(1,0)<<Vector2(0,0)
				<<Vector2(1,0)<<Vector2(0,1)
				<<Vector2(1,1)<<Vector2(0,0);

	triangles<<Vector3(0,1,2)<<Vector3(3,1,0)
			 <<Vector3(4,5,6)<<Vector3(4,7,5)
			 <<Vector3(8,9,10)<<Vector3(10,7,8)
			 <<Vector3(4,11,12)<<Vector3(4,13,11)
			 <<Vector3(14,8,12)<<Vector3(14,15,8)
			 <<Vector3(16,17,18)<<Vector3(16,19,17);

	MO->getComponent<ManualObject>()->makeObject(points,triangles,normals,textureCoord,RenderType::OT_TRIANGLE_LIST);
	MO->addComponent(new Rigidbody());
	MO->addComponent(new BoxCollider());
	MO->transform->setPosition(Vector3(0,1000,0));

	MO->addComponent(new BoundingBox());
	MO->boundingBox->setColor(Color(1,1,0));*/

	//criando veículo
	GameObject* vehicleObject = new GameObject("VehicleObject");
	vehicleObject->addComponent(new Vehicle());
	MeshFilter* filter2 = new MeshFilter();
	filter2->setFileName("Sphere02.mesh");
	vehicleObject->addComponent(filter2);
	vehicleObject->addComponent(new MeshRenderer());
}


void MyLogic::updateLogic()
{
	//evitar bug ao apertar espaço
	if (Rendering->getRenderView("render1")->isOnRender())
	{
	    if (!Rendering->getRenderView("render1")->hasFocus())
             Rendering->getRenderView("render1")->setFocus();
	}

    // updates /////////////////////
	SurfaceManager::getInstance()->update();

	CameraControllerManager::getInstance()->updateCameraControllers();
	AvatarManager::getInstance()->updateAvatars();


#ifdef FUNC_SKYX
	if (SkyXManager::getInstance()->isActive())
	{
		SkyXManager::getInstance()->updateAll(mLight, 
							Time->getDateTime().time());
	}
#endif

#ifdef FUNC_LOG_ROTATE
	LogRotateManager::getInstance()->update();
#endif

	if (Input->getKeyUp(OIS::KC_O))
	{
		static bool active = false;

		active = !active;
		if (active)
		{
			Time->setTimeScale(0.5);
			Time->setFixedDeltaTime(1.0/60.0 * 0.25);
		}
		else
		{
			Time->setTimeScale(1.0);
			Time->setFixedDeltaTime(1.0/60.0);
		}
		
	}
	else if (Input->getKeyUp(OIS::KC_X))
	{
		Camera* cam = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->camera;

		Debug->log(Convert::toString(cam->worldToScreenPoint(go->transform->getPosition() )));
	}
	else if (Input->getKeyUp(OIS::KC_G))
	{

		Vector3 pos = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->transform->getPosition();
		static int x = 0;
		x++;

		go = GameObjectManager::getInstance()->createPrimitive(PrimitiveType::Cube, "BoxTeste" + Convert::toString(x));
		go->transform->setPosition(pos);
		go->addComponent(new Rigidbody());
		go->addComponent(new ConstantForce());

		//go->transform->setLocalScale(Vector3(100,50,100));

		go->constantForce->setForce(Vector3(0,go->rigidbody->getMass() * 9.81f,0));
	}
	else if (Input->getKeyUp(OIS::KC_H))
	{
		Vector3 pos = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->transform->getPosition();

		static int y = 0;
		y+=1;
		GameObject* Box = new GameObject("SphereTeste" + Convert::toString(y));
		Box->transform->setPosition(pos);
		MeshFilter* filter2 = new MeshFilter();
		filter2->setFileName("Sphere02.mesh");
		Box->addComponent(filter2);
		Box->addComponent(new MeshRenderer());
		//Box->transform->setScale(Vector3(2,2,2));
		Box->addComponent(new Rigidbody());
		Box->addComponent(new SphereCollider());
		//Box->transform->setLocalScale(Vector3(100,100,100));
		Camera* g2 = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->camera;

		Box->addComponent(new Billboard());
		Box->billboard->setMaterial("TerraVR/SelectBillboardCircle");
		Box->billboard->setDimensions(10,10);
		Box->billboard->setType(3);
		

		Vector3 dir = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->transform->getForward() * 200;
		Box->rigidbody->setVelocity(dir);

		Box->addComponent(new TrailRenderer());
		Box->trailRenderer->setMaterial("Trail/LightRibbonTrail");
		Box->trailRenderer->setTrailLength(1000);
		Box->trailRenderer->setMaxElements(200);
		Box->trailRenderer->setInitialColour(Color(1,0,0,1));
		Box->trailRenderer->setColourChange(0, Color(0.5f, 0.5f, 0.5f, 1));
		Box->trailRenderer->setInitialWidth(2);
	    //Box->trailRenderer->setAnchor(Box);
	}
	else if (Input->getKeyUp(OIS::KC_J))
	{
		Vector3 pos = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->transform->getPosition();

		static int z = 0;
		z++;
		GameObject* Box = new GameObject("CapsuleTeste" + Convert::toString(z));
		Box->transform->setPosition(pos);
		MeshFilter* filter2 = new MeshFilter();
		filter2->setFileName("capsule.mesh");
		Box->addComponent(filter2);
		Box->addComponent(new MeshRenderer());
		//Box->transform->setScale(Vector3(2,2,2));
		Box->addComponent(new Rigidbody());
		Box->addComponent(new CapsuleCollider());

		//Box->transform->setLocalScale(Vector3(100,100,100));
	}
	else if (Input->getKeyUp(OIS::KC_K))
	{
		static int b = 0;
		b++;
		GameObject* Box = new GameObject("TriangleTeste" + Convert::toString(b));
		Box->transform->setPosition(Vector3(0,100,0));
		MeshFilter* filter2 = new MeshFilter();
		filter2->setFileName("bandeira.mesh");
		Box->addComponent(filter2);
		Box->addComponent(new MeshRenderer());
		//Box->transform->setScale(Vector3(2,2,2));
		Box->addComponent(new Rigidbody());
		Box->addComponent(new MeshCollider());
	}
	else if (Input->getKeyUp(OIS::KC_L))
	{
		Vector3 pos = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->transform->getPosition();

		static int b = 0;
		b++;
		GameObject* Box = new GameObject("ConvexTeste" + Convert::toString(b));
		Box->transform->setPosition(pos);
		MeshFilter* filter2 = new MeshFilter();
		filter2->setFileName("bandeira.mesh");
		Box->addComponent(filter2);
		Box->addComponent(new MeshRenderer());
		//Box->transform->setScale(Vector3(2,2,2));
		Box->addComponent(new Rigidbody());
		MeshCollider* messcollider = new MeshCollider();
		messcollider->setConvex(true);
		Box->addComponent(messcollider);

		//Box->transform->setLocalScale(Vector3(100,100,100));
	}

	if (Input->getMouseButtonUp(OIS::MB_Left))
	{
		Vector3 pos = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->transform->getPosition();
		Vector3 dir = CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->transform->getForward();
		
		if (Physics->raycast(pos, dir))
		{
           Debug->clearDeveloperConsole();
		   Debug->log("Acertou algo.");
		}

		Debug->drawLine(Vector3(pos.x,pos.y+0.01f,pos.z), pos + dir*1000, Color::Red,2,0,true);

		RaycastHitList list = Physics->raycastAll(pos, dir);


		for (int i = 0; i < list.size(); i++)
		{
			Debug->clearDeveloperConsole();
			Debug->log("Acertou: "+list.at(i).getCollider()->getGameObject()->getName());
		}

	}
}