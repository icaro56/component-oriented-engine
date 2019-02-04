#ifndef GIZMO_H
#define GIZMO_H

#include <QObject>
#include "OgreVector3.h"

class Gizmo : public QObject
{
	Q_OBJECT

public:
	Gizmo(QObject *parent = 0);
	~Gizmo();

	/*!
     * \brief 
     */
	void calculeScale();

	Vector3 getPosition();
	double getScale();

public slots:

	void setPosition(const double& x,const double& y,const double& z);

signals:

	void setScale(const double&);

private:
	double x;
	double y;
	double z;
	double scale;
};

#endif // GIZMO_H
