///////////////////////////////////////////////////////////
//  CamAutomaticControl.h
//  Implementation of the Class CamAutomaticControl
///////////////////////////////////////////////////////////

#if !defined(CAM_AUTOMATIC_CONTROL_H)
#define CAM_AUTOMATIC_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;

class CamAutomaticControl: public QObject, public  UiBasic
{
	
	Q_OBJECT

public:

	/*!
     * \brief 
	 * \param 
     */
	static CamAutomaticControl * getInstance();

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

signals:
	void informWasActived(unsigned int t);

private slots:
	/*!
     * \brief 
	 * \param 
     */
	void camActive(bool);

private:

	//! Construtor
	CamAutomaticControl();

	//! Destrutor
	~CamAutomaticControl();

	//! Construtor de copia
    CamAutomaticControl(const CamAutomaticControl&);

	//! operator de atribuição
    CamAutomaticControl& operator = (const CamAutomaticControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static CamAutomaticControl* instance;

	QAction * act;
};

#endif //CAM_AUTOMATIC_CONTROL_H