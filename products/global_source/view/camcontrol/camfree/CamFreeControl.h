///////////////////////////////////////////////////////////
//  CamFreeControl.h
//  Implementation of the Class CamFreeControl
///////////////////////////////////////////////////////////

#if !defined(CAM_FREE_CONTROL_H)
#define CAM_FREE_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;

class CamFreeControl: public QObject, public  UiBasic
{

	Q_OBJECT
	
public:

	/*!
     * \brief 
	 * \param 
     */
	static CamFreeControl * getInstance();

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
	CamFreeControl();

	//! Destrutor
	~CamFreeControl();

	//! Construtor de copia
    CamFreeControl(const CamFreeControl&);

	//! operator de atribuição
    CamFreeControl& operator = (const CamFreeControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static CamFreeControl* instance;

	QAction * act;

};

#endif //CAM_FREE_CONTROL_H