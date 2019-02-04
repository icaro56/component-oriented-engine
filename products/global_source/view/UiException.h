///////////////////////////////////////////////////////////
//  UiException.h
//  Implementation of the Class UiException
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(UI_EXCEPTION_H)
#define UI_EXCEPTION_H

#include <exception>
#include <QString>

class UiException: public std::exception
{

public:

	//! Construtor
	UiException();

	//! Construtor de copia
    UiException(const UiException&);

	//! Destrutor
	virtual ~UiException();

	//! operator de atribuição
	UiException& operator = (const UiException&);

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

#endif // !defined(UI_EXCEPTION_H)