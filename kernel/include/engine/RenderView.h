#ifndef _RENDERVIEW_H_
#define _RENDERVIEW_H_

#include "OgreRenderTargetListener.h"
#include <QWidget>
#include "KernelPrerequisites.h"

#include "math/Vector2.h"

namespace Ogre
{
	class Overlay;
}

class Camera;

/**
 * @struct RenderStats
 * @brief Estatisticas sobre o render view
 */
struct RenderStats
{
    double  lastFPS;		/// current fps (frames per second)
    double  avgFPS;			/// average fps
    double  bestFPS;		/// best fps
    double  worstFPS;		/// worst fps
    int  bestFrameTime;		/// best frame time
    int  worstFrameTime;	/// worst frame time
    int  triangleCount;		/// triangle count in view
    int  batchCount;		///
};


/**
 * @brief Cria uma render view
 */
class KERNEL_VS_EXPORT RenderView : public QWidget, public Ogre::RenderTargetListener
{
    Q_OBJECT

    /// @typedef OverlayList lista de overlay
    typedef std::vector<Ogre::Overlay*> OverlayList;

public:
    /**
    * @brief Cria uma instância de RenderView
    * @param parent ponteiro para a janela pai
    */
    RenderView(const std::string & name, QWidget* parent = 0);
	void initialise(bool allowPerfHUD = false);

    /**
    * @brief Destrói a instância de render view
    */
    virtual ~RenderView();

    /**
    * @brief Define uma Ogre::RenderWindow
    * @param window ponteiro de uma RenderWindow
    * @see getRenderWindow
    */
    void setRenderWindow(Ogre::RenderWindow *window);

    /**
    * @brief Define uma Ogre::RenderWindow
    * @param window ponteiro de uma RenderWindow
    * @see setRenderWindow
    */
    Ogre::RenderWindow *getRenderWindow();

    /**
    * @brief Define uma Camera
    * @param cam ponteiro para câmera
    */
	void setCamera(Camera* cam, int ZOrder = 0);

    /**
    * @brief Define uma Camera
    * @param w tamanho da janela
    * @param h altura da janela
    */
    void resizeWindow(int w, int h);

    /**
    * @brief Retorna as estatísticas do render view
    */
    RenderStats *getRenderStats();

    /**
    * @brief Retorna as dimensões da janela
    */
    Vector2 getDimensions();

    /**
    * @brief Retorna a posição absoluta do widget mais alto na hierarquia
    * das classes
    */
    Vector2 getAbsolutePosition();

    /**
    * @brief Habilita a visualização de overlays
    * @param enable
    */
    void enableOverlays(bool enable = true);

    /**
    * @brief hide all visible overlays
    * @param evt
    */
    virtual void preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);

    /**
    * @brief show overlays previously hidden
    * @param evt
    */
    virtual void postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt);

    /**
       \brief retorna cameraComponent ativa do render
    */
    Camera* getCamera();

    /**
        \brief seta o nome do renderView
    */
    void setNameId(const std::string & name);

    /**
        \brief retorna o nome do renderView
    */
    const std::string getNameId() const;

    /**
        \brief retorna se mouse está em cima do render
    */
    bool isOnRender() const;

    /**
        \brief retorna se a janela está ativada
    */
    //bool isActived() const;

    /**
    * @brief Evento usado para informar que quem vai renderizar não é o QT
    */
	QPaintEngine* paintEngine() const;

	/*!
        \brief Informa se renderView possui MyGUI
	*/
	void setHaveMyGUI(const bool enable);
	bool getHaveMyGUI();

	void setAutoAspectRatio(bool active);
	bool getAutoAspectRatio();

	Ogre::Viewport* addViewport(Camera* cam, int ZOrder = 0, float left = 0.0f, float top = 0.0f , float width = 1.0f, float height = 1.0f);

signals:
	void resizeEventOcurred();

protected:
    /**
    * @brief Redimensiona a janela
    * @param evt
    */
    virtual void resizeEvent(QResizeEvent* evt);

    /**
    * @brief Evento de receber o foco
    * @param evt
    */
    virtual void enterEvent(QEvent *evt);

    /**
    * @brief Evento de tirar o foco
    * @param evt
    */
    virtual void leaveEvent(QEvent *evt);

    /* *
    * @brief Não utilizado
    */
    //para funcionar precisa estar setado SetFocusPolicy()
    /*void focusInEvent(QFocusEvent *evt);
    void focusOutEvent(QFocusEvent *evt);*/

    /* *
    * @brief Evento que renderiza a janela de Ogre
    */
    virtual void paintEvent(QPaintEvent *evt);

private:
    /// RenderWindow from Ogre
    Ogre::RenderWindow *mOgreWindow;

    /// Render statistics
    RenderStats *mRenderStats;
    ///
    bool mOverlayEnabled;

    ///Variável que informa se o mouse está em cima do render
    bool onRender;

    /// OverlayList
    OverlayList mOvList;

    ///Camera ativa do render
	Camera* mCameraComponent;

    ///Nome do render view. Chave para o map
    QString mName;
    bool haveMyGUI;
	bool mIsInitialised;
	bool autoAspectRatio;
};

#endif

