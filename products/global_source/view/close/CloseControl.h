///////////////////////////////////////////////////////////
//  CloseControl.h
//  Implementation of the Class CloseControl
///////////////////////////////////////////////////////////

#if !defined(CLOSE_CONTROL_H)
#define CLOSE_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;
class Close;

class CloseControl: public QObject, public  UiBasic
{
	
	Q_OBJECT

public:

	/*!
     * \brief 
	 * \param 
     */
	static CloseControl * getInstance();

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
	bool close();

public slots:
	void closeApplication();

private:

	//! Construtor
	CloseControl();

	//! Destrutor
	~CloseControl();

	//! Construtor de copia
    CloseControl(const CloseControl&);

	//! operator de atribuição
    CloseControl& operator = (const CloseControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static CloseControl* instance;

	QAction * actVisible;

	Close* view;
};

#endif //CLOSE_CONTROL_H