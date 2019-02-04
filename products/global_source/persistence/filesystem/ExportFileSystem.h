///////////////////////////////////////////////////////////
//  ExportFileSystem.h
//  Implementation of the Class ExportFileSystem
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#if !defined(EXPORT_FILE_H)
#define EXPORT_FILE_H

//#include "Geometry.h"

class QString;

class ExportFileSystem
{

public:	
	
	//! Destrutor
	virtual ~ExportFileSystem();

	/*!
     * \brief exporta o data
     */
	virtual void exportData(const QString& file) = 0;

protected:

	//! Construtor
	ExportFileSystem();

	//! Construtor de copia
    ExportFileSystem(const ExportFileSystem&);

	//! operator de atribuição
    ExportFileSystem& operator = (const ExportFileSystem&);

};
#endif // !defined(EXPORT_FILE_H)
