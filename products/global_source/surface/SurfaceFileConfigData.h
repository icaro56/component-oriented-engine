///////////////////////////////////////////////////////////
//  SurfaceFileConfigData.h
//  Implementation of the Class SurfaceFileConfigData
//  Created on:      23-jun-2012 11:41:15
///////////////////////////////////////////////////////////

#if !defined(SURFACE_FILE_CONFIG_DATA_H)
#define SURFACE_FILE_CONFIG_DATA_H

#include <QList>
#include <QString>

#include "persistence\filesystem\DataFileSystem.h"

class SurfaceData;

class SurfaceFileConfigData: public DataFileSystem
{

public:	

	//! Construtor
	SurfaceFileConfigData();
	
	//! Construtor de copia
    SurfaceFileConfigData(const SurfaceFileConfigData&);
	
	//! Destrutor
	virtual ~SurfaceFileConfigData();	

	//! operator de atribuição
    SurfaceFileConfigData& operator = (const SurfaceFileConfigData&);	

	/*!
     * \brief retorna o terreno corrente
     * \return nome do terreno corrente
     */
	unsigned int getCurrentSurface() const;

	/*!
     * \brief seta o terreno corrente
     * \param name nome do terreno corrente
     */
	void setCurrentSurface(unsigned int current);

private:

	int current;

};
#endif // !defined(SURFACE_FILE_CONFIG_DATA_H)
