///////////////////////////////////////////////////////////
//  StatusBarControl.h
//  Implementation of the Class StatusBarControl
///////////////////////////////////////////////////////////

#if !defined(STATUS_BAR_CONTROL_H)
#define STATUS_BAR_CONTROL_H

#include "view/UiBasic.h"
#include <QStringList>
#include <qobject.h>

class QMenu;
class QAction;
class QToolBar;
class QStatusBar;
class StatusBar;

class StatusBarControl: public QObject, public UiBasic
{
	Q_OBJECT
public:

	/*!
     * \brief 
	 * \param 
     */
	static StatusBarControl * getInstance();

	/*!
     * \brief 
	 * \param 
     */
	void destroy();

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
	void addStatusBar(QStatusBar*);	
	
	/*!
     * \brief 
	 * \param 
     */
	void setCameraCoordinated(const double& x,const double& y,const double& z);

	/*!
     * \brief 
	 * \param 
     */
	void setGizmoCoordinated(const double& x,const double& y,const double& z);

private slots:
	void receivingCamInfo();

private:

	//! Construtor
	StatusBarControl();

	//! Destrutor
	~StatusBarControl();

	//! Construtor de copia
    StatusBarControl(const StatusBarControl&);

	//! operator de atribuição
    StatusBarControl& operator = (const StatusBarControl&);

	/*!
     * \brief 
     */
	void connects();

	/*!
     * \brief 
	 * \param 
     */
	QStringList calculateCoordinated(const QString& type,const double& x, const double& y, const double& z);

private:

	static StatusBarControl* instance;

	QAction * act;

	StatusBar* view;

};

#endif //STATUS_BAR_CONTROL_H