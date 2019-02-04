#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "KernelPrerequisites.h"

#include "../Component.h"
#include <vector>
#include <map>
#include "OgreAxisAlignedBox.h"

#include <QStringList>
/*#include "OgreSceneQuery.h"*/
#include "SelectionBox.h"

#include "math/Vector3.h"
#include "math/Vector2.h"
#include "math/Matrix4.h"
#include "math/Bounds.h"
#include "math/Color.h"
#include "math/Ray.h"

#include "engine/buffers/RaycastHit.h"

namespace Ogre
{
	class Camera;
	class SceneNode;
	class Viewport;
}

enum CullingMask
{
	VF_DEFAULT = 1 << 0,          // padr�o de todos
	VF_RENDER1 = 1 << 1,          // tudo que ser� mostrado apenas no render principal
	VF_MINIMAP = 1 << 2,          // tudo que ser� mostrado apenas no minimap 3d
	VF_CUSTOM1 = 1 << 3,
	VF_CUSTOM2 = 1 << 4,
	VF_CUSTOM3 = 1 << 5,
	VF_CUSTOM4 = 1 << 6,
	VF_CUSTOM5 = 1 << 7,
	VF_ALL = VF_DEFAULT | VF_RENDER1 | VF_MINIMAP | VF_CUSTOM1 | VF_CUSTOM2 | VF_CUSTOM3 | VF_CUSTOM4 | VF_CUSTOM5
};


/*! \class Camera
	\brief A c�mera � um dispositivo atrav�s do qual o jogador v� o mundo.
	\brief Um ponto do espa�o do mundo � definida em coordenadas globais (Transform).
*/

class KERNEL_VS_EXPORT Camera : public Component
{
	friend class SceneCallback;
public:
	enum CameraDesc
	{
		CD_NearDistance,
		CD_FarDistance,
		CD_LodBias,
		CD_Aspect,
		CD_WireFrameActive,
		CD_CullingMask,
		CD_Enabled,
		CD_Size
	};

	typedef std::vector<Camera*>	CameraList;

	Camera();
	Camera(const ComponentDescMap& d);
	virtual ~Camera();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	/*!
		\brief Atribui a visibilidade
	*/
    virtual void setEnabled(bool enable);
	 /*!
		\brief Verifica a visibilidade 
	*/
    virtual bool isEnabled();

	// Sets the view range of the camera
    /**
    * @brief Distancia de alcance da camera
    * @remarks regula a distancia usando os metodos setNearClipDistance()
    * e setNearClipDistance() do Ogre
    * @param nearDist distancia inicial
    * @param farDist alcance da camera
    */
    void setRange(double nearDist, double farDist = 0);
	void setNearClipDistance(double n);
	void setFarClipDistance(double f);
	double getNearClipDistance();

    // seta uma variavel que influenciar� na distancia em que o Lod ser� usado
    /**
    * @brief seta uma variavel que influenciar� na distancia em que o
    Lod ser� usado
    * @remarks quanto maior o valor setado, mais longe da camera ser�
    ativado o Lod
    * @param d vari�vel da distancia da camera para os mesh's.
    */
    void setLodBias(float d);

    // Gets the camera name
    /**
    * @brief Retorna o nome da camera
    * @return const char* nome da camera
    */
    std::string getName();

    // Render objects in wireframe mode
    /**
    * @brief Define o nivel de detalhes requirido por uma camera
    * @remarks caso o parametro seja true � chamado o metodo
    * setPolygonMode() do Ogre com o parametro PM_WIREFRAME caso
    * contrario, � passado o parametro PM_SOLID
    * param enable Boolean (true/PM_WIREFRAME) (false/PM_SOLID)
    */
    void wireframe(bool enable);

    /**
    * @brief Define uma mascara de visibilidade de um porto de vis�o
    * @remarks A mascara de visibilidade � um jeito de excluir objetos
    * da renderiza��o de um viewport, para cada objeto dentro do frustum
    * uma checagem � feita entre as mascaras para marcar a visibilidade
    dos
    * objetos
    * @param mask Inteiro
    */
    void setVisibilityMask(int mask);
	int getVisibilityMask();

	/*!
        \brief Testa se determinada(s) visibility flag do viewport da janela est�(�o) ativa(s)
	*/
	bool isVisibilityFlags(int flags);

    double getAspect();
    void setAspect(double ar);

	void setBackgroundColor(const Color& col);
	Color getBackgroundColor();

	void setOrthographic(bool enable);
	void setOrthoWindow(double w, double h);
	void setOrthoWidth(double v);
	void setOrthoHeight(double v);

	/*!
		\brief M�todo que verifica se um scene node est� sendo visto pela camera
	*/
	bool isOnView(const Bounds& bounds);
	/*!
		\brief Acessa a C�mera da ogre do Component C�mera
	*/
	Ogre::Camera* getOgreCamera(); 

