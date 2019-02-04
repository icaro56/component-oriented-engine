#include "SurfaceVoxelManager.h"
#include "VoxelRaycastCallback.h"

#include "polyvoxcore/CubicSurfaceExtractorWithNormals.h"
#include "polyvoxcore/CubicSurfaceExtractor.h"
#include "polyvoxcore/MarchingCubesSurfaceExtractor.h"
#include "polyvoxcore/SurfaceMesh.h"
#include "polyvoxcore/Raycast.h"
#include "polyvoxcore/MeshDecimator.h"
#include "PolyVoxCore/LowPassFilter.h"
#include "PolyVoxCore/RawVolume.h"

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
#include "gameobject/GameObject.h"
#include "components/rendering/Camera.h"

#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>

#include "system/ProjectConfig.h"
#include "navigation/CameraControllerManager.h"
#include "navigation/CameraController.h"
#include "surface/SurfaceManager.h"


SurfaceVoxelManager* SurfaceVoxelManager::instance = 0;

SurfaceVoxelManager* SurfaceVoxelManager::getInstance()
{
	if (!instance)
	{
		instance = new SurfaceVoxelManager();
	}

	return instance;
}

SurfaceVoxelManager::SurfaceVoxelManager(void)
{
	scaleVoxel = 1.0;
	minRealPosition = Vector3(99999999.0, 99999999.0, 99999999.0);
	maxRealPosition = Vector3::ZERO;
	diffPoint = 0;

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

	//firstRealPosition = Vector3::ZERO;
	useSurfaceOptimized = false;
}


SurfaceVoxelManager::~SurfaceVoxelManager(void)
{
}

void SurfaceVoxelManager::init()
{
	findLimitsInFile("S_P0.dat");
	//findLimitsInFile("S_P1_big.dat");

	readFile("S_P0.dat");
	//readFile("S_P1_big.dat");

	Input->addMouseListener(this);
}

bool SurfaceVoxelManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
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

			PolyVox::Vector3DFloat position = convert3DToVoxelPosition(Convert::toKernel(mouseRay.getOrigin()));

			Vector3 dir = Convert::toKernel(mouseRay.getDirection());
			dir.normalise();
			dir *= 100;

			PolyVox::Vector3DFloat direction(dir.x, dir.y, dir.z);
		
			VoxelRaycastCallbackDensity raycastTestFunctor;
			PolyVox::RaycastResult raycastResult = PolyVox::raycastWithDirection(volData, position, direction, raycastTestFunctor);

			if (raycastResult == PolyVox::RaycastResults::Interupted)
			{
				std::cerr << "\nInterrupted: " << raycastTestFunctor.getLastPosition();
				/*std::cerr << "\nFound: " << raycastTestFunctor.foundIntersection();
				std::cerr << "\nTotal: " << raycastTestFunctor.getTotalVoxelsTouched();*/

				if (volData->getEnclosingRegion().containsPoint(raycastTestFunctor.getLastPosition()))
				{
					/*  remove um marching cube
					PolyVox::MaterialDensityPair88 md;
					md.setMaterial(0);
					md.setDensity(0);

					volData->setVoxelAt(raycastTestFunctor.getLastPosition(), md);

					*/

					/* Adiciona uma esfera
					sphereBrush(volData, 
								PolyVox::Vector3DFloat(raycastTestFunctor.getLastPosition()),
								4, 255, 3);*/

					/* Adiciona um cubo */
					PolyVox::Vector3DFloat min(raycastTestFunctor.getLastPosition());

					PolyVox::Vector3DFloat max = min;

					min -= PolyVox::Vector3DFloat(1,1,1);
					max += PolyVox::Vector3DFloat(1,1,1);

					boxBrush(volData, min, max, 255);

					drawVolume(volData);
				}
			}
			else if (raycastResult == PolyVox::RaycastResults::Completed)
			{
				std::cerr << "\nCompleted: " << raycastTestFunctor.getLastPosition();
				if (volData->getEnclosingRegion().containsPoint(raycastTestFunctor.getLastPosition()))
				{
					if (Input->getKey(OIS::KC_LCONTROL))
					{
						PolyVox::MaterialDensityPair88 md;
						md.setMaterial(255);
						md.setDensity(15);

						volData->setVoxelAt(raycastTestFunctor.getLastPosition(), md);

						drawVolume(volData);
					}
				}
			}
		}
	}

	return true;
}

