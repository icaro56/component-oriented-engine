#ifndef SKYXMANAGER_H
#define SKYXMANAGER_H

#include "KernelPrerequisites.h"
#include <SkyX.h>
#include <OgreVector3.h>
#include <QTime>

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

class RenderView;
class Light;
class Camera;
class GameObject;
class Scene;

/*! 
	\struct SkyXSettings
    \brief Estruturas usada para criar temmplates com as informa��es mais usadas 
	no SkyX

    \author �caro - Visual Virtual
    \version 1.0
    \date 26/04/2012
*/
struct SkyXSettings
{
	/*!
		\brief Construtor padr�o
	*/
	SkyXSettings(){};

	/*!
		\brief Construtor com Skydome + nuvens volum�tricas + raios
		\warning Olhar explica��o dos par�metros nas vari�ves p�blicas
	 */
	SkyXSettings(const Vector3 t, 
				 const Real& tm, 
				 const Real& mp, 
				 const SkyX::AtmosphereManager::Options& atmOpt,
				 const bool& lc, 
				 const bool& vc, 
				 const Real& vcws, 
				 const bool& vcauto, 
				 const Radian& vcwd, 
				 const Vector3& vcac, 
				 const Vector4& vclr,  
				 const Vector4& vcaf, 
				 const Vector2& vcw,
				 const bool& vcl, 
				 const Real& vclat, 
				 const Vector3& vclc, 
				 const Real& vcltm);

	/*! 
		\brief Construtor Skydome + nuvens volum�tricas
	*/
	SkyXSettings(const Vector3 t, 
				 const Real& tm, 
				 const Real& mp, 
				 const SkyX::AtmosphereManager::Options& atmOpt,
				 const bool& lc, 
				 const bool& vc, 
				 const Real& vcws, 
				 const bool& vcauto, 
				 const Radian& vcwd, 
				 const Vector3& vcac, 
				 const Vector4& vclr,  
				 const Vector4& vcaf, 
				 const Vector2& vcw);

	/** 
		\brief Construtor Skydome
	*/
	SkyXSettings(const Vector3 t, 
				 const Real& tm, 
				 const Real& mp, 
				 const SkyX::AtmosphereManager::Options& atmOpt, 
				 const bool& lc);

	/*! \brief Tempo: [0, 24] x = tempo atual, y = hora do nascer do sol, z = hora do por do sol*/
	Vector3 time;

	/*! \brief multiplicador do tempo*/
	Real timeMultiplier;

	/*! \brief fase da lua na faixa [-1,1], onde -1 � totalmente coberta, 0 limpa e 1 totalmente coberta*/
	Real moonPhase;

	/*! \brief op��es da atmosfera, olhar construtor de Options*/
	SkyX::AtmosphereManager::Options atmosphereOpt;

	/*! \brief se h� camadas de nuvens ou n�o*/
	bool layeredClouds;

	/*! \brief se h� nuvem volum�trica ou n�o*/
	bool volumetricClouds;

	/*! \brief velocidade do vento nas nuvens volum�tricas*/
	Real vcWindSpeed;

	/*! \brief se atualizar� nuvens volum�tricas automaticamente ou n�o.*/
	bool vcAutoupdate;

	/*! \brief dire��o do vendo nas nuvens volum�tricas*/
	Radian vcWindDir;

	/*! \brief cor ambiente das nuvens volum�tricas*/
	Vector3 vcAmbientColor;

	/*! \brief resposta � luz das nuvens volum�tricas: x - For�a da luz solar  y - Multiplicador beta do sol z - multiplicador da cor ambiente  w - atenua��o pela dist�ncia*/
	Vector4 vcLightResponse;

	/*! \brief fatores do ambiente para nuvens volum�tricas x - constant, y - linear, z - cuadratic, w - cubic*/
	Vector4 vcAmbientFactors;

