///////////////////////////////////////////////////////////
//  CamOrbitalControl.h
//  Implementation of the Class CamOrbitalControl
///////////////////////////////////////////////////////////

#if !defined(CAM_ORBITAL_CONTROL_H)
#define CAM_ORBITAL_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;

class CamOrbitalControl: public QObject, public  UiBasic
{

	Q_OBJECT
	
public:

	/*!
     * \brief 
	 * \param 
     */
	static CamOrbitalControl * getInstance();

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
	CamOrbitalControl();

	//! Destrutor
	~CamOrbitalControl();

	//! Construtor de copia
    CamOrbitalControl(const CamOrbitalControl&);

	//! operator de atribuição
    CamOrbitalControl& operator = (const CamOrbitalControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static CamOrbitalControl* instance;

	QAction * act;
};

#endif //CAM_ORBITAL_CONTROL_H