///////////////////////////////////////////////////////////
//  ConfigTerrain.h
//  Implementation of the Class ConfigTerrain
///////////////////////////////////////////////////////////

#if !defined(CONFIG_TERRAIN_H)
#define CONFIG_TERRAIN_H

#include <QDialog>
#include <QDate>

class QEvent;
class QDateTime;
class QTreeWidgetItem;
class QTableWidgetItem;

namespace Ui
{
	class ConfigTerrain;
};

class ConfigTerrain : public QDialog
{
    Q_OBJECT

public:

    //! Construtor
    ConfigTerrain(QWidget * parent = 0);

    //! Destrutor
    ~ConfigTerrain();

	/*!
     * \brief 
	 * \param 
     */
	void init();

	/*!
     * \brief 
	 * \param 
     */
	void setDataInfo(const QString&,const QHash<int,QStringList >& );

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
    void insertDataTreeDate(const QString&);
	
	/*!
     * \brief 
	 * \param 
     */
    void insertDataCalendar(const QString&);
	
	/*!
     * \brief 
	 * \param 
     */
    void currentTexDataCalendar(const QDate& v);
	
	/*!
     * \brief 
	 * \param 
     */
    void insertDataTableDescription(int i,const QStringList&);

	/*!
     * \brief 
	 * \param 
     */
    void setPageCalendar(const QString&,const QString&);

	/*!
     * \brief 
	 * \param 
     */
	void updateTableDescription(const QDate & date);

	/*!
     * \brief 
	 * \param 
     */
	void setCurrentTerrain(const QString&);

	/*!
     * \brief 
	 * \param 
     */
	void setCurrentTreeDate(const QDate & date);

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
    void showDir(bool);

	/*!
     * \brief 
	 * \param 
     */
    void showFtp(bool);

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
    void nameChanged(int,const QString&);
	
	/*!
     * \brief 
	 * \param 
     */
    void descriptionChanged(int,const QString&);

private slots:

	/*!
     * \brief 
	 * \param 
     */
	void removeTerrain(bool);

	/*!
     * \brief 
	 * \param 
     */
	void exportTerrain(bool);

	/*!
     * \brief 
	 * \param 
     */
	void loadTerrain(bool);

	/*!
     * \brief 
	 * \param 
     */
	void changedTreeDate(QTreeWidgetItem*, QTreeWidgetItem *);

	/*!
     * \brief 
	 * \param 
     */
	void clickedCalendar(const QDate & date);

	/*!
     * \brief 
	 * \param 
     */
	void itemChangedTableDescription(QTableWidgetItem * item);

	/*!
     * \brief 
	 * \param 
     */
	void currentPageChangedCalendar(int year, int month);

	/*!
     * \brief 
	 * \param 
     */
	void itemDoubleClickedTableDescription(QTableWidgetItem*);

private:

	Ui::ConfigTerrain * window;

	QDate lastDate;
	QString idCurrent;
};

#endif // CONFIG_TERRAIN_H
