///////////////////////////////////////////////////////////
//  DataFileSystem.cpp
//  Implementation of the Class DataFileSystem
//  Created on:      23-jun-2012 11:41:15
///////////////////////////////////////////////////////////
 
#include "DataFileSystem.h"
#include "ObjectFileSystem.h"

//-----------------------------------------------------------------------------
DataFileSystem::DataFileSystem()
{	

}

//-----------------------------------------------------------------------------
DataFileSystem::DataFileSystem(const DataFileSystem& v)
{
	listObejcts = v.listObejcts;
}

//-----------------------------------------------------------------------------
DataFileSystem::~DataFileSystem()
{
	clear();
}

//-----------------------------------------------------------------------------
DataFileSystem& DataFileSystem::operator = (const DataFileSystem& v)
{
	if (&v == this)
        return *this; 
	listObejcts = v.listObejcts;
	return *this; 
}

//-----------------------------------------------------------------------------
QList<ObjectFileSystem*> DataFileSystem::getObjects() const
{
	return listObejcts;
}

//-----------------------------------------------------------------------------
void DataFileSystem::addObject(ObjectFileSystem* v)
{
	listObejcts << v;
}

//-----------------------------------------------------------------------------
void DataFileSystem::clear()
{
	listObejcts.clear();
}

