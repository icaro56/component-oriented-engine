///////////////////////////////////////////////////////////
//  ConfigTerrainModel.h
//  Implementation of the Class ConfigTerrainModel
///////////////////////////////////////////////////////////

#if !defined(CONFIG_TERRAIN__MODEL_H)
#define CONFIG_TERRAIN__MODEL_H

#include <QObject>
#include <QHash>

class QDateTime;
class QString;

class ConfigTerrainModel : public QObject
{
	Q_OBJECT

public:

	 //! Construtor
    ConfigTerrainModel();

    //! Destrutor
    ~ConfigTerrainModel();

public slots:

	/*!
     * \brief 
	 * \param 
     */
	void addTerrain(const QDateTime&, const QHash<int,QString>&, 
		const QHash<int,double>&);

	/*!
     * \brief 
	 * \param 
     */
	void removeTerrain(const QDateTime&, const QDateTime&);

	/*!
     * \brief 
	 * \param 
     */
	void editTerrain(const QHash<int,QString>&);

	/*!
     * \brief 
	 * \param 
     */
	void addPathDir(int,const QString&);

	/*!
     * \brief 
	 * \param 
     */
	void removePathDir(int,const QString&);

	
	/*!
     * \brief 
	 * \param 
     */
	void addPathFtp(int,const QString&);

	/*!
     * \brief 
	 * \param 
     */
	void removePathFtp(int,const QString&);
};


#endif //CONFIG_TERRAIN__MODEL_H