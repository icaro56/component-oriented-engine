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
    \brief O componente de anima��o � usado para reproduzir anima��es.
    \brief Voc� pode atribuir anima��es para o componente de 
	\brief anima��o e controlar toda sua reprodu��o.
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
		\brief Ativa componente de anima��o
		\param enable variavel de ativa��o
	*/
	virtual void setEnabled(bool e);
	virtual bool isEnabled();


	/*!
		\brief Ativa automaticamente o componente de anima��o
		\param active variavel de ativa��o
	*/
	void setPlayAutomatically(bool active);
	/*!
		\brief Define a velocidade da anima��o
		\param s valor de velocidade
	*/
	void setAnimationSpeed(double s);
	/*!
		\brief Ativa a anima��o
		\param name Nome da Anima��o
	*/
	void play(const std::string &name);
	/*!
		\brief Pausa a anima��o
	*/
	void stop();
	/*!
		\brief Atribui repeti��o a anima��o
		\param active ativa repeti��o da Anima��o
	*/
	void setLoop(bool active);
	/*!
		\brief Verifica se a anima��o est� ativada
	*/
    bool isPlaying();
	/*!
		\brief Captura a anima��o pelo nome
		\param name Nome da anima��o
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