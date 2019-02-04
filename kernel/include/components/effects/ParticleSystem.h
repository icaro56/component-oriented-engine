#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "KernelPrerequisites.h"

#include "../Component.h"

#include "OgreColourValue.h"
#include "OgreVector3.h"

#include "math/Color.h"

#include <map>

namespace Ogre
{
	class ParticleSystem;
    class ParticleEmitter;
}

class KERNEL_VS_EXPORT ParticleSystem : public Component 
{
public:
	enum ParticleSystemDesc
	{
		PSD_Scale,
		PSD_Enabled,
		PSD_EnableEmission,
		PSD_Angle,
		PSD_StartColor,
		PSD_MinColor,
		PSD_MaxColor,
		PSD_Duration,
		PSD_DurationMax,
		PSD_EmissionRate,
		PSD_StartLifeTime,
		PSD_EndLifeTime,
		PSD_Material,
		PSD_CastShadows,
		PSD_SpeedFactor,
		PSD_Width,
		PSD_Height,
		PSD_QueryFlags,

		PSD_Size
	};

	ParticleSystem();
	ParticleSystem(const ComponentDescMap& d);
	virtual ~ParticleSystem();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	/*!
        \brief configura a visibilidade da particula
    */
    virtual void setEnabled(bool enable);
	virtual bool isEnabled();

    /*!
        \brief ativa o emissor ou n�o
    */
    void setEnableEmission(const bool enable);
    
	/**
      * @brief Configura o m�ximo �ngulo fora que a part�cula possa
      tomar na dire��o.
      * @param angle �ngulo em graus do m�ximo �ngulo fora
      * @remarks 0 = sempre na mesma dire��o
      *          180 = part�cula pode sair para qualquer lado
      */
    void setAngle(double angle);
    
	/**
      * @brief Configura a cor da part�cula
      * @param r = quantidade de vermelho
      *        g = quantidade de verde
      *        b = quantidade de azul
      *        ou
      *        Vetor3 contento as tr�s cores
      * @see setColour(const Vector3* colour);
      * @see setColour(double rS,double gS, double bS, double rE,
                       double gE, double bE);
      * @see setColour(const Vector3* colourStart,
                       const Vector3* colourEnd);
      */
    void setStartColor(double r,double g, double b);
    void setStartColor(const Color& colour);
    
	/**
      * @brief Configura a cor inicial e final da part�cula,
      onde haver� uma interpola��o de cor.
      * @param rS = quantidade de vermelho inicial
      * @param gS = quantidade de verde inicial
      * @param bS = quantidade de azul inicial
      * @param rE = quantidade de vermelho final
      * @param gE = quantidade de verde final
      * @param bE = quantidade de azul final
      *        ou
      *        Vetor3 contendo as tr�s cores iniciais e outro Vetor3
      cotendo as tr�s cores finais
      */
    void setRangeColor(double rS,double gS, double bS, double rE, double gE, double bE);
    void setRangeColor(const Color& colourStart, const Color& colourEnd);
    /**
      * @brief Configura a cor final que a particula pode se tornar.
      * @param r = quantidade de vermelho
      * @param g = quantidade de verde
      * @param b = quantidade de azul
      *        ou
      *        Vetor3 contento as tr�s cores
      * @see setColourRangeEnd(const Vector3* colourEnd);
      */
    void setMaxColor(double r,double g, double b);
    void setMaxColor(const Color& colourEnd);
    
	/**
      * @brief Configura a cor inicial que a particula pode se tornar.
      * @param r = quantidade de vermelho
      * @param g = quantidade de verde
      * @param b = quantidade de azul
      *        ou
      *  Vetor3 contento as tr�s cores
      * @see setColourRangeStart(const Vector3* colourStart);
      */
    void setMinColor(double r,double g, double b);
    void setMinColor(const Color& colourStart);

    /**
      * @brief Configura a dura��o que ser� emitido as part�culas
      * @param duration dura��o em segundos do tempo que emitir�
      as particulas
      * @see setDuration(double min, double max);
      */
    void setDuration(double duration);
    void setDuration(double min, double max);
    void setDurationMax(double max);

    /**
     * @brief Configura a quantidade de part�cular que ser� emitido
     por segundo
     * @param rate quantidade de part�culas emitidas por segundo
     * @see getEmissionRate();
     */
    void setEmissionRate(double rate);

    /**
     * @brief Retorna a quantidade de part�cular que ser� emitido
     por segundo
     * @return quatidade de part�culas emitidas por segundo
     * @see setEmissionRate(double rate);
     */
    double getEmissionRate();

    /**
      * @brief Configura o tempo de vida da part�cular que ser�
      emitido por segundo
      * @param Ttl tempo em segundos do tempo de vida
      * @param minTtl e maxTtl tempos em segundos onde o tempo de
      vida v�ria do min ao max
      * @see setTimeToLive(double minTtl, double maxTtl);
      */
    void setTimeToLive(double Ttl);
    void setTimeToLive(double minTtl, double maxTtl);
    void setMaxTimeToLive(double maxTtl);

    /**
     * @brief Configura um material para a part�cula
     * @param material nome de um material
     * @see getMaterialName();
     */
    void setMaterialName(const std::string &material);

    /**
     * @brief Retorna o nome do material do sistema de part�culas
     * @return const char* nome do material do sistema de part�culas
     * @see setMaterialName(std::string *material);
     */
    std::string getMaterialName();

    /**
     * @brief Configura se part�culas far�o sombra
     * @param shadows true ou false para sombras
     * @see getCastShadows();
     */
    void setCastShadows(bool shadows);

    /**
    * @brief Retorna se part�culas far�o sombra
    * @return shadows booleana
    * @see setCastShadows(bool shadows);
    */
    bool getCastShadows();

    /**
     * @brief Configura o fator velocidade das part�culas
     * @param double valor da velocidade
     * @see getSpeedFactor();
     */
    void setSpeedFactor(double speed);


    /**
     * @brief Retorna o fator velocidade das part�culas
     * @return double valor da velocidade
     * @see setSpeedFactor(double speed);
     */
    double getSpeedFactor();

    /**
     * @brief Limpa a configura��o da part�cula
     */
    void clear();

	/**
     * @brief Configura dimens�es da part�cula
     */
	void setDimensions(float width, float height);
	void setWidth(float width);
	void setHeight(float height);

	void setScale(float scale);

	void setLayer(unsigned flags);
	unsigned getLayer();

private:
	void applyDescription(ComponentDescMap& mComponentDescMap);
	void applyDesc(int key, const std::string& value);

	void scaleBy( Ogre::Real factor );

	class emitter_params_t
	{
	public:
		Ogre::Vector3 pos;
		Ogre::Real ParticleVelocity;
		Ogre::Real MinParticleVelocity;
		Ogre::Real MaxParticleVelocity;

		void operator*=( Ogre::Real factor )
		{
			pos *= factor;
			ParticleVelocity *= factor;
			MinParticleVelocity *= factor;
			MaxParticleVelocity *= factor;
		}
	};

private:
	Ogre::ParticleSystem* mParticleSystem;
    Ogre::ParticleEmitter* mParticleEmitter;

	ComponentDescMap mParticleSystemDescMap;

	std::string templateName;
};


#endif