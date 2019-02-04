///////////////////////////////////////////////////////////
//  MiniMapControl.h
//  Implementation of the Class MiniMapControl
///////////////////////////////////////////////////////////

#if !defined(MINIMAP_CONTROL_H)
#define MINIMAP_CONTROL_H

#include <QObject>

#include "view/UiBasic.h"
#include <OgreVector3.h>

class QMenu;
class QAction;
class QToolBar;
class MiniMap;

class MiniMapControl: public QObject, public UiBasic
{
	
	Q_OBJECT

public:

	/*!
     * \brief 
	 * \param 
     */
	static MiniMapControl * getInstance();

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

	float getWidth() const;

private slots:
	/*!
     * \brief 
	 * \param 
     */
    void changedTextureTerrain(int);

	/*!
     * \brief 
	 * \param 
     */
    void changedPosition(const double& x,const double& y);

signals:
	void informPosition3D(Vector3& p);

private:

	//! Construtor
	MiniMapControl();

	//! Destrutor
	~MiniMapControl();

	//! Construtor de copia
    MiniMapControl(const MiniMapControl&);

	//! operator de atribuição
    MiniMapControl& operator = (const MiniMapControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static MiniMapControl* instance;

	QAction * actVisible;

	MiniMap* view;
};

#endif //MINIMAP_CONTROL_H