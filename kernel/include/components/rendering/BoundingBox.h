#ifndef BOUND_BOX_H
#define BOUND_BOX_H

#include "KernelPrerequisites.h"
#include "../Component.h"

#include "OrientedBoundingBox.h"

#include "math/Color.h"

class KERNEL_VS_EXPORT BoundingBox : public Component
{
public:
	enum BoundingBoxDesc
	{
		BBD_Color,
		BBD_Enabled,
		BBD_QueryFlags,
		BBD_Size
	};

	BoundingBox();
	BoundingBox(const ComponentDescMap& d);
	virtual ~BoundingBox();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	void setColor(const Color& c);
	Color getColor();

	/*!
		\brief Atribui a visibilidade
	*/
    virtual void setEnabled(bool enable);
	 /*!
		\brief Verifica a visibilidade 
	*/
    virtual bool isEnabled();

	void setLayer(unsigned flags);
	unsigned getLayer();

private:

	virtual void applyDesc(int key, const std::string& value);

private:

	OrientedBoundingBox* mOrientedBoundingBox;

	ComponentDescMap mBoundingBoxDescMap;
};

#endif