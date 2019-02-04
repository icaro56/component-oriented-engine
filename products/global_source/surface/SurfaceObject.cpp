#include "SurfaceObject.h"

unsigned int SurfaceObject::count = 0;

//-----------------------------------------------------------------------------
SurfaceObject::SurfaceObject()
{
	type = 0;
	count++;
	setId(count);
}

//-----------------------------------------------------------------------------
SurfaceObject::SurfaceObject(const SurfaceObject& v)
{
	id = v.id;
	type = v.type;
}

//-----------------------------------------------------------------------------
SurfaceObject::~SurfaceObject()
{

}

//-----------------------------------------------------------------------------
SurfaceObject& SurfaceObject::operator = (const SurfaceObject& v)
{
	if (&v == this)
        return *this; 
	id = v.id;
	type = v.type;
	return *this;
}

//-----------------------------------------------------------------------------
unsigned int SurfaceObject::getId() const
{
	return id;
}


//-----------------------------------------------------------------------------
void SurfaceObject::setId(unsigned int i)
{
	id = i;
}

//-----------------------------------------------------------------------------
unsigned int SurfaceObject::getType() const
{
	return type;
}


//-----------------------------------------------------------------------------
void SurfaceObject::setType(unsigned int i)
{
	type = i;
}

