#ifndef FOLLOWTEXTURE_H
#define FOLLOWTEXTURE_H

#include <string>
#include <QMap>
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreTextAreaOverlayElement.h"
#include "KernelPrerequisites.h"
#include "../Component.h"


class SceneNodeOld;
class Camera;
class GameObject;


class KERNEL_VS_EXPORT FollowTexture : public Component
{
public:

	enum FollowTextureDesc
	{
		FTXD_Enabled
	};

	FollowTexture(const ComponentDescMap& d);
	FollowTexture(void);
	virtual ~FollowTexture(void);

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){};
	
	virtual void applyDesc(int key, const std::string& value);
	
	/**
      *Destrutor
      *@remarks Serve para remover textos que seguem objetos m�veis.
      */
	void destroy();

	/**
		Adiciona um overlayElement ao container
		@param name nome do elemento, deve ser �nico
		@param element ponteiro do overlayElement adicionado
	*/
	void addElement(QString name, Ogre::OverlayElement* element);

	/**
		Cria um elemento de imagem e j� adiciona ao container
		@param name nome do elemento, deve ser �nico
		@param material o material que aparecer� na tela
		@param dimesions dimen��o da imagem
		@param offset incremento a partir da origem do container
	*/
	Ogre::OverlayElement* createElement(QString name, QString material, Ogre::Vector2 dimensions, Ogre::Vector2 offset = Ogre::Vector2::ZERO);

	/**
		Retorna o elemento com o determinado nome
		@param nome do elemento
		@return ponteiro para o OverlayElement, se n�o existir retorna NULL
	*/
	Ogre::OverlayElement* getElement(QString name);
	
	/**
		Retorna o container com o determinado nome
		@param nome do container
		@return ponteiro para o OverlayContainer, se n�o existir retorna NULL
	*/
	Ogre::OverlayContainer* getContainer(QString name);

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
      * update
      * @remarks chame a fun��o no loop principal para atualizar posi��o gr�fico
      */
    void update();

	/**
	  * Atribui uma nova camera para calcula a posi��o
	  * @param cam nova c�mera 
	  */
    void setCamera(Camera* cam);

	/**
	  * Retorna a proje��o em X da posi��o do n�, (-0.5, 0.5)
	  * @return o valor da proje��o em X
	  */
	double getProjectionX();
	
	/**
	  * Retorna a proje��o em Y da posi��o do n�, (0.0, 1.0)
	  * @return o valor da proje��o em Y 
	  */
	double getProjectionY();

private:

	double projX;
	double projY;


    Camera* mCamera;
    Ogre::Overlay* mOverlay;
    Ogre::OverlayElement* mElementText;
    Ogre::OverlayContainer* mContainer;
    Ogre::String mName;

	QMap<QString, Ogre::OverlayElement*> elements;

    bool mEnable;
	ComponentDescMap followTextureDescMap;
};
#endif
