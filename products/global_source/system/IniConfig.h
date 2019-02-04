///////////////////////////////////////////////////////////
//  IniConfig.h
//  Implementation of the Class IniConfig
//  Created on:     
///////////////////////////////////////////////////////////

#if !defined(INI_CONFIG_H)
#define INI_CONFIG_H

#include "persistence/filesystem/KeyValueFileSystem.h"

class IniConfig: public KeyValueFileSystem
{

public:		
	
	/*!
     * \brief 
	 * \param 
     */
	static IniConfig * getInstance();

	/*!
     * \brief 
	 * \param 
     */
	void init();

	/*!
     * \brief 
	 * \param 
     */
	void destroy();	

	/*!
     * \brief 
	 * \param 
     */
	void read();
	

protected:

	//! Construtor
	IniConfig();

	//! Destrutor
	virtual ~IniConfig();

	//! Construtor de copia
    IniConfig(const IniConfig&);

	//! operator de atribuição
    IniConfig& operator = (const IniConfig&);


private:

	static IniConfig * instance;

};
#endif // !defined(IniConfig_FILE_H)
