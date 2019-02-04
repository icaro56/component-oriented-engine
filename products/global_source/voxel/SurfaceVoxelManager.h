#ifndef SURFACEVOXEL_MANAGER_H
#define SURFACEVOXEL_MANAGER_H

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

class SurfaceVoxelManager : public MouseEvents
{
public:
	static SurfaceVoxelManager* getInstance();

	void init();

	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

private:
	SurfaceVoxelManager(void);
	~SurfaceVoxelManager(void);

	void drawVolume(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData);
	void drawVolumeData(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData);
	void drawVolumeDataOptimized(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData);

	void findLimitsInFile(const QString& filename);
	void processLineLimits(QString& line);

	void readFile(const QString& filename);
	void processLine(QString& line, PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData);

	/*void convertRealTo3D(Vector3& point) const;
	void convert3DToReal(Vector3& point) const;*/

	void setVoxelScale(double s);
	void setVoxelPosition(const Vector3 &pos);

	void convertRealTo3D(Vector3& point) const;

	PolyVox::Vector3DInt32 convertRealToVoxelPosition(const Vector3& val);
	PolyVox::Vector3DFloat convert3DToVoxelPosition(const Vector3& val);

	double round(double n);
	void extrudeHeightCoord(int offset, int count, const PolyVox::Vector3DInt32& currentpos, PolyVox::MaterialDensityPair88 md,PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volData);


	//editor
	template <typename VoxelType>
    void smoothSphereRegion(PolyVox::SimpleVolume<VoxelType>* volData, const PolyVox::Region& regionToSmooth, PolyVox::Vector3DFloat &sphereCenter, float radius);
   
    void boxBrush(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volume, PolyVox::Vector3DFloat &min, PolyVox::Vector3DFloat &max, uint8_t density, float smooth = 0);
    void sphereBrush(PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88>* volume, PolyVox::Vector3DFloat &position, float radius, uint8_t density, float smooth = 0);

	float getDistanceSquared(const PolyVox::Vector3DFloat& vec1, const PolyVox::Vector3DFloat& vec2);

	template <typename VoxelType>
	void boundaryTesting(PolyVox::Vector3DFloat& val, PolyVox::SimpleVolume<VoxelType>* volData);

private:
	static SurfaceVoxelManager* instance;

	Vector3 minRealPosition, maxRealPosition;//, firstRealPosition;

	double cReal_x1; 
	double cReal_y1;
	double cReal_z1;
	double cReal_x2;
	double cReal_y2;
	double cReal_z2;

	double c3D_x1; 
	double c3D_y1;
	double c3D_z1;
	double c3D_x2;
	double c3D_y2;
	double c3D_z2;

	Vector3 kRto3D;

	PolyVox::SimpleVolume<PolyVox::MaterialDensityPair88> *volData;
	Ogre::ManualObject* PVWorld;
	Ogre::SceneNode* PVWorldNode;

	// escala do mundo voxel
	Vector3 scaleVoxel;
	double diffPoint;

	bool useSurfaceOptimized;
};

#endif