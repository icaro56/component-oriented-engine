#include "VoxelManager.h"
#include "VoxelRaycastCallback.h"

#include "polyvoxcore/CubicSurfaceExtractorWithNormals.h"
#include "polyvoxcore/CubicSurfaceExtractor.h"
#include "polyvoxcore/SurfaceMesh.h"
#include "polyvoxcore/Raycast.h"

#include "OgreManualObject.h"
#include "OgreSceneManager.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "OgrePass.h"
#include "OgreTechnique.h"
#include "OgreRenderWindow.h"

#include "engine/Engine.h"
#include "engine/Scene.h"
#include "input/InputManager.h"
//#include "engine/EntityOld.h"
#include "gameobject/GameObject.h"
#include "components/rendering/Camera.h"

#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>

#include "system/ProjectConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"
#include "surface/SurfaceManager.h"

VoxelManager* VoxelManager::instance = 0;

VoxelManager* VoxelManager::getInstance()
{
	if (!instance)
	{
		instance = new VoxelManager();
	}

	return instance;
}

VoxelManager::VoxelManager(void)
{
	scaleVoxel = 1.0;
	minRealPosition = Vector3(99999999.0, 99999999.0, 99999999.0);
	maxRealPosition = Vector3::ZERO;

	cReal_x1 = 0;
	cReal_y1 = 0;
	cReal_z1 = 0;
	cReal_x2 = 0;
	cReal_y2 = 0;
	cReal_z2 = 0;

	//exemplo do wiki do http://www.volumesoffun.com/polyvox/documentation/dokuwiki/rendering_a_volume_in_ogre
	PVWorld = GEngine->getCurrentScene()->getOgreScene()->createManualObject();
	PVWorld->setDynamic(true);

	PVWorldNode = GEngine->getCurrentScene()->getOgreScene()->getRootSceneNode()->createChildSceneNode();
	PVWorldNode->attachObject(PVWorld);

	//double k = SurfaceManager::getInstance()->kRealTo3D();

	//setVoxelScale(k);

	blockDimension = 1;
	firstLine = true;
	//firstRealPosition = Vector3::ZERO;
	useCubicExtractorWithNormals = true;
}


VoxelManager::~VoxelManager(void)
{
}

void VoxelManager::init()
{
	findLimitsInFile("marvin.csv");

	readFile("marvin.csv");

	Input->addMouseListener(this);
}

bool VoxelManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if ( (Input->getRenderView()->isActiveWindow()) && (Input->getRenderView()->isOnRender()) )
	{
		if (id == OIS::MB_Left)
		{
			float mX = arg.state.X.abs;
			float mY = arg.state.Y.abs;

			Vector2 posRender = Rendering->getRenderView("render1")->getAbsolutePosition();

			float largura = float(Rendering->getRenderView("render1")->getRenderWindow()->getWidth());
			float altura = float(Rendering->getRenderView("render1")->getRenderWindow()->getHeight());

			//calculando a posição correta
			double mx = (mX - posRender.x) / largura;
			double my = (mY - posRender.y) / altura;

			GameObject* camObject = CameraControllerManager::getInstance()->getMainCameraController()->getCamera();
			Camera* component = dynamic_cast<Camera*>(camObject->getComponent("Camera"));
			Ogre::Ray mouseRay = component->getOgreCamera()->getCameraToViewportRay(mx, my);

			/*QList<double> realPoints = SurfaceManager::getInstance()->getDimensionReal();
			// calculando a posição correta
			Vector3 ponto = minRealPosition;
			ponto.x = realPoints.at(0);
			ponto.y = realPoints.at(1);
			//ponto.y = maxRealPosition.y;

			SurfaceManager::getInstance()->convertRealTo3D(ponto);

			Vector3 iniPos = mouseRay.getOrigin() - ponto;
			iniPos = (iniPos / scaleVoxel);
		
			PolyVox::Vector3DFloat position(iniPos.x, iniPos.y, iniPos.z);*/

			PolyVox::Vector3DFloat position = convert3DToVoxelPosition(Convert::toKernel(mouseRay.getOrigin()));

			Vector3 dir = Convert::toKernel(mouseRay.getDirection());
			dir.normalise();
			dir *= 1000;

			PolyVox::Vector3DFloat direction(dir.x, dir.y, dir.z);
		
			VoxelRaycastCallback raycastTestFunctor;
			PolyVox::RaycastResult raycastResult = PolyVox::raycastWithDirection(volData, position, direction, raycastTestFunctor);

			/*std::cerr << "\n\nOri: " << position;
			std::cerr << "\nDir: " << direction;*/

			if (raycastResult == PolyVox::RaycastResults::Interupted)
			{
				std::cerr << "\nInterrupted: " << raycastTestFunctor.getLastPosition();
				/*std::cerr << "\nFound: " << raycastTestFunctor.foundIntersection();
				std::cerr << "\nTotal: " << raycastTestFunctor.getTotalVoxelsTouched();*/

				if (volData->getEnclosingRegion().containsPoint(raycastTestFunctor.getLastPosition()))
				{
					volData->setVoxelAt(raycastTestFunctor.getLastPosition(), 11);

					drawVolumeData(volData);


				}
			}
			else if (raycastResult == PolyVox::RaycastResults::Completed)
			{
				//std::cerr << "\nCompletado: " << raycastTestFunctor.getLastPosition();
			}
		}
	}

	return true;
}

