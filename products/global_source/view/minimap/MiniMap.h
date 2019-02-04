///////////////////////////////////////////////////////////
//  MiniMap.h
//  Implementation of the Class MiniMap
///////////////////////////////////////////////////////////

#if !defined(MINIMAP_H)
#define MINIMAP_H

#include <QObject>
#include "view/UiRepresentation.h"

class QMenu;

namespace Ogre
{
	class Overlay;
	class OverlayContainer;
	class OverlayElement;
	class TextAreaOverlayElement;
};

enum TextureType
{
	TT_Satellite,
	TT_ShadesGray
};

class MiniMap: public QObject, public UiRepresentation
{
    Q_OBJECT

public:

    //! Construtor
    MiniMap();

    //! Destrutor
    ~MiniMap();

	/*!
     * \brief 
	 * \param 
     */
    void close();

	/*!
     * \brief 
	 * \param 
     */
	double getWidth() const;

	/*!
     * \brief 
	 * \param 
     */
	void setMaterialName(const QString& name);


	/*!
     * \brief 
	 * \param 
     */
	void changedView(const double& x,const double& y);

	/*!
     * \brief 
	 * \param 
     */
	bool isVisibledRepresentation();
	
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
	bool mouseMoved( const OIS::MouseEvent &arg,const QString& = "");

	/*!
     * \brief 
	 * \param 
     */
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
		const QString& = "");

	/*!
     * \brief 
	 * \param 
     */
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id,
		const QString& = "");
	
	/*!
        \brief informa a posição do miniMap no render da Ogre
        \param const float largura da janela
        \param const float altura da janela
        \return Ogre::Vector2 posicao do mapa
    */
    Ogre::Vector2 getRealPosition(const float largura, const float altura);

private:

	/*!
     * \brief 
	 * \return 
     */
	void mousePosition(int mx,int my, double& x, double& y);

	/*!
        \brief configura minimapa com as configurações de fábrica
    */
    void defaut();

	/*!
        \brief informa o nome do equipamento abaixo do minimapa ao posicionar o
        mouse acima do ponto neste. O nome é representado na cor do tipo do
        equipamento.
		\author Douglas Jano - 
    	\date 09/05/2011
    */
    void displayTag(double x, double y);

	/*!
        \brief Esconde o nome do equipamento abaixo do minimapa. Utilizado
        quando o mouse não estiver sobre ponto algum.
		\author Douglas Jano - 
    	\date 09/05/2011
    */
    void hideDisplayTag();

	/*!
        \brief trata eventos de reposicionar e redimensionar o minimapa
    */
    void moveOrResize();

	/*!
        \brief Retorna se o mapa estava movendo
    */
    bool wasMoving();

    /*!
        \brief Retorna se o mapa estava redimensionando
    */
    bool wasResizing();

    /*!
        \brief configura a nova posição depois que foi movido
    */
    void setNewPosition();

    /*!
        \brief configura a nova dimensão depois que foi redimensionado
    */
    void setNewDimension();

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
    void changedTextureTerrain(int);

	/*!
     * \brief 
	 * \param 
     */
    void changedPosition(const double& x,const double& y);


private:

	Ogre::Overlay* map;

	Ogre::OverlayContainer* mapPanel;
	Ogre::OverlayContainer* containerMap;

    Ogre::OverlayElement* elementIcon;
    Ogre::OverlayElement* elementIconColor;
    Ogre::OverlayElement* elementMove;
    Ogre::OverlayElement* elementResize;

    Ogre::TextAreaOverlayElement* elementTag;

	bool onMap;
	bool onResizeOverlay;
	bool moved;
    bool resized;

	QMenu * optionMenu;
	QAction * actVisible;

	float newDim;
	Vector2 newPos;
    Vector2 posDim;
};

#endif // MINIMAP_H
