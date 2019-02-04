///////////////////////////////////////////////////////////
//  ConfigTerrainFtp.h
//  Implementation of the Class ConfigTerrainFtp
///////////////////////////////////////////////////////////

#if !defined(CONFIG_TERRAIN_FTP_H)
#define CONFIG_TERRAIN_FTP_H

#include <QDialog>

class QEvent;
class QDateTime;

namespace Ui
{
	class ConfigTerrainFtp;
};

class ConfigTerrainFtp : public QDialog
{
    Q_OBJECT

public:

    //! Construtor
    ConfigTerrainFtp(QWidget * parent = 0);

    //! Destrutor
    ~ConfigTerrainFtp();

	/*!
     * \brief 
	 * \param 
     */
	void init();

protected:

    /*!
     * \brief 
	 * \param 
     */
    bool event(QEvent * e);

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
	void addPathFtp(int,const QString&);

	/*!
     * \brief 
	 * \param 
     */
	void removePathFtp(int,const QString&);

private:

	Ui::ConfigTerrainFtp * window;

    //static bool visible;
    QPoint actualPos;

	//tamanho da janela
	QSize oldSize;
};

#endif // CONFIG_TERRAIN_FTP_H
