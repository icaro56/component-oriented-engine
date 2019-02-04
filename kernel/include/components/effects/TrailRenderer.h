#ifndef TRAILRENDERING_COMPONENT_H
#define TRAILRENDERING_COMPONENT_H

#include "KernelPrerequisites.h"
#include "../Component.h"
#include "math/Vector2.h"
#include "math/Color.h"
#include "OgreRibbontrail.h"

class KERNEL_VS_EXPORT TrailRenderer : public Component
{

public:
	enum TrailDesc
	{
		TR_MaxElements,
		TR_NumberOfChains,
		TR_MaterialName,
		TR_InitialColour,
		TR_TrailLength,
		TR_ColourChange,
		TR_InitialWidth,
		TR_Enabled,
		TR_QueryFlags,
		TR_VisibilityFlags
	};

	TrailRenderer();
	TrailRenderer(std::string idComponent);
	TrailRenderer(const ComponentDescMap& d);
	virtual ~TrailRenderer();

	void setMaxElements(const int max);
	void setNunberOfChains(const int num);
	void setMaterial(const std::string mat);
	void setInitialColour(const Color color = Color::Blue);
	void setColourChange(const int size = 0 ,const Color color = Color::Blue);
	void setInitialWidth(const int max);
	void setTrailLength(const int max);
	void setAnchor(GameObject*pivot);

	virtual void setEnabled(bool enable);
	virtual bool isEnabled();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	void setLayer(unsigned flags);
	unsigned getLayer();

	void setVisibilityFlags(int flags);
	unsigned getVisibilityFlags();


private:

	void applyDesc(int key, const std::string& value);

	Ogre::RibbonTrail* trail;
	ComponentDescMap mTrailDescMap;
};
#endif