///////////////////////////////////////////////////////////
//  StatusBar.h
//  Implementation of the Class StatusBar
///////////////////////////////////////////////////////////

#if !defined(STATUS_BAR_H)
#define STATUS_BAR_H

#include <QObject>

class QStatusBar;
class QComboBox;
class QLabel;
class QIcon;
class QProgressBar;
class QFrame;
class QHBoxLayout;

class StatusBar 
{

public:

    //! Construtor
    StatusBar();

    //! Destrutor
    ~StatusBar();

	/*!
     * \brief 
	 * \param 
     */
	void init();

	/*!
     * \brief 
	 * \param 
     */
	QString getTypeCoordinated();

	/*!
     * \brief 
	 * \param 
     */
	void setCameraCoordinated(const QString& x, const QString& y, 
		const QString& z);

	/*!
     * \brief 
	 * \param 
     */
	void setGizmoCoordinated(QString x, QString y, 
		QString z);


	/*!
     * \brief 
	 * \param 
     */
	void setupUi(QStatusBar *);

private:

	QStatusBar * statusBar;

	QComboBox * comboBox; 

	QLabel *lblDefault, *lblPosCamera, *lblPosGizmo, *lblTotalEquip;
    QLabel *lbltenanceIco, *lbltenanceTxt;
    QLabel *lblNormalIco, *lblNormalTxt;
    QLabel *lblSimulIco, *lblSimulTxt;
    QLabel *lblFailIco, *lblFailTxt; 
	QLabel *lblUnknowIco, *lblUnknowTxt;
    QLabel *lblChangeTerrain;

	QIcon *iconSimulated, *iconActive;
    QIcon *iconFail, *icontenance;
    QIcon *iconUnknow;
    QIcon *iconOFF, *iconON;

    QProgressBar *progress;

    QFrame *frmtenanceIcons, *frmNormalIcons;
    QFrame *frmSimulIcons, *frmFailIcons;
    QFrame *frmUnknowIcons;

    QHBoxLayout *laytenanceIcons, *layNormalIcons;
    QHBoxLayout *laySimulIcons, *layFailIcons;
    QHBoxLayout *layUnknowIcons;
};

#endif // StatusBar_H
