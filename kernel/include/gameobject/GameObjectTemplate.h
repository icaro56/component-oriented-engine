
#ifndef __OBJECTTEMPLATE_HPP__
#define __OBJECTTEMPLATE_HPP__

#include "KernelPrerequisites.h"
#include <vector>

#include "components/ComponentTemplate.h"

class KERNEL_VS_EXPORT GameObjectTemplate 
{
public:
	GameObjectTemplate( const std::string& name);

	~GameObjectTemplate();

	void clearComponents();

	const std::string& name() const;

	void setName(const std::string& name);

	ComponentSetTemplate* componentSetTemplate();
	
	void setCompSetTemplate( ComponentSetTemplate* compTemplate);

protected:
	std::string mName;
	ComponentSetTemplate* myComponentSetTemplate;
};

#endif
