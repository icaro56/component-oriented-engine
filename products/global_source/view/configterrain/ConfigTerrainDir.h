///////////////////////////////////////////////////////////
//  ConfigTerrainDir.h
//  Implementation of the Class ConfigTerrainDir
///////////////////////////////////////////////////////////

#if !defined(CONFIG_TERRAIN_DIR_H)
#define CONFIG_TERRAIN_DIR_H

#include <QDialog>

#include <QHash>

class QEvent;
class QDateTime;
class QTableWidgetItem;
class SpinBoxDelegate;

namespace Ui
{
	class ConfigTerrainDir;
};

class ConfigTerrainDir : public QDialog
{
    Q_OBJECT

public:

    //! Construtor
    ConfigTerrainDir(QWidget * parent = 0);

    //! Destrutor
    ~ConfigTerrainDir();

	/*!
     * \brief 
	 * \param 
     */
	void setPaths(const QList<QStringList> &);

	/*!
     * \brief 
	 * \param 
     */
	void setTypes(const QStringList&);

	/*!
     * \brief 
	 * \param 
     */
	void setInfoObtain(const QList<QStringList >&);

protected:

    /*!
     * \brief 
	 * \param 
     */
    bool event(QEvent * e);

private:

	/*!
     * \brief 
	 * \param 
     */
    void connects();

	/*!
     * \brief 
	 * \param 
     */
	unsigned int idType(const QString&); 

	/*!
     * \brief 
	 * \param 
     */
	void setTableWidgetDirs(int); 

	/*!
     * \brief 
	 * \param 
     */
	void addItems(int); 

public:	

signals:

    /*!
     * \brief 
	 * \param 
     */
    void visibilityChanged(bool visible);

	/*!
     * \brief 
	 * \param 
     */
	void removePath(const QStringList&);

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
	void currentPathObtain(int);

	/*!
     * \brief 
	 * \param 
     */
	void download(int,const QStringList&);

private slots:	

	/*!
     * \brief 
	 * \param 
     */
	void addPath(bool);

	/*!
     * \brief 
	 * \param 
     */
	void removePath(bool);

	/*!
     * \brief 
	 * \param 
     */
	void download(bool);

	/*!
     * \brief 
	 * \param 
     */
	void itemChangedTableDirs(QTableWidgetItem *);

	/*!
     * \brief 
	 * \param 
     */
	void currentIndexChangedComboBoxTypeDir(const QString &);

	/*!
     * \brief 
	 * \param 
     */
	void currentIndexChangedComboBoxTypeDirObtain(const QString &);

	/*!
     * \brief 
	 * \param 
     */
	void currentIndexChangedComboBoxLocalObtain(const QString &);

	/*!
     * \brief 
	 * \param 
     */
	void itemDoubleClickedTableDirs(QTableWidgetItem *);

	/*!
     * \brief 
	 * \param 
     */
	void itemChangedTableDirObtain(QTableWidgetItem *);

private:

	Ui::ConfigTerrainDir * window;

	bool inAddition;

	//-------- Diretório
	QHash<int,QStringList> paths;

	SpinBoxDelegate * spinBoxDelegate;
	SpinBoxDelegate * spinBoxDelegatePort;
};

#endif // CONFIG_TERRAIN_DIR_H
