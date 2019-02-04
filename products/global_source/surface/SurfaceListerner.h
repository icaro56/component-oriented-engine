///////////////////////////////////////////////////////////
//  SurfaceListerner.h
//  Implementation of the Class SurfaceListerner
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(SURFACE_LISTERNER_H)
#define SURFACE_LISTERNER_H

#include <QHash>

#include "SurfaceEvents.h"

enum class TypeSurfaceEvent
{
	ChangedLoad,
	ChangedListName,
	ChangedPath,
	ChangedListPath,
	ChangedPainter,
	ChangedConfig,
	None
};

class SurfaceListerner
{

public:


	/*!
     * \brief retorna uma instancia da classe
	 * \return SurfaceManager *
     */
	static SurfaceListerner* getInstance();	

	/*!
     * \brief 
     */
	static void destroy();

	/*!
     * \brief 
     */
	static void init();

	/*!
     * \brief adiciona um evento
	 * \return SurfaceEvent *
     */
	void addEvent(SurfaceEvents*);

	/*!
     * \brief remove um evento
	 * \return SurfaceEvent *
     */
	void removeEvent(SurfaceEvents*);

	/*!
     * \brief notifica a realização de um evento
	 * \return SurfaceManager*
     */
	void notify(TypeSurfaceEvent tte);

private:

	//! Construtor 
    SurfaceListerner();

	//! Construtor de copia
    SurfaceListerner(const SurfaceListerner&);

	//! Construtor 
    ~SurfaceListerner();

	//! operator de atribuição
    SurfaceListerner& operator = (const SurfaceListerner&);	

private:

	static SurfaceListerner* instance;

	QHash<unsigned int ,SurfaceEvents*> list;
};

#endif // !defined(SURFACE_LISTERNER_H)