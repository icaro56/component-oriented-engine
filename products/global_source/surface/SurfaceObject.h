///////////////////////////////////////////////////////////
//  SurfaceObject.h
//  Implementation of the Class SurfaceObject
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_OBJECT_H)
#define SURFACE_OBJECT_H

#include <iostream>

class SurfaceObject
{

public:

	//! destrutor
	virtual ~SurfaceObject();		

	/*!
     * \brief retornar o identificador
	 * \return unsigned int
     */
	unsigned int getId() const;	

	/*!
     * \brief retornar o tipo
	 * \return unsigned int
     */
	unsigned int getType() const;		
	
protected:

	//! Construtor 
    SurfaceObject();

	//! Construtor de copia
    SurfaceObject(const SurfaceObject&);

	//! operator de atribuição
    SurfaceObject& operator = (const SurfaceObject&);

	/*!
     * \brief modifica o identificador
	 * \return 
     */
	void setId(unsigned int i);

	/*!
     * \brief modifica o tipo
	 * \return 
     */
	void setType(unsigned int i);

private:

	static unsigned int count;

private:

	unsigned int id;
	unsigned int type;

};

#endif // !defined(SURFACE_OBJECT_H)