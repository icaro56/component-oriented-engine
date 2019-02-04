#ifndef VOXEL_MANAGER_H
#define VOXEL_MANAGER_H

#include "polyvoxcore/SimpleVolume.h"
#include "polyvoxcore/MaterialDensityPair.h"

#include <qstring.h>
#include "OgreVector3.h"

#include "input/MouseEvents.h"

namespace Ogre
{
	class ManualObject;
	class SceneNode;
};

class VoxelManager : public MouseEvents
{
public:
	static VoxelManager* getInstance();

	void init();

	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

private:
	VoxelManager(void);
	~VoxelManager(void);

	void drawVolumeData(PolyVox::SimpleVolume<uint8_t>* volData);
	void drawVolumeDataWithNormals(PolyVox::SimpleVolume<uint8_t>* volData);
	void drawVolumeDataWithoutNormals(PolyVox::SimpleVolume<uint8_t>* volData);

	void findLimitsInFile(const QString& filename);
	void processLineLimits(QString& line);

	void readFile(const QString& filename);
	void processLine(QString& line, PolyVox::SimpleVolume<uint8_t>* volData);

	/*void convertRealTo3D(Vector3& point) const;
	void convert3DToReal(Vector3& point) const;*/

	void setVoxelScale(double s);
	void setVoxelPosition(const Vector3 &pos);

	PolyVox::Vector3DInt32 convertRealToVoxelPosition(const Vector3& val);
	PolyVox::Vector3DFloat convert3DToVoxelPosition(const Vector3& val);

	void createSimplePolyvox();
	void createSphereInVolume(PolyVox::SimpleVolume<uint8_t>* volData, float fRadius);

private:
	static VoxelManager* instance;

	Vector3 minRealPosition, maxRealPosition;//, firstRealPosition;

	double cReal_x1; 
	double cReal_y1;
	double cReal_z1;
	double cReal_x2;
	double cReal_y2;
	double cReal_z2;

	/*double c3D_x1; 
	double c3D_y1;
	double c3D_z1;
	double c3D_x2;
	double c3D_y2;
	double c3D_z2;

	double kRto3D;*/

	PolyVox::SimpleVolume<uint8_t> *volData;
	Ogre::ManualObject* PVWorld;
	Ogre::SceneNode* PVWorldNode;

	// dimensão de cada bloco nos eixos x, y e z
	int blockDimension;

	// escala do mundo voxel
	double scaleVoxel;

	bool firstLine;
	bool useCubicExtractorWithNormals;
};

#endif