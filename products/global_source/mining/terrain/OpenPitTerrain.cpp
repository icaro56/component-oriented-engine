#include "OpenPitTerrain.h"

#include "system/ProjectConfig.h"
#include <engine/Engine.h>
#include <engine/Scene.h>
#include <engine/RenderView.h>
#include <OgreRenderWindow.h>
#include <Terrain.h>
#include <heightmap/Heightmap.h>
#include <time/Time.h>
#include "surface/SurfaceException.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"
#include "gameobject/GameObject.h"
#include "components/rendering/Camera.h"
#include "components/transform/Transform.h"


//-----------------------------------------------------------------------------
OpenPitTerrain::OpenPitTerrain()
{
	setType(OpenPit);

	terrain = new Terrain();

	terrain->setSceneManager(GEngine->getCurrentScene()->getOgreScene());
	Ogre::Camera* c =  CameraControllerManager::getInstance()->getMainCameraController()->getCamera()->camera->getOgreCamera();
	terrain->setCamera(c);

	lod = 10;
}

//-----------------------------------------------------------------------------
OpenPitTerrain::~OpenPitTerrain()
{
	if(terrain)
	{
		terrain->setLODDistBias(1);
		terrain->destroy();
		delete terrain;
	}
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::init()
{
	QString path(GlobalConfig::TERRAIN_PATH);
    path.append(QString::number(getId()));
    path.append(".dll");

	QDir dir;
	if(!dir.exists(path))
	{
		QString t = QString::number(getId());		

		QFile::copy(GlobalConfig::TERRAIN_FOLDER_DEFAULT + t + ".dll",
			GlobalConfig::TERRAIN_FOLDER + t + ".dll");
	}

	QString auxName = GlobalConfig::TERRAIN_FOLDER + QString::number(getId()) 
								+ ".dll";

	if(!QFile::exists(auxName))
	{
		QString str = QObject::tr("Arquivo de terreno não existe.");
		std::cout << str.toStdString() << std::endl;
		SurfaceException e;
		e.setMessage(str);
		throw e;
	}

	buildTerrainMesh();
}

//-----------------------------------------------------------------------------
int OpenPitTerrain::getLod()
{
	int o = terrain->getLODDistBias();
	return o;
}
//-----------------------------------------------------------------------------
double OpenPitTerrain::getOpacity() const
{
	double o = terrain->getOpacity();
	return o;
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::setOpacity(const double& alpha)
{
	terrain->setOpacity(alpha);
	SurfaceData::setOpacity(alpha);
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::setLod(int l)
{
   SurfaceData::setLod(l);
   terrain->setLODDistBias(l);
   lod = l;
}

//-----------------------------------------------------------------------------
bool OpenPitTerrain::mousePosition(int mX, int mY,double& x, double& y,double & z)
{
	RenderView * rv = Rendering->getRenderView("render1");
	if(rv->isActiveWindow())
    {
        Vector2 pos = rv->getAbsolutePosition();

		double width = double(rv->getRenderWindow()->getWidth());
        double height = double(rv->getRenderWindow()->getHeight());

		double mx = (mX - pos.x);
        double my = (mY - pos.y);
		
		Vector3 mousePos = mouseWorldPosition(mx, my, width, height);
		if( (mousePos.x != 599994) && (mousePos.y != 599994)
                && (mousePos.z != 599994 ))
        {
			x = mousePos.x;
			y = mousePos.y;
			z = mousePos.z;
			return true;
		}
	}
	return false;
}
 
//-----------------------------------------------------------------------------
bool OpenPitTerrain::isSurfaceCollision(Vector3& oldPos, 
								   const double& height, 
								   Vector3 &newPos, 
								   int mask,
								   bool onlyVisible)
{
	getHeightAt(newPos, height, true);

	//newPos.y += height;

	if (oldPos.y < newPos.y)
    {
		return true;
    }
	else
	{
		return false;
	}
}

//-----------------------------------------------------------------------------
double OpenPitTerrain::getHeightAt( const double& vX, const double& vZ, const double& rBonus, 
	bool bUseRoot, bool bUseGeoMorphing, bool onlyVisible )
{
	return terrain->getHeightAt( vX, vZ, rBonus, bUseRoot,  bUseGeoMorphing );
}

//-----------------------------------------------------------------------------
bool OpenPitTerrain::getHeightAt(Vector3& vPos, const double& rBonus, 
	bool bUseRoot, bool bUseGeoMorphing, bool onlyVisible) 
{
	return terrain->getHeightAt(Convert::toOgre(vPos), rBonus, bUseRoot, bUseGeoMorphing);
}

//-----------------------------------------------------------------------------
Vector3 OpenPitTerrain::mouseWorldPosition(const double& xMouse, const double& yMouse, 
		const double& wWindow, const double& hWindow)
{
	GameObject* cam = CameraControllerManager::getInstance()->getMainCameraController()->getCamera();
	Camera* component = dynamic_cast<Camera*>(cam->getComponent("Camera"));

	return Convert::toKernel( terrain->mouseWorldPosition(xMouse, yMouse, 
		wWindow, hWindow, component->getOgreCamera()));
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::update()
{
	double dt = Time->getDeltaTime();

	if ((terrain->getMorphSpeed() != 0) || (terrain->getLODCheckTime() != 0))
    {
        terrain->setMorphSpeed(0);
        terrain->setLODCheckTime(0);
		
    }

    terrain->onFrameStart(dt);
    terrain->onFrameEnd(dt);
	
}

//-----------------------------------------------------------------------------
double OpenPitTerrain::getHeight() const
{
	return terrain->getTerrainHeight();
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::setMaterial(const std::string& materialName, unsigned idEnt)
{
	terrain->setMaterial(materialName);
}

//-----------------------------------------------------------------------------
Ogre::MaterialPtr OpenPitTerrain::getMaterial(unsigned i)
{
	return terrain->getMaterial();
}

//-----------------------------------------------------------------------------
unsigned int OpenPitTerrain::getNumMaterials() const
{
	return 1;
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::createMaterial(const QString& name)
{
	if(getMaterial()->getTechnique(0)->getPass(name.toStdString()) == NULL)
    {
        Ogre::Pass* pass = getMaterial()->getTechnique(0)->createPass();
        pass->setName(name.toStdString());
        Ogre::TextureUnitState * tus = pass->createTextureUnitState(name.toStdString());
        pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        tus->setTextureAnisotropy(16);
        tus->setTextureFiltering(Ogre::FT_MIP, Ogre::FO_ANISOTROPIC);
    }
}

//-----------------------------------------------------------------------------
bool OpenPitTerrain::thisVisible(const Vector3& p,const QString& name)
{
	Transform* transf = Rendering->getRenderView("render1")->getCamera()->transform();
	//Ogre::Viewport *vp = GRender->getRenderView("render1")->getViewport();	
	const Vector3 pos = p + Vector3(0,5,0);
	const Vector3 pos_c = transf->getPosition();
	Vector3 d = (pos-pos_c);
	double dist = pos.distance(pos_c); 
	d.normalise();

	Ogre::Ray mouseRay(Convert::toOgre(pos_c), Convert::toOgre(d));
	
	Ogre::RaySceneQuery *rsq = terrain->createRayQuery(mouseRay);
	rsq->setSortByDistance(true);

	Ogre::RaySceneQueryResult &result = rsq->execute();
    Ogre::RaySceneQueryResult::iterator itr;

    for (itr = result.begin(); itr != result.end(); ++itr)
    {
		if(itr->worldFragment)
		{
			return (itr->distance > dist);
		}
    }

	return true;
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::configureSurfaceMaterial(bool isColor, const QColor& color)
{
	if (isColor)
	{		

		for(unsigned m = 0; m < getNumMaterials(); m++)
		{			
			Ogre::MaterialPtr mat = getMaterial(m);			
			Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
			for(int i = 0; i < pass->getNumTextureUnitStates(); i++)
			{
				//atibui que a unidade de textura vai misturar só o diffuse
				pass->getTextureUnitState(i)->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_COLOUR,
											Ogre::LBS_DIFFUSE, Ogre::LBS_CURRENT);
			}
				
			//fazendo backup do valores do material
			setColorDiffuse(pass->getDiffuse());
			setColorSpecular(pass->getSpecular());
			setColorAmbient(pass->getAmbient());
			setColorSelf(pass->getSelfIllumination());

			pass->setDiffuse(color.redF(), color.greenF(), color.blueF(),1);
			pass->setAmbient(0.1,0.1,0.1);
			pass->setSpecular(1,1,1,1);
			pass->setSelfIllumination(0.1,0.1,0.1);
		}
	}
	else
	{
		for(unsigned m = 0; m < getNumMaterials(); m++)
		{
			Ogre::MaterialPtr mat = getMaterial(m);
			Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
			for(int i = 0; i < pass->getNumTextureUnitStates(); i++)
			{
				//atibui que a unidade de textura vai misturar a textura
				pass->getTextureUnitState(i)->setColourOperationEx(Ogre::LBX_MODULATE,
											Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);
			}

			//retornando os padrões do material
			pass->setDiffuse(getColorDiffuse());
			pass->setAmbient(getColorAmbient());
			pass->setSpecular(getColorSpecular());
			pass->setSelfIllumination(getColorSelf());
		}
	}
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::configureSurfaceColor(const QColor& color)
{
	for(unsigned m = 0; m < getNumMaterials(); m++)
	{
		Ogre::MaterialPtr mat = getMaterial(m);
		Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
		pass->setDiffuse(color.redF(), color.greenF(), color.blueF(),1);
	}
}

//-----------------------------------------------------------------------------
Vector3 OpenPitTerrain::getCenterPosition() const
{
	return SurfaceData::getCenterPosition();
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void OpenPitTerrain::buildTerrainMesh()
{
	const QString &name = QString::number(getId());
	Rendering->addResourceLocation(GlobalConfig::TERRAIN_FOLDER.toStdString() +
		name.toStdString() + ".dll","Zip", name.toStdString());
    Rendering->initialiseResourceGroup(name.toStdString());
    terrain->setViewport(Rendering->getRenderView("render1")->getCamera()->getViewport());
    terrain->setDiscardGeometryData(false);		
	
	QList<double> &list = getDimension3D();

	terrain->quickSetup(std::string(name.toStdString() + ".png"),
               "Terreno", list[1] - list[0], list[5]);	

	terrain->setLightDirection(Convert::toOgre(Vector3(-0.808863, -0.488935, -0.326625)));
    terrain->setAutoUpdateLightmap(false);
    terrain->setLODDistBias(lod);
}

//-----------------------------------------------------------------------------
void OpenPitTerrain::setWireFrame(bool active)
{
	if (active)
	{
		terrain->getMaterial()->getTechnique(0)->getPass(1)->setPolygonMode(Ogre::PM_WIREFRAME);

		if (!terrain->isVisible())
			terrain->setVisible(true);
	}
	else
	{
		terrain->getMaterial()->getTechnique(0)->getPass(1)->setPolygonMode(Ogre::PM_POINTS);

		//
		if (!isSolidFrame() )
		{
			terrain->setVisible(false);
		}
	}
}

//-----------------------------------------------------------------------------
bool OpenPitTerrain::isWireFrame() const
{
	return terrain->getMaterial()->getTechnique(0)->getPass(1)->getPolygonMode() == Ogre::PM_WIREFRAME;
}

void OpenPitTerrain::setSolidFrame(bool active)
{
	if (active)
	{
		terrain->getMaterial()->getTechnique(0)->getPass(1)->setPolygonMode(Ogre::PM_SOLID);

		if (!terrain->isVisible())
			terrain->setVisible(true);
	}
	else
	{
		terrain->getMaterial()->getTechnique(0)->getPass(1)->setPolygonMode(Ogre::PM_POINTS);

		//
		if (!isWireFrame() )
		{
			terrain->setVisible(false);
		}
	}
}

bool OpenPitTerrain::isSolidFrame() const
{
	return terrain->getMaterial()->getTechnique(0)->getPass(1)->getPolygonMode() == Ogre::PM_SOLID;
}

void OpenPitTerrain::configureEffectiveMining()
{
	int imageWidth;
	Ogre::uchar* imgData;

	if (mImg.getWidth() > 2047 && mImg.getWidth() < 2049)
	{
		imageWidth = mImg.getWidth(); //2048
		imgData = mImg.getData();
	}
	else
	{
		imageWidth = 2048;
		imgData = new Ogre::uchar[imageWidth * imageWidth * 4];
	}

	for(int i = 0; i < imageWidth * 4; i += 4)
    {
        for(int j = 0; j < imageWidth * 4; j += 4)
        {
            int iMaskPos = imageWidth * i + j;
            imgData[ iMaskPos ] = 0; //B
            imgData[ iMaskPos + 1 ] = 0; //G
            imgData[ iMaskPos + 2 ] = 0; //R
            imgData[ iMaskPos + 3 ] = 0; //A
        }
    }

    mImg.loadDynamicImage(imgData, imageWidth, imageWidth, 1, Ogre::PF_A8R8G8B8);
}

HEIGHTMAPTYPE* OpenPitTerrain::getHeightMapData()
{
	return terrain->getHeightmapData();
}

size_t OpenPitTerrain::getHeightMapWidth()
{
	return terrain->getHeightmap()->getWidth();
}

/*
//-----------------------------------------------------------------------------
void OpenPitTerrain::swapTerrain(const QString& terrain)
{
	QString newTerrain = convertToWrite(terrain);

    setTerrainDate(newTerrain);

    QString path(GlobalConfig::TERRAIN_PATH);
    path.append(newTerrain);
    path.append(".dll");

    setFilePath(path);
	readDimensions();

    changeTerrainMesh();
}
*/

/*
//-----------------------------------------------------------------------------
void OpenPitTerrain::initSecond()
{
	readDimensions();

	setSceneManager(GRender->getSceneManager());
	setCamera(GRender->getSceneManager()->getCamera("AllCamera"));

	buildTerrainMesh();
}
*/

/*
//-----------------------------------------------------------------------------
void OpenPitTerrain::changeTerrainMesh()
{
	GRender->addResourceLocation(GlobalConfig::TERRAIN_FOLDER.toStdString() + terrainDate.toStdString() + ".dll","Zip", terrainDate.toStdString());
    GRender->initialiseResourceGroup(terrainDate.toStdString());
    destroy();
    std::string s = terrainDate.toStdString() + ".png";
    initialize(s,0.0);
    setMaterial("Terreno");
    setLODDistBias(lod);

    if(Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(lastTerrainDate.toStdString()))
    {
        Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(lastTerrainDate.toStdString());
    }
}
*/