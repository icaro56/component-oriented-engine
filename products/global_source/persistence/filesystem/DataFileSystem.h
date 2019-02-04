///////////////////////////////////////////////////////////
//  DataFileSystem.h
//  Implementation of the Class DataFileSystem
//  Created on:      23-jun-2012 11:41:15
///////////////////////////////////////////////////////////

#if !defined(DATA_FILESYSTEM_H)
#define DATA_FILESYSTEM_H

#include <QList>

class ObjectFileSystem;

class DataFileSystem
{

public:	
	
	//! Destrutor
	virtual ~DataFileSystem();	

	/*!
     * \brief  retorna a lista de objetos
     * \return QList<ObjectFileSystem*>
     */
	virtual QList<ObjectFileSystem*> getObjects() const;

	/*!
     * \brief  adicina um objecto na lista
     * \param ObjectFileSystem
     */
	virtual void addObject(ObjectFileSystem*);

	/*!
     * \brief  limpa a estrutura
     * \param ObjectFileSystem
     */
	virtual void clear();

protected:

	//! Construtor
	DataFileSystem();
	
	//! Construtor de copia
    DataFileSystem(const DataFileSystem&);

	//! operator de atribuição
    DataFileSystem& operator = (const DataFileSystem&);

private:

	QList<ObjectFileSystem*> listObejcts;

};
#endif // !defined(DATA_FILE_H)
