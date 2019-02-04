///////////////////////////////////////////////////////////
//  Compass.h
//  Implementation of the Class Compass
///////////////////////////////////////////////////////////

#if !defined(COMPASS_H)
#define COMPASS_H

#include <QObject>
#include "view/UiRepresentation.h"
#include <OgreOverlayElement.h>

namespace Ogre
{
    class Overlay;
    class OverlayElement;
    class TextAreaOverlayElement;
}

class QMenu;


class Compass : public QObject, public UiRepresentation
{
    Q_OBJECT

public:

    //! Construtor
    Compass();

    //! Destrutor
    ~Compass();

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

	void calculateOffsetDegree();
	void saveNewNorth(const Ogre::Vector2 &dir);

public slots:

	/*!
     * \brief 
	 * \param 
     */
	void setVisible(bool);

signals:

    /*!
     * \brief 
	 * \param 
     */
    void visibilityChanged(bool visible);

private:

	Ogre::Overlay *overlay;
    Ogre::OverlayElement* compassPanel;
    Ogre::OverlayElement* compassPanelReflex;
    Ogre::TextAreaOverlayElement* compassText;

	QMenu * optionMenu;
	QAction * actVisible;

	Ogre::DisplayString degreeSign;
	int oldDegree;

	Ogre::Vector2 north;
    Ogre::Vector2 newNorth;
    float offsetDegree;
};

#endif // COMPASS_H
