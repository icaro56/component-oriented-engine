#ifndef FOLLOWTEXT_H
#define FOLLOWTEXT_H

#include <string>
#include <QMap>
#include "math/Vector3.h"
#include "OgreTextAreaOverlayElement.h"
#include "KernelPrerequisites.h"
#include "../Component.h"
#include <map>

class Camera;
class GameObject;

class KERNEL_VS_EXPORT FollowText : public Component
{
public:

	enum FolowTextDesc
	{
		FTD_Enabled,
		FTD_Text,
		FTD_Height
	};

	/**
      * Construtor da classe
      * @remarks Serve para escrever textos que seguem objetos m�veis.
      * @param node entidade que ser� seguida.
      * @param cam  camera ativa da cena.
      */
    FollowText();
	FollowText(const ComponentDescMap & d);
	virtual ~FollowText(void);

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){};

    /**
      *Destrutor
      *@remarks Serve para remover textos que seguem objetos m�veis.
      */

	void destroy();

    /**
      * Enable ativa e desativa a vizualiza��o do texto
      * @param enabled true ou false, por padr�o o objeto inicia-se false
      */
    virtual void setEnabled(bool enabled);

    /**
      * isEnable retorna se objeto est� ativo ou n�o
      * @return bool  true se estiver ativo, false se estiver escondido
      */
    virtual bool isEnabled();

    /**
     * setText atribui um texto para renderizar
     * @return char*  string com o texto
     */
    void setText(const std::string &text);

    /**
      * update
      * @remarks chame a fun��o no loop principal para atualizar posição
      do texto
      */
	void update();

    void setCamera(Camera* cam);

    /**
      * setColor atribui uma cor para o texto
      * @param R,G,B quantidade de red, green, blue variando de 0 a 1;
      */

    void setColor(const float r, const float g, const float b);

    Vector3 getColor();

	void setTextAlign(Ogre::TextAreaOverlayElement::Alignment a);
    Ogre::TextAreaOverlayElement::Alignment setTextAlign();

    void setHeight(float height);
    float getHeight();

	double getProjectionX();
	double getProjectionY();

	void applyDesc(int key, const std::string& value);

private:

	double projX;
	double projY;

    Camera* mCamera;

    Ogre::Overlay* mOverlay;
    Ogre::OverlayElement* mElementText;
    Ogre::OverlayContainer* mContainer;

    bool mEnable;
    Vector3 mColor;

	ComponentDescMap folowTextDescMap;
};
#endif

