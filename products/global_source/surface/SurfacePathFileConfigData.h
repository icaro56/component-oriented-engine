///////////////////////////////////////////////////////////
//  SurfacePathFileConfigData.h
//  Implementation of the Class SurfacePathFileConfigData
//  Created on:      23-jun-2012 11:41:15
///////////////////////////////////////////////////////////

#if !defined(SURFACE_FILE_CONFIG_DATA_H)
#define SURFACE_FILE_CONFIG_DATA_H

#include <QList>
#include <QString>

#include "persistence\filesystem\DataFileSystem.h"

class SurfaceData;

class SurfacePathFileConfigData: public DataFileSystem
{

public:	

	//! Construtor
	SurfacePathFileConfigData();
	
	//! Construtor de copia
    SurfacePathFileConfigData(const SurfacePathFileConfigData&);
	
	//! Destrutor
	virtual ~SurfacePathFileConfigData();	

	//! operator de atribuição
    SurfacePathFileConfigData& operator = (const SurfacePathFileConfigData&);	

};
#endif // !defined(SURFACE_FILE_CONFIG_DATA_H)