void SurfaceVoxelManager::drawVolume(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData)
{
	if (!useSurfaceOptimized)
		drawVolumeData(volData);
	else
		drawVolumeDataOptimized(volData);
}

void SurfaceVoxelManager::drawVolumeData(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData)
{
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh;

	PolyVox::Region region = volData->getEnclosingRegion();
	region.setUpperCorner(region.getUpperCorner() + PolyVox::Vector3DInt32(1,1,1));
	region.setLowerCorner(region.getLowerCorner() - PolyVox::Vector3DInt32(1,1,1));

	PolyVox::MarchingCubesSurfaceExtractor< PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88> >surfaceExtractor(volData, region, &mesh);
	surfaceExtractor.execute();


	//reduz o número de triângulos por mesh
	/*PolyVox::MeshDecimator<PolyVox::PositionMaterialNormal> decim(&mesh, &mesh2);
    decim.execute();*/

	uint32_t noVertices = mesh.getNoOfVertices();
    uint32_t noIndices = mesh.getNoOfIndices();

	PVWorld->estimateVertexCount(noVertices);
	PVWorld->estimateIndexCount(noIndices);

	//PVWorld->beginUpdate(0);
	PVWorld->clear();
	PVWorld->begin("triplanarReferenceHigh", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	
	
	// vertices
	const std::vector<PolyVox::PositionMaterialNormal>& vVertices = mesh.getVertices();

	for(std::vector<PolyVox::PositionMaterialNormal>::const_iterator it = mesh.getVertices().begin(); it != mesh.getVertices().end(); ++it)
    {
        const PolyVox::Vector3DFloat& vertexPos = it->getPosition();
        const PolyVox::Vector3DFloat& vertexNorm = it->getNormal();
        PVWorld->position(vertexPos.getX(),vertexPos.getY(),vertexPos.getZ());
        PVWorld->normal(vertexNorm.getX(),vertexNorm.getY(),vertexNorm.getZ());
    }

	//Now we iterate through all the indices from the mesh and also add them to the ManualObject section
    for(std::vector<uint32_t>::const_iterator it = mesh.getIndices().begin(); it != mesh.getIndices().end(); ++it)
    {
        PVWorld->index(*it);
    }

    PVWorld->end();
}

void SurfaceVoxelManager::drawVolumeDataOptimized(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData)
{
	PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> mesh, mesh2;

	PolyVox::Region region = volData->getEnclosingRegion();
	//region.setUpperCorner(region.getUpperCorner() + PolyVox::Vector3DInt32(1,1,1));
	//region.setLowerCorner(region.getLowerCorner() - PolyVox::Vector3DInt32(1,1,1));
	region.shiftLowerCorner(PolyVox::Vector3DInt32(-1,-1,-1));
    region.shiftUpperCorner(PolyVox::Vector3DInt32(1,1,1));

	PolyVox::MarchingCubesSurfaceExtractor< PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88> >surfaceExtractor(volData, region, &mesh);
	surfaceExtractor.execute();


	//reduz o número de triângulos por mesh
	PolyVox::MeshDecimator<PolyVox::PositionMaterialNormal> decim(&mesh, &mesh2);
    decim.execute();

	uint32_t noVertices = mesh2.getNoOfVertices();
    uint32_t noIndices = mesh2.getNoOfIndices();

	PVWorld->estimateVertexCount(noVertices);
	PVWorld->estimateIndexCount(noIndices);

	//PVWorld->beginUpdate(0);
	PVWorld->clear();
	PVWorld->begin("triplanarReferenceHigh", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	
	
	// vertices
	const std::vector<PolyVox::PositionMaterialNormal>& vVertices = mesh2.getVertices();

	for(std::vector<PolyVox::PositionMaterialNormal>::const_iterator it = mesh2.getVertices().begin(); it != mesh2.getVertices().end(); ++it)
    {
        const PolyVox::Vector3DFloat& vertexPos = it->getPosition();
        const PolyVox::Vector3DFloat& vertexNorm = it->getNormal();
        PVWorld->position(vertexPos.getX(),vertexPos.getY(),vertexPos.getZ());
        PVWorld->normal(vertexNorm.getX(),vertexNorm.getY(),vertexNorm.getZ());
    }

	//Now we iterate through all the indices from the mesh and also add them to the ManualObject section
    for(std::vector<uint32_t>::const_iterator it = mesh2.getIndices().begin(); it != mesh2.getIndices().end(); ++it)
    {
        PVWorld->index(*it);
    }

    PVWorld->end();
}

void SurfaceVoxelManager::findLimitsInFile(const QString& filename)
{
	QFile file(GlobalConfig::BLOCKS_DIR + filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

	//pular cabeçalho
	in.readLine();


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

	QList<double> points3D = SurfaceManager::getInstance()->getDimension3D();

	c3D_x1 = points3D.at(0); 
	c3D_y1 = points3D.at(2);
	c3D_z1 = points3D.at(4);
	
	if ( (cReal_x2 - cReal_x1) >= (cReal_y2 - cReal_y1) )
	{
		c3D_x2 = points3D.at(1);

		

		c3D_y2 = (((c3D_x2 - c3D_x1) * (cReal_y2 - cReal_y1) ) / (cReal_x2 - cReal_x1)) / 2.0 * (c3D_y1 > 0 ? -1 : -1);

		kRto3D.x = qAbs((c3D_x2 - c3D_x1)/(cReal_x2-cReal_x1));
		kRto3D.y = kRto3D.x;

		c3D_z2 =  (abs(c3D_x2 - c3D_x1) / (cReal_x2 - cReal_x1)) * (cReal_z2 - cReal_z1);
	}
	else
	{
		c3D_y2 = points3D.at(3);

		c3D_x2 = (((c3D_y2 - c3D_y1) * (cReal_x2 - cReal_x1) ) / (cReal_y2 - cReal_y1)) / 2.0 * (c3D_x1 > 0 ? -1 : -1);

		kRto3D.y = qAbs((c3D_y2 - c3D_y1)/(cReal_y2-cReal_y1));
		kRto3D.x = kRto3D.y;

		c3D_z2 =  ( abs(c3D_y2 - c3D_y1) / (cReal_y2 - cReal_y1)) * (cReal_z2 - cReal_z1);
	}

	// exagero vertical
	kRto3D.z = qAbs((c3D_z2 - c3D_z1)/(cReal_z2-cReal_z1));

	setVoxelScale(diffPoint);
}

void SurfaceVoxelManager::processLineLimits(QString& line)
{
	QStringList list = line.split(",");

	if (list.size() == 3)
	{
		Vector3 realPos = Vector3(list.at(0).toDouble(),
											  list.at(1).toDouble(),
											  list.at(2).toDouble());

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

		//fazer isso apenas até a segunda linha do arquivo
		static int count = 0;
		if (count < 2 && (diffPoint == 0 || diffPoint != realPos.x))
		{
			count++;
			diffPoint = realPos.x - diffPoint;
		}
	}
}

void SurfaceVoxelManager::readFile(const QString& filename)
{
	QFile file(GlobalConfig::BLOCKS_DIR + filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

	PolyVox::Vector3DInt32 upperBorder = convertRealToVoxelPosition(Vector3(cReal_x2, cReal_z2, cReal_y2));

	PolyVox::Region region = PolyVox::Region(PolyVox::Vector3DInt32(0, 0, 0), upperBorder);

	volData = new PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>(region);
	
    QTextStream in(&file);

	//pular cabeçalho
	in.readLine();

    while (!in.atEnd()) 
	{
        QString line = in.readLine();

		if (line.size() > 1)
			processLine(line, volData);
    }

	drawVolume(volData);

	Vector3 ponto = Vector3(c3D_x1, c3D_z1, c3D_y2);
	//Vector3 ponto = Vector3(-16000, 0, -16000);

	setVoxelPosition(ponto);
}

void SurfaceVoxelManager::processLine(QString& line, PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData)
{
	QStringList list = line.split(",");

	if (list.size() == 3)
	{
		//convertRealTo3D(pos);
		int x,y,z;

		x = list.at(0).toDouble();
		y = list.at(1).toDouble();
		z = list.at(2).toDouble();

		// populando região
		PolyVox::Vector3DInt32 v3dCurrentPos = convertRealToVoxelPosition(Vector3(x, z, y));

		uint8_t uVoxelValue = 0;
		uint8_t uDensity = 0;
		PolyVox::MaterialDensityPair88 md;
		md.setDensity(0);
		md.setMaterial(0);
		
		if (volData->getEnclosingRegion().containsPoint(v3dCurrentPos))
		{
			md.setMaterial(255);

			md.setDensity(md.getMaxDensity());

			//criando um voxel abaixo de cada ponto
			extrudeHeightCoord(-1, 2, v3dCurrentPos, md, volData);
		}

		//Wrte the voxel value into the volume
		volData->setVoxelAt(v3dCurrentPos.getX(), v3dCurrentPos.getY(), v3dCurrentPos.getZ(), md);
	}
}

void SurfaceVoxelManager::setVoxelScale(double s)
{
	scaleVoxel.x = s * kRto3D.x;
	scaleVoxel.y = s * kRto3D.z;
	scaleVoxel.z = s * kRto3D.y;

	PVWorldNode->setScale(Convert::toOgre(scaleVoxel));
	//PVWorldNode->translate(-scaleVoxel, -scaleVoxel, -scaleVoxel, Ogre::Node::TS_WORLD);

	/*Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingletonPtr()->getByName("ColouredCubicVoxel");
	mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("scale", (float)scaleVoxel);*/
}

void SurfaceVoxelManager::setVoxelPosition(const Vector3 &pos)
{
	PVWorldNode->setPosition(pos.x, pos.y, pos.z);

	// offset de recuo
	//PVWorldNode->translate(-scaleVoxel*1.5, -scaleVoxel, -scaleVoxel/2, Ogre::Node::TS_WORLD);
	PVWorldNode->translate(-scaleVoxel.x, -scaleVoxel.z, -scaleVoxel.y, Ogre::Node::TS_WORLD);


	//Debugando

	/*EntityOld* ent = new EntityOld("esgefdfads", "Sphere02.mesh");
	ent->scale(250,250,250);
	ent->setPosition(pos);*/
}

void SurfaceVoxelManager::convertRealTo3D(Vector3& point) const
{
	//isInitiated();

    double auxY = point.y;
    point.x = c3D_x1 + (point.x - cReal_x1)*
                (c3D_x2 - c3D_x1)/
                (cReal_x2 - cReal_x1);
    point.y = c3D_z1 + (point.z - cReal_z1)*
                (c3D_z2 - c3D_z1)/
                (cReal_z2 - cReal_z1);
    point.z = c3D_y1 + (auxY - cReal_y1)*
                (c3D_y2 - c3D_y1)/
                (cReal_y2 - cReal_y1);
}

PolyVox::Vector3DInt32 SurfaceVoxelManager::convertRealToVoxelPosition(const Vector3 &val)
{
	PolyVox::Vector3DInt32 result;


	result.setX(round(val.x)/diffPoint - round(cReal_x1)/diffPoint);
	result.setY(round(val.y)/diffPoint - round(cReal_z1)/diffPoint);
	result.setZ(round(val.z)/diffPoint - round(cReal_y1)/diffPoint);

	return result;
}

PolyVox::Vector3DFloat SurfaceVoxelManager::convert3DToVoxelPosition(const Vector3 &val)
{
	Vector3 ponto = Vector3(c3D_x1, c3D_z1, c3D_y2);

	Vector3 iniPos = (val - ponto) / scaleVoxel;
		
	PolyVox::Vector3DFloat position(iniPos.x, iniPos.y, iniPos.z);

	return position;
}

double SurfaceVoxelManager::round(double n)
{
	return floor(n + 0.5);
}

void SurfaceVoxelManager::extrudeHeightCoord(int offset, int count, const PolyVox::Vector3DInt32& currentpos, PolyVox::MaterialDensityPair88 md, PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData)
{
	//criando um voxel abaixo de cada ponto
	PolyVox::Vector3DInt32 belowPos = currentpos;

	for (int i = 0; i < count; i++)
	{
		belowPos.setY(belowPos.getY() + offset);

		if (volData->getEnclosingRegion().containsPoint(belowPos))
			volData->setVoxelAt(belowPos.getX(), belowPos.getY(), belowPos.getZ(), md);
	}
}

void SurfaceVoxelManager::boxBrush(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volume, PolyVox::Vector3DFloat &min, PolyVox::Vector3DFloat &max, uint8_t density, float smooth)
{
	// testando bordas
	boundaryTesting(min, volume);
	boundaryTesting(max, volume);

    PolyVox::Vector3DFloat boxCenter = (max + min);
	boxCenter /= 2;

    PolyVox::Vector3DFloat boxExtents = boxCenter - min;

    float xDiff, yDiff, zDiff;
    float maxDiff;

    PolyVox::Vector3DFloat innerExtents = boxExtents - PolyVox::Vector3DFloat(smooth,smooth,smooth);

    for (int z = min.getZ(); z <= max.getZ(); z++)
    {
        for (int y = min.getY(); y <= max.getY(); y++)
        {
			for (int x = min.getX(); x <= max.getX(); x++)
			{
				// Store our current position as a vector...
				PolyVox::Vector3DFloat v3dCurrentPos(x,y,z);
               
				// Our new density value

				xDiff = 1 - Ogre::Math::Clamp( int((abs(v3dCurrentPos.getX() - boxCenter.getX()) - innerExtents.getX()) / smooth), 0, 1);
				yDiff = 1 - Ogre::Math::Clamp( int((abs(v3dCurrentPos.getY() - boxCenter.getY()) - innerExtents.getY()) / smooth), 0, 1);
				zDiff = 1 - Ogre::Math::Clamp( int((abs(v3dCurrentPos.getZ() - boxCenter.getZ()) - innerExtents.getZ()) / smooth), 0, 1);
			 
				maxDiff = std::max((int)xDiff, std::max((int)yDiff, (int)zDiff)) ;

				uint8_t uDensity = density * maxDiff;

				// Get the old voxel
				PolyVox::MaterialDensityPair88 voxel = volume->getVoxelAt(x,y,z);

				// Modify the density
				voxel.setDensity(uDensity);

				// Write the voxel value into the volume
				volume->setVoxelAt(x, y, z, voxel);
               
			}
        }
    }

    //PolyVox::smoothRegion(volume, PolyVox::Region(PolyVox::Vector3DInt16(min.x, min.y, min.z), PolyVox::Vector3DInt16(max.x, max.y, max.z));
}

void SurfaceVoxelManager::sphereBrush(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volume, PolyVox::Vector3DFloat &position, float radius, uint8_t density, float smooth)
{
    float smoothSQ = smooth * smooth;
    float radiusSQ = radius * radius;
    float distanceSQ;
    float diff;
      
    for (int z = (position.getZ() - radius); z <= (position.getZ() + radius); z++)
    {
        for (int y = (position.getY() - radius); y <= (position.getY() + radius); y++)
        {
			for (int x = (position.getX() - radius); x <= (position.getX() + radius); x++)
			{
				// Store our current position as a vector...
				PolyVox::Vector3DFloat v3dCurrentPos(x,y,z);
				// And compute how far the current position is from the center of the volume

				if (volData->getEnclosingRegion().containsPoint(v3dCurrentPos))
				{

					distanceSQ = getDistanceSquared(position, v3dCurrentPos);
               
					if (distanceSQ <= radiusSQ)
					{
						// Our new density value
						uint8_t uDensity = density;
                  
						diff = radiusSQ - distanceSQ;
						
						if (smoothSQ >= diff)
							uDensity = sqrt(abs(distanceSQ-radiusSQ) / smooth) * density;

						//Get the old voxel
						PolyVox::MaterialDensityPair88 voxel = volume->getVoxelAt(x,y,z);

						// Modify the density
						voxel.setDensity(uDensity);

						// Write the voxel value into the volume
						volume->setVoxelAt(x, y, z, voxel);
					}
				}
			}
        }
    }
   
    smoothSphereRegion(volume, 
					   PolyVox::Region( PolyVox::Vector3DInt32(position.getX() - radius, position.getY() - radius, position.getZ() - radius), 
									    PolyVox::Vector3DInt32(position.getX() + radius, position.getY() + radius, position.getZ() + radius)), 
					   position, 
					   radius);
}

template <typename VoxelType>
void SurfaceVoxelManager::smoothSphereRegion(PolyVox::SimpleVolume<VoxelType>* volData, const PolyVox::Region& regionToSmooth, PolyVox::Vector3DFloat& sphereCenter, float radius)
{
    PolyVox::Region croppedRegion = regionToSmooth;
    croppedRegion.cropTo(volData->getEnclosingRegion());

    PolyVox::Array<3, uint16_t> temp(PolyVox::ArraySizes(croppedRegion.width())(croppedRegion.height())(croppedRegion.depth()));

    for (int z = croppedRegion.getLowerCorner().getZ(); z < croppedRegion.getUpperCorner().getZ(); z++)
    {
        for (int y = croppedRegion.getLowerCorner().getY(); y < croppedRegion.getUpperCorner().getY(); y++)
        {
        for (int x = croppedRegion.getLowerCorner().getX(); x < croppedRegion.getUpperCorner().getX(); x++)
        {
            uint16_t& uDensity = temp[x-croppedRegion.getLowerCorner().getX()][y-croppedRegion.getLowerCorner().getY()][z-croppedRegion.getLowerCorner().getZ()];

            uDensity=0;
            uDensity += volData->getVoxelAt(x-1,y-1,z-1).getDensity();
            uDensity += volData->getVoxelAt(x-1,y-1,z-0).getDensity();
            uDensity += volData->getVoxelAt(x-1,y-1,z+1).getDensity();
            uDensity += volData->getVoxelAt(x-1,y-0,z-1).getDensity();
            uDensity += volData->getVoxelAt(x-1,y-0,z-0).getDensity();
            uDensity += volData->getVoxelAt(x-1,y-0,z+1).getDensity();
            uDensity += volData->getVoxelAt(x-1,y+1,z-1).getDensity();
            uDensity += volData->getVoxelAt(x-1,y+1,z-0).getDensity();
            uDensity += volData->getVoxelAt(x-1,y+1,z+1).getDensity();

            uDensity += volData->getVoxelAt(x-0,y-1,z-1).getDensity();
            uDensity += volData->getVoxelAt(x-0,y-1,z-0).getDensity();
            uDensity += volData->getVoxelAt(x-0,y-1,z+1).getDensity();
            uDensity += volData->getVoxelAt(x-0,y-0,z-1).getDensity();
            uDensity += volData->getVoxelAt(x-0,y-0,z-0).getDensity();
            uDensity += volData->getVoxelAt(x-0,y-0,z+1).getDensity();
            uDensity += volData->getVoxelAt(x-0,y+1,z-1).getDensity();
            uDensity += volData->getVoxelAt(x-0,y+1,z-0).getDensity();
            uDensity += volData->getVoxelAt(x-0,y+1,z+1).getDensity();

            uDensity += volData->getVoxelAt(x+1,y-1,z-1).getDensity();
            uDensity += volData->getVoxelAt(x+1,y-1,z-0).getDensity();
            uDensity += volData->getVoxelAt(x+1,y-1,z+1).getDensity();
            uDensity += volData->getVoxelAt(x+1,y-0,z-1).getDensity();
            uDensity += volData->getVoxelAt(x+1,y-0,z-0).getDensity();
            uDensity += volData->getVoxelAt(x+1,y-0,z+1).getDensity();
            uDensity += volData->getVoxelAt(x+1,y+1,z-1).getDensity();
            uDensity += volData->getVoxelAt(x+1,y+1,z-0).getDensity();
            uDensity += volData->getVoxelAt(x+1,y+1,z+1).getDensity();
            uDensity /= 27;
        }
        }
    }

    float radiusSQ = radius * radius;
    for (int z = croppedRegion.getLowerCorner().getZ(); z < croppedRegion.getUpperCorner().getZ(); z++)
    {
        for (int y = croppedRegion.getLowerCorner().getY(); y < croppedRegion.getUpperCorner().getY(); y++)
        {
        for (int x = croppedRegion.getLowerCorner().getX(); x < croppedRegion.getUpperCorner().getX(); x++)
        {
            PolyVox::Vector3DFloat v3dCurrentPos(x,y,z);
            if (getDistanceSquared(v3dCurrentPos, sphereCenter) > radiusSQ)
                continue;

            uint16_t& uDensity = temp[x-croppedRegion.getLowerCorner().getX()][y-croppedRegion.getLowerCorner().getY()][z-croppedRegion.getLowerCorner().getZ()];

            PolyVox::MaterialDensityPair88 val = volData->getVoxelAt(x,y,z);
            val.setDensity(uDensity);
            volData->setVoxelAt(x,y,z,val);
        }
        }
    }
}

float SurfaceVoxelManager::getDistanceSquared(const PolyVox::Vector3DFloat& vec1, const PolyVox::Vector3DFloat& vec2)
{
    PolyVox::Vector3DFloat diffVec = vec2 - vec1;

	float distance =  diffVec.lengthSquared();

    //float distance = diffVec.getX() * diffVec.getX() + diffVec.getY() * diffVec.getY() + diffVec.getZ() * diffVec.getZ();

    return distance;
}

template <typename VoxelType>
void SurfaceVoxelManager::boundaryTesting(PolyVox::Vector3DFloat& val, PolyVox::SimpleVolume<VoxelType>* volume)
{
	PolyVox::Region region = volume->getEnclosingRegion();

	if (!region.containsPoint(val))
	{
		PolyVox::Vector3DFloat lower(region.getLowerCorner());
		PolyVox::Vector3DFloat upper(region.getUpperCorner());

		if (val.getX() < lower.getX())
			val.setX(lower.getX());
		else if (val.getX() > upper.getX())
			val.setX(upper.getX());

		if (val.getY() < lower.getY())
			val.setY(lower.getY());
		else if (val.getY() > upper.getY())
			val.setY(upper.getY());

		if (val.getZ() < lower.getZ())
			val.setZ(lower.getZ());
		else if (val.getZ() > upper.getZ())
			val.setZ(upper.getZ());
	}	
}