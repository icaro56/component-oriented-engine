///////////////////////////////////////////////////////////
//  ConfigTerrainControl.h
//  Implementation of the Class ConfigTerrainControl
///////////////////////////////////////////////////////////

#if !defined(CONFIG_TERRAIN_CONTROL_H)
#define CONFIG_TERRAIN_CONTROL_H

#include <QObject>
#include <QHash>

#include "view/UiBasic.h"
#include "surface/SurfaceEvents.h"

class QMenu;
class QAction;
class QToolBar;
class ConfigTerrain;
class ConfigTerrainDir;
class ConfigTerrainFtp;
class QDateTime;

class ConfigTerrainControl: public QObject, public  UiBasic, public SurfaceEvents
{
	
	Q_OBJECT

public:

	/*!
     * \brief 
	 * \param 
     */
	static ConfigTerrainControl * getInstance();

	/*!
     * \brief 
	 * \param 
     */
	void destroy();

	/*!
     * \brief 
	 * \param 
     */
	void addActionsInMenu(QMenu*);

	/*!
     * \brief 
	 * \param 
     */
	void addActionsInToolbar(QToolBar*);

	/*!
     * \brief 
	 * \param 
     */
	void setEnabledUi(bool);

	/*!
     * \brief 
	 * \param 
     */
	void setCheckedUi(bool);

	/*!
     * \brief 
	 * \param 
     */
	void setVisibleUi(bool);

	/*!
     * \brief mundança na lista de terrenos
     */
	void changedListName();

	/*!
     * \brief mundança na lista de terrenos carregados
     */
	void changedLoad();

private:

	//! Construtor
	ConfigTerrainControl();

	//! Destrutor
	~ConfigTerrainControl();

	//! Construtor de copia
    ConfigTerrainControl(const ConfigTerrainControl&);

	//! operator de atribuição
    ConfigTerrainControl& operator = (const ConfigTerrainControl&);

	/*!
     * \brief 
     */
	void connects();

	/*!
     * \brief 
     */
	void setDataConfigDir();

private slots:

	/*!
     * \brief 
	 * \param 
     */
	void removeTerrain(const QStringList &);

	/*!
     * \brief 
	 * \param 
     */
	void exportTerrain(const QStringList &);

	/*!
     * \brief 
	 * \param 
     */
	void loadTerrain(const QStringList &);	

	/*!
     * \brief 
	 * \param 
     */
    void nameChangedTerrain(int,const QString&);
	
	/*!
     * \brief 
	 * \param 
     */
    void descriptionChangedTerrain(int,const QString&);

	/*!
     * \brief 
	 * \param 
     */
	void showFtp(bool);

	/*!
     * \brief 
	 * \param 
     */
	void showDir(bool);

	/*!
     * \brief 
	 * \param 
     */
	void visibilityConfigTerrain(bool);
	
	/*!
     * \brief 
	 * \param 
     */
	void addPath(const QStringList&);
	
	/*!
     * \brief 
	 * \param 
     */
	void editPath(const QStringList&);
	
	/*!
     * \brief 
	 * \param 
     */
	void removePath(const QStringList&);
		
	/*!
     * \brief 
	 * \param 
     */
	void currentPathObtain(int);
		
	/*!
     * \brief 
	 * \param 
     */
	void download(int,const QStringList&);

private:

	static ConfigTerrainControl* instance;

	QAction * actVisible;

	ConfigTerrain* configTerrain;
	ConfigTerrainDir* configTerrainDir;
	ConfigTerrainFtp* configTerrainFtp;
};

#endif //CONFIG_TERRAIN_CONTROL_H