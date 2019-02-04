#include "Surface.h"

//-----------------------------------------------------------------------------
Surface::Surface(): SurfaceData()
{	
}

//-----------------------------------------------------------------------------
Surface::~Surface()
{	
}

//-----------------------------------------------------------------------------
Surface& Surface::operator = (const SurfaceData& v)
{
	if (&v == this)
        return *this;
	setSurfaceData(v);
	return *this;
}

//-----------------------------------------------------------------------------
bool Surface::isInside(Vector3& worldPosition)
{
    double halfWidth = getWidth()/2;
	bool hasChange = false;

    if(worldPosition.x > halfWidth)
    {
        worldPosition.x = halfWidth;
		hasChange = true;
    }

    if(worldPosition.x < -halfWidth)
    {
        worldPosition.x = -halfWidth;
		hasChange = true;
    }

    if(worldPosition.z > halfWidth)
    {
        worldPosition.z = halfWidth;
		hasChange = true;
    }

    if(worldPosition.z < -halfWidth)
    {
        worldPosition.z = -halfWidth;
		hasChange = true;
    }

	return !hasChange;
}

//-----------------------------------------------------------------------------
bool Surface::thisVisible(const Vector3&,const QString&)
{
	return true;
}

//-----------------------------------------------------------------------------
void Surface::setSurfaceData(const SurfaceData& v)
{
	setId(v.getId());

	setName(v.getName());

	setCreationDate(v.getCreationDate());

	setRegistrationDate(v.getRegistrationDate());

	setDescription(v.getDescription());

	setDimensionReal(v.getDimensionReal());

	//setDimension3D(v.getDimension3D());

	setColorDiffuse(v.getColorDiffuse());
	setColorSpecular(v.getColorSpecular());
	setColorAmbient(v.getColorAmbient());
	setColorSelf(v.getColorSelf());

	setEdited(v.getEdited());

	SurfaceData::init();
}

//-----------------------------------------------------------------------------
void Surface::setLod(int lod)
{
}

//-----------------------------------------------------------------------------
int Surface::getLod()
{
	return 0;
}