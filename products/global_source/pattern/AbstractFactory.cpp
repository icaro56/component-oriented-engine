///////////////////////////////////////////////////////////
//  AbstractFactory.cpp
//  Implementation of the Class AbstractFactory
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#include "AbstractFactory.h"
#include "ProjectPrerequisites.h"

#include "mining/mine/OpenPitFactory.h"
//#include "mining/mine/UnderGroundFactory.h"

//-----------------------------------------------------------------------------
AbstractFactory::AbstractFactory()
{
}

//-----------------------------------------------------------------------------
AbstractFactory::~AbstractFactory()
{
}

//-----------------------------------------------------------------------------
AbstractFactory * AbstractFactory::getFactory()
{
/*#ifdef MINE_UG
	return new UnderGroundFactory();
#else*/
	return  new OpenPitFactory();
//#endif
}




