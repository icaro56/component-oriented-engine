///////////////////////////////////////////////////////////
//  UiBasic.h
//  Implementation of the Class UiBasic
//  Created on:      
///////////////////////////////////////////////////////////

#if !defined(UI_REPRESENTATION_H) 
#define UI_REPRESENTATION_H

#include <input/InputManager.h>

enum TypeUiRepresentation
{
	Ui_Representation_MiniMap,
	Ui_Representation_Terrain,
	Ui_Representation_Compass,
	Ui_Representation_StatusMessages,
	Ui_Representation_StatusMessagesEquipment,
	Ui_Representation_EffectiveMining,
	Ui_Representation_GeologicalBlocks
};

/*!
	\class UiRepresentation
	\brief responsável pelas interações com as representações dependentes de Ogre
*/
class UiRepresentation
{

public: 

	//! Destrutor
	virtual ~UiRepresentation();		

	/*!
     * \brief 
	 * \param 
     */
	unsigned int getType() const;

	/*!
     * \brief 
	 * \param 
     */
	virtual void setActiveRepresentation(bool);	

	/*!
     * \brief 
	 * \param 
     */
	virtual bool isActiveRepresentation();

	/*!
     * \brief 
	 * \param 
     */
	virtual bool isVisibledRepresentation() = 0;	

	
	/*!
     * \brief 
	 * \param 
     */
	virtual void setVisibledRepresentation(bool) = 0;

	/*!
     * \brief 
	 * \param 
     */
	void setEditedRepresentation(bool);	
			
	/*!
     * \brief 
	 * \param 
     */
	virtual bool isValidName(const QString& id) = 0;
	
	/*!
     * \brief 
	 * \param 
     */
	virtual void updateRepresentation() = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual void startEditing() = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual void endEditing() = 0;	

	/*!
     * \brief 
	 * \param 
     */
	virtual bool keyPressed(const OIS::KeyEvent &arg) = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual bool keyReleased(const OIS::KeyEvent &arg) = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual bool mouseMoved( const OIS::MouseEvent &arg,
		const QString& = "") = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id
		,const QString& = "") = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
		const QString& = "") = 0;


protected:

	//! Construtor
	UiRepresentation(unsigned int);	

private:

	//! Construtor de copia
    UiRepresentation(const UiRepresentation&);

	//! operator de atribuição
    UiRepresentation& operator = (const UiRepresentation&);

private:

	unsigned int type;
	bool isActive;
};

#endif