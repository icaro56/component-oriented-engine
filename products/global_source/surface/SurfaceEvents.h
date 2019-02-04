///////////////////////////////////////////////////////////
//  SurfaceEvents.h
//  Implementation of the Class SurfaceEvents
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_EVENTS_H)
#define SURFACE_EVENTS_H

#include "SurfaceObject.h"


class SurfaceEvents
{

public:		

	//! Destrutor 
    virtual ~SurfaceEvents();	

	/*!
     * \brief mundança no terreno carregado
     */
	virtual void changedLoad();

	/*!
     * \brief mundança na lista de terrenos
     */
	virtual void changedListName();

	/*!
     * \brief mundança nos diretórios de terrenos
     */
	virtual void changedPath();

	/*!
     * \brief mundança na lista de diretórios de terrenos
     */
	virtual void changedListPath();

	/*!
     * \brief mudança na pintura do terreno
     */
	virtual void changedPainter();

	/*!
     * \brief retornar o identificador
	 * \return unsigned int
     */
	unsigned int getId() const;		

protected:

	//! Construtor 
    SurfaceEvents();

	//! Construtor de copia
    SurfaceEvents(const SurfaceEvents&);

	//! operator de atribuição
    SurfaceEvents& operator = (const SurfaceEvents&);

	/*!
     * \brief modifica o identificador
	 * \return 
     */
	void setId(unsigned int i);

private:

	static unsigned int count;

private:

	unsigned int id;

};

#endif // !defined(SURFACE_EVENTS_H)