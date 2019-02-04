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
      *@remarks Serve para remover textos que seguem objetos móveis.
      */
	void destroy();

	/**
		Adiciona um overlayElement ao container
		@param name nome do elemento, deve ser único
		@param element ponteiro do overlayElement adicionado
	*/
	void addElement(QString name, Ogre::OverlayElement* element);

	/**
		Cria um elemento de imagem e já adiciona ao container
		@param name nome do elemento, deve ser único
		@param material o material que aparecerá na tela
		@param dimesions dimenção da imagem
		@param offset incremento a partir da origem do container
	*/
	Ogre::OverlayElement* createElement(QString name, QString material, Ogre::Vector2 dimensions, Ogre::Vector2 offset = Ogre::Vector2::ZERO);

	/**
		Retorna o elemento com o determinado nome
		@param nome do elemento
		@return ponteiro para o OverlayElement, se não existir retorna NULL
	*/
	Ogre::OverlayElement* getElement(QString name);
	
	/**
		Retorna o container com o determinado nome
		@param nome do container
		@return ponteiro para o OverlayContainer, se não existir retorna NULL
	*/
	Ogre::OverlayContainer* getContainer(QString name);

    /**
      * Enable ativa e desativa a vizualização do texto
      * @param enabled true ou false, por padrão o objeto inicia-se false
      */
    virtual void setEnabled(bool enabled);

    /**
      * isEnable retorna se objeto está ativo ou não
      * @return bool  true se estiver ativo, false se estiver escondido
      */
    virtual bool isEnabled();

    /**
      * update
      * @remarks chame a função no loop principal para atualizar posição gráfico
      */
    void update();

	/**
	  * Atribui uma nova camera para calcula a posição
	  * @param cam nova câmera 
	  */
    void setCamera(Camera* cam);

	/**
	  * Retorna a projeção em X da posição do nó, (-0.5, 0.5)
	  * @return o valor da projeção em X
	  */
	double getProjectionX();
	
	/**
	  * Retorna a projeção em Y da posição do nó, (0.0, 1.0)
	  * @return o valor da projeção em Y 
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
