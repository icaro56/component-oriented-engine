#include "SurfaceException.h"
#include <QObject>

//-----------------------------------------------------------------------------
SurfaceException::SurfaceException():exception()
{
	str = QObject::tr("Falha no terreno");
}

//-----------------------------------------------------------------------------
SurfaceException::SurfaceException(const SurfaceException& v):exception()
{
	str = v.str;
}

//-----------------------------------------------------------------------------
SurfaceException::~SurfaceException()
{
}

//-----------------------------------------------------------------------------
SurfaceException& SurfaceException::operator = (const SurfaceException& v)
{
	if (&v == this)
        return *this; 
	str = v.str;
	return *this;
}

//-----------------------------------------------------------------------------
void SurfaceException::setMessage(const QString& messagem)
{
	str = messagem;
}

//-----------------------------------------------------------------------------
QString SurfaceException::getMessage() const
{
	return str;
}