void VoxelManager::drawVolumeData(PolyVox::SimpleVolume<uint8_t>* volData)
{
	if (useCubicExtractorWithNormals)
		drawVolumeDataWithNormals(volData);
	else
		drawVolumeDataWithoutNormals(volData);
}

void VoxelManager::drawVolumeDataWithNormals(PolyVox::SimpleVolume<uint8_t>* volData)
{
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;

	
	PolyVox::Region region = volData->getEnclosingRegion();
	region.setUpperCorner(region.getUpperCorner() + PolyVox::Vector3DInt32(1,1,1));
	region.setLowerCorner(region.getLowerCorner() - PolyVox::Vector3DInt32(1,1,1));

	PolyVox::CubicSurfaceExtractorWithNormals< PolyVox::SimpleVolume<uint8_t> >surfaceExtractor(volData, region, &mesh);
	surfaceExtractor.execute();

	uint32_t noVertices = mesh.getNoOfVertices();
    uint32_t noIndices = mesh.getNoOfIndices();

	PVWorld->estimateVertexCount(noVertices);
	PVWorld->estimateIndexCount(noIndices);

	//PVWorld->beginUpdate(0);
	PVWorld->clear();
	PVWorld->begin("ColouredCubicVoxel", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	
	
	// vertices
	const std::vector<PolyVox::PositionMaterialNormal>& vVertices = mesh.getVertices();

	for(std::vector<PolyVox::PositionMaterialNormal>::const_iterator it = mesh.getVertices().begin(); it != mesh.getVertices().end(); ++it)
    {
        const PolyVox::Vector3DFloat& vertexPos = it->getPosition();
        const PolyVox::Vector3DFloat& vertexNorm = it->getNormal();
        PVWorld->position(vertexPos.getX(),vertexPos.getY(),vertexPos.getZ());
        PVWorld->normal(vertexNorm.getX(),vertexNorm.getY(),vertexNorm.getZ());
		
		//Ogre::ColourValue val;
		uint8_t material = it->getMaterial();

		if (material > 0)
		{
			int matid = material - 1;
			//val.r = matid /*% (int)texture_count*/  * ( 1.0f / texture_count);
			PVWorld->textureCoord(Ogre::Vector4(matid,0,0,0));
		}
    }

	//Now we iterate through all the indices from the mesh and also add them to the ManualObject section
    for(std::vector<uint32_t>::const_iterator it = mesh.getIndices().begin(); it != mesh.getIndices().end(); ++it)
    {
        PVWorld->index(*it);
    }

    PVWorld->end();
}

void VoxelManager::drawVolumeDataWithoutNormals(PolyVox::SimpleVolume<uint8_t>* volData)
{
	PolyVox::SurfaceMesh<PolyVox::PositionMaterial> mesh;

	
	PolyVox::Region region = volData->getEnclosingRegion();
	region.setUpperCorner(region.getUpperCorner() + PolyVox::Vector3DInt32(1,1,1));
	region.setLowerCorner(region.getLowerCorner() - PolyVox::Vector3DInt32(1,1,1));

	PolyVox::CubicSurfaceExtractor< PolyVox::SimpleVolume<uint8_t> >surfaceExtractor(volData, region, &mesh);
	surfaceExtractor.execute();

	uint32_t noVertices = mesh.getNoOfVertices();
    uint32_t noIndices = mesh.getNoOfIndices();

	PVWorld->estimateVertexCount(noVertices);
	PVWorld->estimateIndexCount(noIndices);

	//PVWorld->beginUpdate(0);
	PVWorld->clear();
	PVWorld->begin("ColouredCubicVoxelWithoutNormal", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	
	
	// vertices
	const std::vector<PolyVox::PositionMaterial>& vVertices = mesh.getVertices();

	for(std::vector<PolyVox::PositionMaterial>::const_iterator it = mesh.getVertices().begin(); it != mesh.getVertices().end(); ++it)
    {
        const PolyVox::Vector3DFloat& vertexPos = it->getPosition();
        PVWorld->position(vertexPos.getX(),vertexPos.getY(),vertexPos.getZ());
		
		//Ogre::ColourValue val;
		uint8_t material = it->getMaterial();

		if (material > 0)
		{
			int matid = material - 1;
			//val.r = matid /*% (int)texture_count*/  * ( 1.0f / texture_count);
			PVWorld->textureCoord(Ogre::Vector4(matid,0,0,0));
		}
    }

	//Now we iterate through all the indices from the mesh and also add them to the ManualObject section
    for(std::vector<uint32_t>::const_iterator it = mesh.getIndices().begin(); it != mesh.getIndices().end(); ++it)
    {
        PVWorld->index(*it);
    }

    PVWorld->end();
}

void VoxelManager::findLimitsInFile(const QString& filename)
{
	QFile file(GlobalConfig::BLOCKS_DIR + filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

	//pular cabeçalho
	in.readLine();

	firstLine = true;

    while (!in.atEnd()) 
	{
        QString line = in.readLine();

		if (line.size() > 1)
			processLineLimits(line);
    }

	cReal_x1 = minRealPosition.x;
	cReal_y1 = minRealPosition.y;
	cReal_z1 = minRealPosition.z;
	cReal_x2 = maxRealPosition.x;
	cReal_y2 = maxRealPosition.y;
	cReal_z2 = maxRealPosition.z;

	double k = SurfaceManager::getInstance()->kRealTo3D() * blockDimension;
	setVoxelScale(k);
}

void VoxelManager::processLineLimits(QString& line)
{
	QStringList list = line.split(",");

	if (list.size() >= 7)
	{
		Vector3 realPos = Vector3(list.at(4).toDouble(),
											  list.at(5).toDouble(),
											  list.at(6).toDouble());

		if (realPos.x < minRealPosition.x)
			minRealPosition.x = realPos.x;

		if (realPos.y < minRealPosition.y)
			minRealPosition.y = realPos.y;

		if (realPos.z < minRealPosition.z)
			minRealPosition.z = realPos.z;

		if (realPos.x > maxRealPosition.x)
			maxRealPosition.x = realPos.x;

		if (realPos.y > maxRealPosition.y)
			maxRealPosition.y = realPos.y;

		if (realPos.z > maxRealPosition.z)
			maxRealPosition.z = realPos.z;

		if (firstLine)
		{
			firstLine = false;
			blockDimension = list.at(1).toInt();
		}
	}
}

void VoxelManager::readFile(const QString& filename)
{
	QFile file(GlobalConfig::BLOCKS_DIR + filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

	PolyVox::Vector3DInt32 upperBorder = convertRealToVoxelPosition(Vector3(cReal_x2, cReal_z2, cReal_y2));

	PolyVox::Region region = PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), upperBorder);
	/*PolyVox::Region region = PolyVox::Region(PolyVox::Vector3DInt32(cReal_x1/blockDimension, cReal_z1/blockDimension, cReal_y1/blockDimension), 
											 PolyVox::Vector3DInt32(cReal_x2/blockDimension, cReal_z2/blockDimension, cReal_y2/blockDimension));*/

	volData = new PolyVox::SimpleVolume<uint8_t>(region);

    QTextStream in(&file);

	//pular cabeçalho
	in.readLine();

    while (!in.atEnd()) 
	{
        QString line = in.readLine();

		if (line.size() > 1)
			processLine(line, volData);
    }

	drawVolumeData(volData);

	QList<double> realPoints = SurfaceManager::getInstance()->getDimensionReal();

	// calculando a posição correta
	Vector3 ponto = minRealPosition;
	ponto.x = realPoints.at(0);
	ponto.y = realPoints.at(1);
	//ponto.y = maxRealPosition.y;
	SurfaceManager::getInstance()->convertRealTo3D(ponto);

	//Vector3 ponto = Vector3(-16000, 0, -16000);

	setVoxelPosition(ponto);
}

void VoxelManager::processLine(QString& line, PolyVox::SimpleVolume<uint8_t>* volData)
{
	QStringList list = line.split(",");

	if (list.size() >= 7)
	{
		//convertRealTo3D(pos);
		int x,y,z;

		/*x = list.at(4).toDouble() / blockDimension;
		y = list.at(5).toDouble() / blockDimension;
		z = list.at(6).toDouble() / blockDimension;*/

		x = list.at(4).toDouble();
		y = list.at(5).toDouble();
		z = list.at(6).toDouble();

		// populando região
		//PolyVox::Vector3DFloat v3dCurrentPos(x, z, y);	
		PolyVox::Vector3DInt32 v3dCurrentPos = convertRealToVoxelPosition(Vector3(x, z, y));

		uint8_t uVoxelValue = 0;
		
		if (volData->getEnclosingRegion().containsPoint(v3dCurrentPos))
		{
			uint8_t r = Ogre::Math::RangeRandom(1, 10);
			//uint8_t r = 12;
			//Our new voxel value
            uVoxelValue = r;
		}

		//Wrte the voxel value into the volume
		volData->setVoxelAt(v3dCurrentPos.getX(), v3dCurrentPos.getY(), v3dCurrentPos.getZ(), uVoxelValue);
	}
}

void VoxelManager::setVoxelScale(double s)
{
	scaleVoxel = s;

	PVWorldNode->setScale(scaleVoxel, scaleVoxel, scaleVoxel);
	//PVWorldNode->translate(-scaleVoxel, -scaleVoxel, -scaleVoxel, Ogre::Node::TS_WORLD);

	/*Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingletonPtr()->getByName("ColouredCubicVoxel");
	mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("scale", (float)scaleVoxel);*/
}

void VoxelManager::setVoxelPosition(const Vector3 &pos)
{
	PVWorldNode->setPosition(pos.x, pos.y, pos.z);

	// offset de recuo
	//PVWorldNode->translate(-scaleVoxel*1.5, -scaleVoxel, -scaleVoxel/2, Ogre::Node::TS_WORLD);
	PVWorldNode->translate(-scaleVoxel, -scaleVoxel, -scaleVoxel, Ogre::Node::TS_WORLD);


	//Debugando

	/*EntityOld* ent = new EntityOld("esgefdfads", "Sphere02.mesh");
	ent->scale(250,250,250);
	ent->setPosition(pos);*/
}


PolyVox::Vector3DInt32 VoxelManager::convertRealToVoxelPosition(const Vector3 &val)
{
	PolyVox::Vector3DInt32 result;


	result.setX(val.x/blockDimension - cReal_x1/blockDimension);
	result.setY(val.y/blockDimension - cReal_z1/blockDimension);
	result.setZ(val.z/blockDimension - cReal_y1/blockDimension);

	return result;
}

PolyVox::Vector3DFloat VoxelManager::convert3DToVoxelPosition(const Vector3 &val)
{
	//Vector3 ponto = Vector3(-16000, 0, -16000);

	QList<double> realPoints = SurfaceManager::getInstance()->getDimensionReal();

	// calculando a posição correta
	Vector3 ponto = minRealPosition;
	ponto.x = realPoints.at(0);
	ponto.y = realPoints.at(1);

	SurfaceManager::getInstance()->convertRealTo3D(ponto);

	Vector3 iniPos = (val - ponto) / scaleVoxel;
		
	PolyVox::Vector3DFloat position(iniPos.x, iniPos.y, iniPos.z);

	return position;
}


void VoxelManager::createSimplePolyvox()
{
	//polyvox simples
	 volData = new PolyVox::SimpleVolume<uint8_t>(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(63, 63, 63)));
	
	createSphereInVolume(volData, 30);

	drawVolumeData(volData);
}

void VoxelManager::createSphereInVolume( PolyVox::SimpleVolume<uint8_t>* volData, float fRadius)
{
	//This vector hold the position of the center of the volume
	PolyVox::Vector3DFloat v3dVolCenter(volData->getWidth() / 2, volData->getHeight() / 2, volData->getDepth() / 2);

	//This three-level for loop iterates over every voxel in the volume
	for (int z = 0; z < volData->getWidth(); z++)
	{
		for (int y = 0; y < volData->getHeight(); y++)
		{
			for (int x = 0; x < volData->getDepth(); x++)
			{
				//Store our current position as a vector...
				PolyVox::Vector3DFloat v3dCurrentPos(x,y,z);	
				//And compute how far the current position is from the center of the volume
				float fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();

				uint8_t uVoxelValue = 0;

				//If the current voxel is less than 'radius' units from the center then we make it solid.
				if(fDistToCenter <= fRadius)
				{
					//Our new voxel value
                    uVoxelValue = 255;
				}

				//Wrte the voxel value into the volume
                volData->setVoxelAt(x, y, z, uVoxelValue);
			}
		}
	}
}