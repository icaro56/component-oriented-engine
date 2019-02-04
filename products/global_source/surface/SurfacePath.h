///////////////////////////////////////////////////////////
//  SurfacePath.h
//  Implementation of the Class SurfaceEvent
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_PATH_H)
#define SURFACE_PATH_H

#include "persistence\filesystem\ObjectFileSystem.h"

#include <QHash>
#include <QTimer>
#include <QObject>
#include <QThread>
#include <QList>
#include <QStringList>

enum TypeSurfacePath
{
	TSP_Dir = 1,
	TSP_FTP = 2
};

enum TypeSurfacePathAttribute
{
	TSPA_Path,
	TSPA_UserName,
	TSPA_Password,
	TSPA_Port,
	TSPA_Dir,
	TSPA_TimeMonitoring,
	TSPA_LastTime
};

class SurfaceData;

class SurfacePath: public QObject, public ObjectFileSystem
{

	Q_OBJECT

public:
	
	//! Construtor 
    SurfacePath();

	//! Construtor de copia
    SurfacePath(const SurfacePath&);

	//! Destrutor 
    ~SurfacePath();	

	//! operator de atribuição
    SurfacePath& operator = (const SurfacePath&);	

	/*!
     * \brief 
	 * \return 
     */
	unsigned int getId() const;

	/*!
     * \brief 
	 * \return 
     */
	void setId(unsigned int id);

	/*!
     * \brief 
	 * \return 
     */
	unsigned int getType() const;

	/*!
     * \brief 
	 * \return 
     */
	void setType(unsigned int id);	

	/*!
     * \brief 
	 * \return 
     */
	void setAttribute(unsigned int,const QString&);

	/*!
     * \brief 
	 * \return 
     */
	QString getAttribute(unsigned int) const;

	/*!
     * \brief 
	 * \return 
     */
	bool initTimer();

	/*!
     * \brief 
	 * \return 
     */
	QList<QStringList> readInfo() const;	

public:

	/*!
     * \brief 
	 * \return 
     */
	static QString getTypeName(unsigned int);

	/*!
     * \brief 
	 * \return 
     */
	static unsigned int getType(const QString&);

	/*!
     * \brief 
	 * \return 
     */
	static void writeInfo(const QString& path,const SurfaceData* data);

private slots:

	/*!
     * \brief 
	 * \return 
     */
	void scanFiles();

public:

	static const QString FileTerrainFormat;
	static const QString FileTerrainInfoFormat;

private:

	unsigned int id;
	unsigned int type;
	QHash<unsigned int,QString> attributes;

	QTimer timer;
};

#endif // !defined(SURFACE_PATH_H)