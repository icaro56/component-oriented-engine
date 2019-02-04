///////////////////////////////////////////////////////////
//  OpenPitFactory.h
//  Implementation of the Class OpenPitFactory
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#if !defined(OPENPIT_FACTORY_H)
#define OPENPIT_FACTORY_H

#include "pattern/AbstractFactory.h"

class Surface;

class OpenPitFactory: public AbstractFactory
{

public:	

	//! Construtor
	OpenPitFactory();
	
	//! Destrutor
	~OpenPitFactory();
	
	/*!
     * \brief 
	 * \param 
     */
	Surface * createSurface();

private:	

	//! Construtor de copia
    OpenPitFactory(const OpenPitFactory&);

	//! operator de atribuição
    OpenPitFactory& operator = (const OpenPitFactory&);

};
#endif // !defined(OPENPIT_FACTORY_H)
