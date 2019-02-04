///////////////////////////////////////////////////////////
//  SurfacePathManager.h
//  Implementation of the Class SurfaceEvent
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_PATH_MANAGER_H)
#define SURFACE_PATH_MANAGER_H

#include "SurfacePath.h"

#include <QStringList>
#include <QHash>

class SurfaceListerner;

class SurfacePathManager
{

public:

	/*!
     * \brief p
     */
	static SurfacePathManager* getInstance();	

	/*!
     * \brief 
     */
	static void destroy();	

	/*!
     * \brief 
     */
	void importData(unsigned int,const QStringList&);

	/*!
     * \brief 
     */
	void addPath(const SurfacePath&);

	/*!
     * \brief 
     */
	void editPath(const SurfacePath&);

	/*!
     * \brief 
     */
	void removePath(unsigned int);

	/*!
     * \brief 
     */
	void removePaths(const QList<unsigned int>&);

	/*!
     * \brief 
     */
	const SurfacePath* getPath(unsigned int) const;

	/*!
     * \brief 
     */
	QList<const SurfacePath*> getPaths() const;
	
	/*!
     * \brief 
     */
	void setAttributePath(unsigned int,unsigned int,const QString&);

	/*!
     * \brief 
     */
	QString getAttributePath(unsigned int,unsigned int);

	/*!
     * \brief 
     */
	QList<QStringList> filesInfo(unsigned int) const;

private:

	//! Construtor 
    SurfacePathManager();

	//! Construtor de copia
    SurfacePathManager(const SurfacePathManager&);

	//! Destrutor 
    ~SurfacePathManager();	

	//! operator de atribuição
    SurfacePathManager& operator = (const SurfacePathManager&);

	/*!
     * \brief 
     */
	void init();

	/*!
     * \brief
     */
	QString fileConfiguration() const;

	/*!
     * \brief
     */
	void fileConfigurationRead();

	/*!
     * \brief verifica se o arquivo de configuração
     */
	void fileConfigurationSave();

private:

	bool isInit;

	unsigned int nId; 	

	static SurfacePathManager* instance;	

	SurfaceListerner * listerner;

	QHash<unsigned int,SurfacePath> paths;
};

#endif // !defined(SURFACE_PATH_MANAGER_H)