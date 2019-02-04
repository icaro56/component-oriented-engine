///////////////////////////////////////////////////////////
//  GizmoControl.h
//  Implementation of the Class GizmoControl
///////////////////////////////////////////////////////////

#if !defined(GIZMO_CONTROL_H)
#define GIZMO_CONTROL_H

#include <QObject>

class QMenu;
class QAction;
class QToolBar;
class Gizmo;
class Gizmo3d;

class GizmoControl : public QObject
{
	Q_OBJECT

public:

	/*!
     * \brief 
	 * \param 
     */
	static GizmoControl * getInstance();

	/*!
     * \brief 
	 * \param 
     */
	void destroy();

	/*!
     * \brief 
	 * \param 
     */
	void setPosition(const double& x, const double& y, const double& z);


	/*!
     * \brief 
	 * \param 
     */
	void setPosition(int x, int y);

public slots:
	void calculateScale();

private:

	//! Construtor
	GizmoControl();

	//! Destrutor
	~GizmoControl();

	//! Construtor de copia
    GizmoControl(const GizmoControl&);

	//! operator de atribuição
    GizmoControl& operator = (const GizmoControl&);

	/*!
     * \brief 
     */
	void connects();

private:

	static GizmoControl* instance;

	Gizmo3d* viewMain;
	Gizmo* model;
};

#endif //GIZMO_CONTROL_H