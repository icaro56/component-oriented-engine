#include "UiException.h"
#include <QObject>

//-----------------------------------------------------------------------------
UiException::UiException():exception()
{
	str = QObject::tr("Falha na interface");
}

//-----------------------------------------------------------------------------
UiException::UiException(const UiException& v):exception()
{
	str = v.str;
}

//-----------------------------------------------------------------------------
UiException::~UiException()
{
}

//-----------------------------------------------------------------------------
UiException& UiException::operator = (const UiException& v)
{
	if (&v == this)
        return *this; 
	str = v.str;
	return *this;
}

//-----------------------------------------------------------------------------
void UiException::setMessage(const QString& messagem)
{
	str = messagem;
}

//-----------------------------------------------------------------------------
QString UiException::getMessage() const
{
	return str;
}