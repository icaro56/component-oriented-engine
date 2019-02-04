///////////////////////////////////////////////////////////
//  CamAvatarControl.h
//  Implementation of the Class CamAvatarControl
///////////////////////////////////////////////////////////

#if !defined(CAM_AVATAR_CONTROL_H)
#define CAM_AVATAR_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;

class CamAvatarControl: public QObject, public  UiBasic
{
	
	Q_OBJECT

public:

	/*!
     * \brief 
	 * \param 
     */
	static CamAvatarControl * getInstance();

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
	CamAvatarControl();

	//! Destrutor
	~CamAvatarControl();

	//! Construtor de copia
    CamAvatarControl(const CamAvatarControl&);

	//! operator de atribuição
    CamAvatarControl& operator = (const CamAvatarControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static CamAvatarControl* instance;

	QAction * act;
};

#endif //CAM_AVATAR_CONTROL_H