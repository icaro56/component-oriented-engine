///////////////////////////////////////////////////////////
//  SurfaceException.h
//  Implementation of the Class SurfaceException
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_EXCEPTION_H)
#define SURFACE_EXCEPTION_H

#include <exception>
#include <QString>

class SurfaceException: public std::exception
{

public:

	//! Construtor
	SurfaceException();

	//! Construtor de copia
    SurfaceException(const SurfaceException&);

	//! Destrutor
	virtual ~SurfaceException();

	//! operator de atribuição
	SurfaceException& operator = (const SurfaceException&);

	/*!
     * \brief modifca a messagem de erro
	 * \return QString nome
     */
	void setMessage(const QString& );

	/*!
     * \brief retorna a messagem de erro
	 * \return QString
     */
	QString getMessage() const;

private:

	QString str;
	
};

#endif // !defined(SURFACE_EXCEPTION_H)