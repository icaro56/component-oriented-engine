///////////////////////////////////////////////////////////
//  ObjectFileSystem.h
//  Implementation of the Class ObjectFileSystem
//  Created on:      23-jun-2012 11:41:15
//  Original author: Leandro da Silva Santos
///////////////////////////////////////////////////////////

#if !defined(OBJECT_FILE_H)
#define OBJECT_FILE_H
 
class ObjectFileSystem
{

public:		

	//! Destrutor
	virtual ~ObjectFileSystem();	

	/*!
     * \brief  retorna o tipo do objeto
     * \return 
     */
	unsigned int getType() const;

protected:	

	//! Construtor
	ObjectFileSystem(unsigned int type = 0);
	
	//! Construtor de copia
    ObjectFileSystem(const ObjectFileSystem&);

	//! operator de atribuição
    ObjectFileSystem& operator = (const ObjectFileSystem&);

private:

	unsigned int type;
};
#endif // !defined(OBJECT_FILE_H)
