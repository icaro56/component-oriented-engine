#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include "KernelPrerequisites.h"

#include "../Component.h"

#include "OgreLight.h"

#include "components/rendering/Camera.h"

#include <map>

/*! \class Light
	\brief Usado para controlar todos os aspectos de luz da cena.
*/
class KERNEL_VS_EXPORT Light : public Component
{
public:
	enum LightType
	{
		POINT = Ogre::Light::LT_POINT,
		DIRECTIONAL = Ogre::Light::LT_DIRECTIONAL,
		SPOT = Ogre::Light::LT_SPOTLIGHT
	};

	enum LightDesc
	{
		LD_LightType,
		LD_DiffuseColor,
		LD_SpecularColor,
		LD_SpotInnerAngle,
		LD_SpotOuterAngle,
		LD_Enabled,
		LD_BillboardWidth,
		LD_BillboardHeight,
		LD_BillboardColor,
		LD_BillboardMaterial,
		LD_BillboardCreate,
		LD_VisibilityFlags,
		LD_CastShadows,
		LD_QueryFlags,

		LD_Size
	};

	Light();
	Light(const ComponentDescMap& d);
	virtual ~Light();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	/*!
		\brief Atribui o tipo de luz
		\param type Enum de tipo de luz
	*/
    void setType(int type);

	/*!
		\brief Atribui a cor difusa em emissão
		\param r Red
		\param g Green
		\param b Blue
	*/
    void setDiffuseColor(double r, double g, double b);

	/*!
		\brief Atribui a cor difusa em emissão
		\param c Valor de Cor 
	*/
	void setDiffuseColor(const Color& c);

   	/*!
		\brief Atribui a cor especular em emissão
		\param r Red
		\param g Green
		\param b Blue
	*/
    void setSpecularColor(double r, double g, double b);

	 /*!
		\brief Atribui a cor especular em emissão
		\param c Valor de Cor 
	*/
	void setSpecularColor(const Color& c);

	/*!
		\brief Atribui o angulo do Cone de emissão
		\param inner interior
		\param outer exterior
	*/
    void setSpotlightAngles(double inner, double outer);
	void setSpotlightInnerAngle(double inner);
	void setSpotlightOuterAngle(double outer);

	 /*!
		\brief Atribui a visibilidade da emissão
		\param enable Ativa luz ou não 
	*/
    virtual void setEnabled(bool enable);

	 /*!
		\brief Verifica a visibilidade da emissão
	*/
    virtual bool isEnabled();

	 /*!
		\brief Verifica se a emissão está sendo renderizada por alguma câmera
	*/
	bool isVisible();

	/// Cria uma billboard, um obejto que emite a luz como no mundo real
	/*!
		\brief Cria um Billboard representativo de luz
		\param width largura do billboard
		\param height altura do billboard
		\param color cor do billboard
		\param materialName material do billboard
	*/
    void setBillboard(const float width, const float height,
                      const Color &color,
                      const std::string &materialName =
                          std::string("Examples/Flare"));

	 /*!
		\brief Atribui a luz uma flag de renderização
		\param flag Flag de Renderização 
	*/
	void setVisibilityFlags(int flags);

	 /*!
		\brief Define que a luz projetará sombras
		\param enable Ativa renderização de sombras 
	*/
	void setCastShadows(bool enable);

	 /*!
		\brief Verifica se a luz projeta sombras
	*/
	bool getCastShadows();

	 /*!
		\brief Define a atenuação da luz
		\param range distancia da luz
		\param constant valor de emissão constante
		\param linear tipo de projeção
		\param quadratic tipo de projeção
	*/
	void setAttenuation(double range, double constant, double linear, double quadratic);

	/*!
		\brief Define a distancia mais proxima de emissão
		\param r distancia minima 
	*/
	void setSpotlightNearClipDistance(double r);

	virtual void applyDesc(int key, const std::string& value);

	void setLayer(unsigned flags);
	unsigned getLayer();

private:
	/// Ogre::Light
    Ogre::Light *mLight;

	/// Conjunto de Billboards
    Ogre::BillboardSet *billboardSet;
    /// Billboard
    Ogre::Billboard *billboard;

	ComponentDescMap mLightDescMap;
};


#endif