	Ogre::Viewport* getViewport();
	void setViewport(Ogre::Viewport* viewport);
	void setHorizontalFOV(double f);
	void setVerticalFOV(double f);

	void resizeViewport(double width, double height);

	/*!
		\brief Configura query flag dos raios lan�ados pelo mouse pick
	*/
	void setLayer(unsigned flags);

	/*!
		\brief Retorna a query flag dos raios lan�ados pelo mouse pick
	*/
	unsigned getLayer();

	/*!
		\brief Configura query flag dos raios lan�ados pelo mouse pick
	*/
	bool isLayer(unsigned flags);

	/*!
		\brief Retorna um raio da c�mera atrav�s de um ponto do viewport.
		varia de 0 a 1
	*/
	Ray viewportPointToRay(const Vector2 &position);

	/*!
		\brief Retorna um raio da c�mera atrav�s de um ponto da tela
		posi��o em pixel
	*/
	Ray screenPointToRay(const Vector2 &position, bool isTopWindow = false);

	Vector3 worldToViewportPoint(const Vector3& worldPos);
	Vector3 worldToScreenPoint(const Vector3& worldPos, bool isTopWindow = false);

	Vector3 viewportToWorldPoint(const Vector3& viewPos);
	Vector3 viewportToScreenPoint(const Vector3& viewPos, bool isTopWindow = false);

	Vector3 screenToWorldPoint(const Vector3& screenPos, bool isTopWindow = false);
	Vector3 screenToViewportPoint(const Vector3& screenPos, bool isTopWindow = false);


	void startVolPosition(Vector2 s);
	RaycastHitList stopVolPosition(Vector2 s);
	void updateVolPosition(Vector2 _stop);

	/*QStringList getVolSelectionList();*/

	/**
    * @brief Verifica se um Movable tem certa string
    * @param findName nome a ser procurado
    * @param cond true visivel false invisivel
    * @param result primeiro nome encontrado
	* @param dis distance limite
    * @return true se tem a string
    */
    //bool mousePick(const QString &findName,bool cond,QStringList &result,const double& dis = -1, bool isTopWindow = false);

    /**
    * @brief Verifica se um Movable tem certa string
    * @param findName nome a ser procurado
    * @param result primeiro nome encontrado
    * @return true se tem a string
    */
    //bool mousePick(const QString &findName,QString &result, bool isTopWindow = false);
	/**
    * @brief Retorna qual entidade o mouse sobre
    */
    //char *mousePickEntity(bool isTopWindow = false);

	/**
    * @brief Retorna qual entidade o mouse sobre. M�todo usado para inputs diferentes do principal
    */
    //char *mousePickEntity(int mouseX, int mouseY);

	/*!
		\brief M�todo usado para recuperar a posi��o dos raios lan�ados por duas posi��es em Y na metade da janela em X
		\param Ogre::Vector3& posTop Vetor que receber� a primeira posi��o do raio
		\param Ogre::Vector3& posDown Vetor que receber� a segunda posi��o do raio
		\param double dist dist�ncia que ser� percorrida pelo raio
		\param double dY1 primeira posi��o em Y da janela [0, 1]
		\param double dY2 segunda posi��o em Y da janela [0, 1]
	*/
	//void cameraPickPositions(Vector3& posTop, Vector3& posDown, double dist, double dY1, double dY2);

	const Matrix4& getProjectionMatrix() const;
	const Matrix4& getViewMatrix() const;

	static CameraList allCameras;

private:
	void removeCamera(Camera* camera);
	
	virtual void applyDesc(int key, const std::string& value);

	RaycastHitList performSelection(const Vector2& first, const Vector2& second);

	void setOgreCamera(Ogre::Camera* cam);

	//check that target is inside our viewvolume
	inline bool isinfrustum(const Ogre::Vector3& coords)
	{
		return ( ( coords.x >= -1 ) && ( coords.x <= 1 ) && ( coords.y >= -1 ) && ( coords.y <= 1 ) );
	}

	//check that target is in camera view
	inline bool isinfront(const Ogre::Vector3& worldview)
	{
		return ( worldview.z < 0 );
	}
	
private:
	/// Ogre::Camera
    Ogre::Camera *mCamera;
	Ogre::Viewport* mViewport;

	ComponentDescMap mCameraDescMap;

	double horizontalFOV;

	unsigned layer;

	// RaySceneQuery
    //Ogre::RaySceneQuery *mRaySceneQuery;

	//sele��o por �rea
	//Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery;
	Vector2 mStart, mStop;
	SelectionBox* mSelectionBox;
	bool startedSelection;
	//QStringList volSelectionList;
};


#endif