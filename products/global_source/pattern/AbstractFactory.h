///////////////////////////////////////////////////////////
//  AbstractFactory.h
//  Implementation of the Class AbstractFactory
//  Created on:    
///////////////////////////////////////////////////////////
#if !defined(ABSTRACT_FACTORY_H)
#define ABSTRACT_FACTORY_H

class Surface;

class AbstractFactory
{

public:	

	//! Destrutor
	virtual ~AbstractFactory();
	
	/*!
     * \brief 
	 * \param 
     */
	static AbstractFactory * getFactory();

	/*!
     * \brief 
	 * \param 
     */
	virtual Surface * createSurface() = 0;

protected:

	//! Construtor
	AbstractFactory();

private:	

	//! Construtor de copia
    AbstractFactory(const AbstractFactory&);

	//! operator de atribuição
    AbstractFactory& operator = (const AbstractFactory&);

};
#endif // !defined(MINE_H)