	/*! \brief par�metros do tempo para nuvens volum�tricas: x = humidade, porcentagem das nuvens na faixa [0, 1], y = m�dia do tamanho das nuvens*/
	Vector2 vcWheater;

	/*! \brief ativa e desativa sistema de raios*/
	bool vcLightnings;

	/// Configura a m�dia do tempo de apari��o dos raios*/
	Real vcLightningsAT;

	/*! \brief Cor dos raios*/
	Vector3 vcLightningsColor;

	/*! \brief multiplicador do tempo dos raios*/
	Real vcLightningsTM;
};



/*! 
	\class SkyXManager
    \brief Classe respons�vel pelo gerenciamento do plugin do c�u SkyX

    \author �caro - Visual Virtual
    \version 1.0
    \date 26/04/2012

*/
class KERNEL_VS_EXPORT SkyXManager
{
public:

	static SkyXManager* getInstance();

	/*!
		\brief m�todo respons�vel por iniciar a classe singleton
		\param renderView RenderView* render view principal da aplica��o
		\param mRenderingCamera Ogre::Camera* camera principal da aplica��o
		\param east Ogre::Vector2& dire��o leste da aplica��o
		\param t const QTime& tempo corrente da aplica��o
	*/
	void initSky(RenderView *renderView, Camera *mRenderingCamera, Vector2 &east, const QTime& t, Scene* scene);

	/*!
		\brief destroi o objeto singleton da classe
	*/
	void destroy();


	/*!
		\brief Configura o SkyX de acordo com o template passado
		\param nPreset Ogre::uint32 n�mero do template
		\param mRenderingCamera Ogre::Camera* camera principal da aplica��o
		\param east Ogre::Vector2& dire��o leste da aplica��o
		\param t const QTime& tempo corrente da aplica��o
	*/
	void setPreset(unsigned nPreset, Camera *mRenderingCamera, Vector2 &east, const QTime& t);

	/*!
		\brief Retorna o numero do template atual
		\return Ogre::uint32 numero do template
	*/
	unsigned getPreset();

	/*!
		\brief Fun��o que retorna o SkyX
		\return SkyX::SkyX*
	*/
	SkyX::SkyX* getSkyX();

	/*!
		\brief M�todo respons�vel por configurar o tempo atual
		\param hour float hora atual
		\warning A hora pode ser 1.5 hora, ou seja, 1 hora e 30 minutos
	*/
	void setTime(float hour);

	/*!
		\brief Atualiza a dire��o da luz de acordo com a luz do sol e da lua.
		\warning quando usa a luz da Lua, � porque n�o h� mais sol. Diminui um pouco
		a intensidade da luz
		\param light Ogre::light luz da aplica��o
	*/
	void updateLightDir(GameObject* lightObject);

	/*!
		\brief Configura o leste. Ou seja, aonde o Sol vai nascer
		\param dir Ogre::Vector2& dire��o do leste
	*/
	void setEastDirection(Vector2 &dir);

	/*!
		\brief M�todo respons�vel por atualizar a luz e o tempo
	*/
	void updateAll(GameObject* lightObject, const QTime& t);

	void setActive(bool act);
	bool isActive();
	void forceUpdate();

	/*!
     * \brief 
	 * \return 
     */
	void updateSkyXTime(GameObject* lightObject, const QTime &t);

	/*!
     * \brief 
	 * \return 
     */
	void updateSkyXCurrentTime(GameObject* lightObject);

private:
	/*!
		\brief Construtor padr�o
	*/
	SkyXManager();

	/*!
		\brief Destrutor padr�o
	*/
	~SkyXManager();

	SkyX::SkyX * mSkyX;
	SkyX::BasicController* mBasicController;
	unsigned mCurrentPreset;


	/** 
		\briefs os presets s�o criados manualmente, mas podem ser criados com o
		paradise editor (comercial) ou futuramente do ogitor editor (free)
	*/
	SkyXSettings mPresets[6];

	static SkyXManager *instance;

	bool active;

	Scene* parentScene;
};

#endif