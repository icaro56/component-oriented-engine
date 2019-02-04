#ifndef BILLBOARD_COMPONENT_H
#define BILLBOARD_COMPONENT_H

#include "KernelPrerequisites.h"
#include "OgreBillboard.h"
#include "OgreBillboardSet.h"
#include "../Component.h"
#include "math/Vector2.h"
#include "math/Color.h"
#include <map>

/*! \class Billboard
	\brief Usado para representar um quadro que sempre olha para a câmera .
*/
class KERNEL_VS_EXPORT Billboard : public Component
{
public:

	enum BillboardType
	{
		BBT_POINT = Ogre::BillboardType::BBT_POINT,
		BBT_ORIENTED_COMMON = Ogre::BillboardType::BBT_ORIENTED_COMMON,
		BBT_ORIENTED_SELF = Ogre::BillboardType::BBT_ORIENTED_SELF,
		BBT_PERPENDICULAR_SELF = Ogre::BillboardType::BBT_PERPENDICULAR_SELF
	};

	enum BillboardOrigin
	{
		BOTTOM_CENTER = Ogre::BillboardOrigin::BBO_BOTTOM_CENTER,
		BOTTOM_LEFT = Ogre::BillboardOrigin::BBO_BOTTOM_LEFT,
		BOTTOM_RIGHT = Ogre::BillboardOrigin::BBO_BOTTOM_RIGHT,
		CENTER = Ogre::BillboardOrigin::BBO_CENTER,
		CENTER_LEFT = Ogre::BillboardOrigin::BBO_CENTER_LEFT,
		CENTER_RIGHT = Ogre::BillboardOrigin::BBO_CENTER_RIGHT,
		TOP_CENTER = Ogre::BillboardOrigin::BBO_TOP_CENTER,
		TOP_LEFT = Ogre::BillboardOrigin::BBO_TOP_LEFT,
		TOP_RIGHT = Ogre::BillboardOrigin::BBO_TOP_RIGHT
	};

	enum BillboardDesc
	{
		BD_Enabled,
		BD_Origin,
		BD_Type,
		BD_Dimension,
		BD_Color,
		BD_Material,
		BD_QueryFlags,

		BD_Size,
	};


	Billboard(void);
	Billboard(const ComponentDescMap & d);
	virtual ~Billboard(void);

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	/*!
		\brief Atribui o tipo de billboard
		\param BillboardType type Enum de tipo de posicionamento do billboard
	*/
    void setType(int type);
	/*!
		\brief Atribui a Origem de billboard
		\param BillboardOrigin type Enum de tipo de posicionamento do billboard na Origem
	*/
    void setOrigin(int type);
	/*!
		\brief Atribui a cor difusa em emissão
		\param r Red
		\param g Green
		\param b Blue
	*/
    void setColor(double r, double g, double b);
	/*!
		\brief Atribui a cor difusa em emissão
		\param c Valor de Cor 
	*/
	void setColor(const Color& c);
	/*!
		\brief Atribui a visibilidade do Billboard
		\param enable Ativa o Billboard sim ou não 
	*/
    virtual void setEnabled(bool enable);
	 /*!
		\brief Verifica a visibilidade do Billboard
	*/
    virtual bool isEnabled();
	 /*!
		\brief Verifica se o billboard está sendo renderizado por alguma câmera
	*/
	bool isVisible();

	void setDimensions(double w,double h);
	void setDimensions(const Vector2& dim);

	void setMaterial(std::string name);
	
	virtual void applyDesc(int key, const std::string& value);

	void setLayer(unsigned flags);
	unsigned getLayer();

private:

	Ogre::BillboardSet *billboardSet;
	Ogre::Billboard *billboard;

	ComponentDescMap mBillboardDescMap;
};
#endif
