///////////////////////////////////////////////////////////
//  SurfaceFileConfigData.cpp
//  Implementation of the Class SurfaceFileConfigData
//  Created on:      23-jun-2012 11:41:15
///////////////////////////////////////////////////////////
 
#include "SurfaceFileConfigData.h"

//-----------------------------------------------------------------------------
SurfaceFileConfigData::SurfaceFileConfigData():DataFileSystem()
{	
	current = 0;
}

//-----------------------------------------------------------------------------
SurfaceFileConfigData::SurfaceFileConfigData(const SurfaceFileConfigData& v)
{
	current = v.current;
}

//-----------------------------------------------------------------------------
SurfaceFileConfigData::~SurfaceFileConfigData()
{	
	
}

//-----------------------------------------------------------------------------
SurfaceFileConfigData& SurfaceFileConfigData::operator = 
	(const SurfaceFileConfigData& v)
{
	if (&v == this)
        return *this; 
	current = v.current;
	return *this; 
}

//-----------------------------------------------------------------------------
unsigned int SurfaceFileConfigData::getCurrentSurface() const
{
	return current;
}

//-----------------------------------------------------------------------------
void SurfaceFileConfigData::setCurrentSurface(unsigned int v)
{
	current = v;
}

