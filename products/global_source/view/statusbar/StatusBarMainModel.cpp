#include "StatusBarMainModel.h"

#include "surface/Surface.h"
#include "surface/SurfaceManager.h"

//-----------------------------------------------------------------------------
StatusBarMainModel::StatusBarMainModel()
{

}

//-----------------------------------------------------------------------------
StatusBarMainModel::~StatusBarMainModel()
{
}

//-----------------------------------------------------------------------------
QStringList StatusBarMainModel::calculateCoordinated(const QString& type,const double& x3d, 
	const double& y3d, 
		const double& z3d)
{
	double x = x3d;
	double y = y3d;
	double z = z3d;
	QStringList r;
	if(type == QObject::tr(" Coordenada Real "))
	{		
		SurfaceManager::getInstance()->convert3dToReal(x,y,z);
		r << QString::number(x,'f',2);
		r << QString::number(y,'f',2);
		r << QString::number(z,'f',2);
	}
	else if(type == QObject::tr(" Coordenada 3d "))
	{
		r << QString::number(x,'f',2);
		r << QString::number(y,'f',2);
		r << QString::number(z,'f',2);
	}
	else
	{
		r << "-";
		r << "-";
		r << "-";
	}

	return r;
}

