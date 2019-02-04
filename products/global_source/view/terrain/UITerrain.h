///////////////////////////////////////////////////////////
//  UITerrain.h
//  Implementation of the Class UITerrain
///////////////////////////////////////////////////////////

#if !defined(UITerrain_H)
#define UITerrain_H

#include <QObject>
#include "view/UiRepresentation.h"

class QMenu;
class QAction;

class UITerrain: public QObject, public UiRepresentation
{
    Q_OBJECT

public:

    //! Construtor
    UITerrain();

    //! Destrutor
    ~UITerrain();

	/*!
     * \brief 
	 * \param 
     */
	void close();

	/*!
     * \brief 
	 * \param 
     */
	bool isActiveRepresentation();

	/*!
     * \brief 
	 * \param 
     */
	bool isVisibledRepresentation();

	/*!
     * \brief 
	 * \param 
     */
	void setActiveRepresentation(bool);
	
	/*!
     * \brief 
	 * \param 
     */
	void setVisibledRepresentation(bool);	
		
	/*!
     * \brief 
	 * \param 
     */
	bool isValidName(const QString& id);

	/*!
     * \brief 
	 * \param 
     */
	void updateRepresentation();

	/*!
     * \brief 
	 * \param 
     */
	void startEditing();

	/*!
     * \brief 
	 * \param 
     */
	void endEditing();	

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
	bool mouseMoved( const OIS::MouseEvent &arg,const QString& idname);

	/*!
     * \brief 
	 * \param 
     */
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
	const QString& idname);

	/*!
     * \brief 
	 * \param 
     */
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
	const QString& idname);
	
public slots:

	/*!
     * \brief 
	 * \param 
     */
    void setVisible(bool visible);

signals:

    /*!
     * \brief 
	 * \param 
     */
    void visibilityChanged(bool visible);

	/*!
     * \brief 
	 * \param 
     */
	void positionGizmo(int,int);

private:

	QMenu * optionMenu;
	QAction * actVisible;

};

#endif // MINIMAP_H
