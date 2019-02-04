///////////////////////////////////////////////////////////
//  SurfaceFilePath.h
//  Implementation of the Class SurfaceFilePath
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_PATH_FILE_INCLUDED_)
#define SURFACE_PATH_FILE_INCLUDED_

#include "persistence\filesystem\ImportFileSystem.h"
#include "persistence\filesystem\ExportFileSystem.h"

class DataFileSystem;

class SurfacePathFile: public ImportFileSystem, public ExportFileSystem
{

public:

	/*!
     * \brief Construtor
     * \param data estrutura de dados
     */
	SurfacePathFile(DataFileSystem * data);
	
	//! Construtor de copia
    SurfacePathFile(const SurfacePathFile&);

	//! Destrutor
	virtual ~SurfacePathFile();

	//! operator de atribuição
    SurfacePathFile& operator = (const SurfacePathFile&);

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

#endif // !defined(SURFACE_FILE_PATH_INCLUDED_)