#ifndef CAMERABASICS_H
#define CAMERABASICS_H

#include "OgreVector3.h"
#include "OgreQuaternion.h"

enum CameraBehaviorTypes
{
	CBT_NONE,
	CBT_FREE,
	CBT_ORBIT,
	CBT_AVATAR,
	CBT_AUTOMATIC,
	CBT_GOOGLE_EARTH
};

struct PlaceRecovery
{
public:
    Vector3 position;
    Quaternion orientation;

    PlaceRecovery()
		: position(Vector3::ZERO), 
		orientation(Quaternion::IDENTITY),
		  wasInit(false)
    {

    }

    void setValue(const Vector3 &p, const Quaternion &o)
    {
        position = p;
        orientation = o;
		wasInit = true;
    }

	bool wasInitialized()
	{
		return wasInit;
	}

private:
	bool wasInit;

};

#endif