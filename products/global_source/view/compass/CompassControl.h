///////////////////////////////////////////////////////////
//  CompassControl.h
//  Implementation of the Class CompassControl
///////////////////////////////////////////////////////////

#if !defined(COMPASS_CONTROL_H)
#define COMPASS_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;
class Compass;

class CompassControl: public QObject, public  UiBasic
{

	Q_OBJECT
	
public:

	/*!
     * \brief 
	 * \param 
     */
	static CompassControl * getInstance();

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

public slots:
	void updateCompass();

private:

	//! Construtor
	CompassControl();

	//! Destrutor
	~CompassControl();

	//! Construtor de copia
    CompassControl(const CompassControl&);

	//! operator de atribuição
    CompassControl& operator = (const CompassControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static CompassControl* instance;

	QAction * actVisible;

	Compass* view;
};

#endif //COMPASS_CONTROL_H