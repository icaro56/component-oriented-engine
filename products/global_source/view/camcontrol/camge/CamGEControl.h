///////////////////////////////////////////////////////////
//  CamGEControl.h
//  Implementation of the Class CamGEControl
///////////////////////////////////////////////////////////

#if !defined(CAM_GE_CONTROL_H)
#define CAM_GE_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;

class CamGEControl: public QObject, public  UiBasic
{

	Q_OBJECT
	
public:

	/*!
     * \brief 
	 * \param 
     */
	static CamGEControl * getInstance();

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
	CamGEControl();

	//! Destrutor
	~CamGEControl();

	//! Construtor de copia
    CamGEControl(const CamGEControl&);

	//! operator de atribuição
    CamGEControl& operator = (const CamGEControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static CamGEControl* instance;

	QAction * act;
};

#endif //CAM_GE_CONTROL_H