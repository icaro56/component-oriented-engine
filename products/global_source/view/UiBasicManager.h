///////////////////////////////////////////////////////////
//  UiBasicManager.h
//  Implementation of the Class UiBasic
//  Created on:      
///////////////////////////////////////////////////////////

#if !defined(UI_BASIC_MANAGER_H)
#define UI_BASIC_MANAGER_H

#include <QHash>
#include <QList>

class UiBasic;
class QMenu;

class UiBasicManager
{

public:

	/*!
     * \brief 
	 * \param 
     */
	static UiBasicManager* getInstance();

	/*!
     * \brief 
	 * \param 
     */
	void destroy();	

	/*!
     * \brief 
	 * \param 
     */
	void init();	

	/*!
     * \brief 
	 * \param 
     */
	void setEnableUi(unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	void setCheckedUi(unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	void setVisibleUi(unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	void setFunctionalityEnabledUi(unsigned int, unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	void setFunctionalityCheckedUi(unsigned int, unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	void setFunctionalityVisibleUi(unsigned int, unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	QList<int> getUsedTypesUiBasics();

	/*!
     * \brief Desativa todas as ferramentas
	 * \param 
     */
	void disableAllTools();

private:

	//! Destrutor
	~UiBasicManager();

	//! Construtor
	UiBasicManager();
	
	//! Construtor de copia
    UiBasicManager(const UiBasicManager&);

	//! operator de atribuição
    UiBasicManager& operator = (const UiBasicManager&);

	/*!
     * \brief 
	 * \param 
     */
	void addUi(UiBasic*);

	/*!
     * \brief 
	 * \param 
     */
	void removeUi(UiBasic*);

private:

	static UiBasicManager * instance;
	QHash<int,UiBasic*> listUi;

	friend UiBasic;
};

#endif