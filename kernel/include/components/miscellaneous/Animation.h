#ifndef ANIMATION_H
#define ANIMATION_H

#include "KernelPrerequisites.h"

#include "../Component.h"

#include <map>

namespace Ogre
{
	class AnimationState;
}

class SkinnedMeshRenderer;

/*! \class Animation
    \brief O componente de animação é usado para reproduzir animações.
    \brief Você pode atribuir animações para o componente de 
	\brief animação e controlar toda sua reprodução.
*/

class KERNEL_VS_EXPORT Animation : public Component
{
public:
	enum AnimationDesc
	{
		AD_PlayAutomatically = 0,
		AD_Loop = 1,
		AD_CurrentAnimation = 2,
		AD_Size = 3
	};

	typedef std::map<const std::string, Ogre::AnimationState*> AnimationMap;

	Animation();
	Animation(const ComponentDescMap& d);
	virtual ~Animation();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit();
	virtual void update();
	
	/*!
		\brief Ativa componente de animação
		\param enable variavel de ativação
	*/
	virtual void setEnabled(bool e);
	virtual bool isEnabled();


	/*!
		\brief Ativa automaticamente o componente de animação
		\param active variavel de ativação
	*/
	void setPlayAutomatically(bool active);
	/*!
		\brief Define a velocidade da animação
		\param s valor de velocidade
	*/
	void setAnimationSpeed(double s);
	/*!
		\brief Ativa a animação
		\param name Nome da Animação
	*/
	void play(const std::string &name);
	/*!
		\brief Pausa a animação
	*/
	void stop();
	/*!
		\brief Atribui repetição a animação
		\param active ativa repetição da Animação
	*/
	void setLoop(bool active);
	/*!
		\brief Verifica se a animação está ativada
	*/
    bool isPlaying();
	/*!
		\brief Captura a animação pelo nome
		\param name Nome da animação
	*/
	Ogre::AnimationState* getAnimation(const std::string& name);

protected:
	virtual void applyDescription(ComponentDescMap& mComponentDescMap);
	void applyDesc(int key, const std::string& value);
	void requestAnimations(SkinnedMeshRenderer* skin);

private:
	Ogre::AnimationState* activeAnimationState;

	ComponentDescMap mAnimationDescMap;

	AnimationMap animationMap;

	bool enabled;
	double speed;
};

#endif