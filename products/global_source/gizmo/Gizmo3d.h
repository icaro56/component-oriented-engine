///////////////////////////////////////////////////////////
//  Gizmo3D.h
//  Implementation of the Class Gizmo3D
///////////////////////////////////////////////////////////

#if !defined(GIZMO3D_H)
#define GIZMO3D_H

#include <QObject>
#include "gizmo/AxisObject.h"

class QEvent;
class QDateTime;
class GameObject;

class Gizmo3d : public QObject
{
    Q_OBJECT

public:

    //! Construtor
    Gizmo3d(QObject *parent = 0);

    //! Destrutor
    ~Gizmo3d();

public slots:

	/*!
     * \brief 
     */
	void setPosition(const double&x, const double& y, const double& z);

	/*!
     * \brief 
     */
	void setScale(const double&x);

private:

	GameObject * ent;
	AxisObject *axisObj;

};

#endif // GIZMO_H
