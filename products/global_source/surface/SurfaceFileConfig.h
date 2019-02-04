///////////////////////////////////////////////////////////
//  SurfaceFileConfig.h
//  Implementation of the Class SurfaceFileConfig
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_FILE_CONFIG_H)
#define SURFACE_FILE_CONFIG_H

#include "persistence\filesystem\ImportFileSystem.h"
#include "persistence\filesystem\ExportFileSystem.h"

class DataFileSystem;

class SurfaceFileConfig: public ImportFileSystem, public ExportFileSystem
{

public:

	/*!
     * \brief Construtor
     * \param data estrutura de dados
     */
	SurfaceFileConfig(DataFileSystem * data);
	
	//! Construtor de copia
    SurfaceFileConfig(const SurfaceFileConfig&);

	//! Destrutor
	virtual ~SurfaceFileConfig();

	//! operator de atribuição
    SurfaceFileConfig& operator = (const SurfaceFileConfig&);

	/*!
     * \brief exporta o data
     */
	void exportData(const QString& file);

private:

	/*!
     * \brief importa o dado
     * \param QString 
     */
	void read(const QString& file);	

private:

	DataFileSystem * data;

};

#endif // !defined(SURFACE_FILE_CONFIG_H)