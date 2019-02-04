///////////////////////////////////////////////////////////
//  ObjectFileSystem.cpp
//  Implementation of the Class ObjectFileSystem
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////
 
#include "ObjectFileSystem.h"


//-----------------------------------------------------------------------------
ObjectFileSystem::ObjectFileSystem(unsigned int t)
{	
	type = t;
}

//-----------------------------------------------------------------------------
ObjectFileSystem::ObjectFileSystem(const ObjectFileSystem& v)
{
	type = v.type;
}

//-----------------------------------------------------------------------------
ObjectFileSystem::~ObjectFileSystem()
{
}

//-----------------------------------------------------------------------------
ObjectFileSystem& ObjectFileSystem::operator = (const ObjectFileSystem& v)
{
	if (&v == this)
        return *this; 
	type = v.type;
	return *this; 
}

//-----------------------------------------------------------------------------
unsigned int ObjectFileSystem::getType() const
{
	return type;
}
