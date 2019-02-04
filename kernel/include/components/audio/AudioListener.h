#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "KernelPrerequisites.h"
#include "../Component.h"

class KERNEL_VS_EXPORT AudioListener : public Component
{
public:
	AudioListener();
	AudioListener(std::string idComponent);
	virtual ~AudioListener();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}
	virtual void setEnabled(bool enable);
	virtual bool isEnabled();

private:

	bool enabled;
};

#endif