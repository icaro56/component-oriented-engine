///////////////////////////////////////////////////////////
//  UITerrainControl.h
//  Implementation of the Class UITerrainControl
///////////////////////////////////////////////////////////

#if !defined(UITerrain_CONTROL_H)
#define UITerrain_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"

class QMenu;
class QAction;
class QToolBar;
class UITerrain;

class UITerrainControl: public QObject, public UiBasic
{
	
	Q_OBJECT

public:

	/*!
     * \brief 
	 * \param 
     */
	static UITerrainControl * getInstance();

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

private slots:
	/*!
     * \brief 
	 * \param 
     */
	void setVisible(bool);

	/*!
     * \brief 
	 * \param 
     */
	void positionGizmo(int,int);

private:

	//! Construtor
	UITerrainControl();

	//! Destrutor
	~UITerrainControl();

	//! Construtor de copia
    UITerrainControl(const UITerrainControl&);

	//! operator de atribuição
    UITerrainControl& operator = (const UITerrainControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static UITerrainControl* instance;

	QAction * act;

	UITerrain* view;

};

#endif //UITerrain_CONTROL_H