///////////////////////////////////////////////////////////
//  UiBasicManager.h
//  Implementation of the Class UiBasic
//  Created on:      
///////////////////////////////////////////////////////////

#if !defined(UI_REPRESENTATION_MANAGER_H)
#define UI_REPRESENTATION_MANAGER_H

#include <input/MouseEvents.h>
#include <input/KeyEvents.h>

#include <QHash>

class UiRepresentation;

class UiRepresentationManager: public KeyEvents, public MouseEvents
{

public:

	/*!
     * \brief 
	 * \param 
     */
	static UiRepresentationManager* getInstance();

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
	void updateRepresentation();	

	/*!
     * \brief 
	 * \param 
     */
	bool keyPressed(const OIS::KeyEvent &arg);

	/*!
     * \brief 
	 * \param 
     */
	bool keyReleased(const OIS::KeyEvent &arg);

	/*!
     * \brief 
	 * \param 
     */
	bool mouseMoved( const OIS::MouseEvent &arg );

	/*!
     * \brief 
	 * \param 
     */
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	/*!
     * \brief 
	 * \param 
     */
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	/*!
     * \brief 
	 * \param 
     */
	void setEdition(UiRepresentation* rep,bool);

private:

	//! Destrutor
	~UiRepresentationManager();

	//! Construtor
	UiRepresentationManager();
	
	//! Construtor de copia
    UiRepresentationManager(const UiRepresentationManager&);

	//! operator de atribuição
    UiRepresentationManager& operator = (const UiRepresentationManager&);

	/*!
     * \brief 
	 * \param 
     */
	void addUi(UiRepresentation*);

	/*!
     * \brief 
	 * \param 
     */
	void removeUi(UiRepresentation*);	

private:

	static UiRepresentationManager * instance;
	QHash<int,UiRepresentation*> listUiRep;

	UiRepresentation * repEdition;

	friend UiRepresentation;
};

#endif