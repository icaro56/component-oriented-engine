///////////////////////////////////////////////////////////
//  OpenPitFactory.cpp
//  Implementation of the Class OpenPitFactory
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#include "OpenPitFactory.h"
#include "mining/terrain/OpenPitTerrain.h"

//-----------------------------------------------------------------------------
OpenPitFactory::OpenPitFactory()
{
}

//-----------------------------------------------------------------------------
OpenPitFactory::~OpenPitFactory()
{
}

//-----------------------------------------------------------------------------
Surface * OpenPitFactory::createSurface()
{
	return new OpenPitTerrain();
}




