///////////////////////////////////////////////////////////
//  SurfacePathFileConfig.h
//  Implementation of the Class SurfacePathFileConfig
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_FILE_CONFIG_H)
#define SURFACE_FILE_CONFIG_H

#include "persistence\filesystem\ImportFileSystem.h"
#include "persistence\filesystem\ExportFileSystem.h"

class DataFileSystem;

class SurfacePathFileConfig: public ImportFileSystem, public ExportFileSystem
{

public:

	/*!
     * \brief Construtor
     * \param data estrutura de dados
     */
	SurfacePathFileConfig(DataFileSystem * data);
	
	//! Construtor de copia
    SurfacePathFileConfig(const SurfacePathFileConfig&);

	//! Destrutor
	virtual ~SurfacePathFileConfig();

	//! operator de atribuição
    SurfacePathFileConfig& operator = (const SurfacePathFileConfig&);

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