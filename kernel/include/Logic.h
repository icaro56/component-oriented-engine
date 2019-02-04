#ifndef LOGIC_H
#define LOGIC_H

#include "KernelPrerequisites.h"

class KERNEL_VS_EXPORT Logic
{
public:
	//************************************
	// Method:    Logic
	// FullName:  Logic::Logic
	// Access:    public 
	// Returns:   
	// Qualifier:
	//************************************
	Logic();

	//************************************
	// Method:    ~Logic
	// FullName:  Logic::~Logic
	// Access:    virtual public 
	// Returns:   
	// Qualifier:
	//************************************
	virtual ~Logic();

	//************************************
	// Method:    init
	// FullName:  Logic::init
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	//************************************
	virtual void init() = 0;


	//************************************
	// Method:    updateLogic
	// FullName:  Logic::updateLogic
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	//************************************
	virtual void updateLogic() = 0;

private:
};

#endif