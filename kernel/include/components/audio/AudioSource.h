#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "KernelPrerequisites.h"
#include "../Component.h"

#include <map>

namespace OgreAL
{
	class Sound;
}

class KERNEL_VS_EXPORT AudioSource : public Component
{
public:
	enum AudioSourceDesc
	{
		ASD_Loop = 0,
		ASD_Pitch = 1,
		ASD_Volume = 2,
		ASD_Size = 3
	};

	//construtor automatico, idComponent gerado automaticamente
	/**
      * @brief Construtor 
     */
	AudioSource();
	/**
      * @brief Construtor 
      * @param idComponent id do Component
     */
	AudioSource(std::string idComponent);
	/**
      * @brief Construtor
      * @param d descritor
     */
	AudioSource(const ComponentDescMap &d);
	/**
      * @brief Destrutor 
     */
	virtual ~AudioSource();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	/**
      * @brief Se a musica esta rodando 
     */
	bool isPlaying();

	/**
      * @brief Atribui a execu��o o tipo loop
      * @param loop booleano de ativa��o
     */
	void setLoop(bool loop);
	/**
      * @brief Se a musica esta em loop 
     */
	bool isLoop();
	/**
      * @brief Atribui a execu��o o volume desligado
      * @param mute booleano de ativa��o
     */
	void setMute(bool mute);

	/**
      * @brief Se a musica esta mutada 
     */
	bool isMute();
	/**
      * @brief Atribui o tom da musica em execu��o
      * @param p booleano de ativa��o
     */
	void setPitch(double p);

	/**
      * @brief Pega o valor do tom da musica em execu��o 
     */
	double getPitch();
	/**
      * @brief Atribui o volume da musica em execu��o
      * @param v booleano de ativa��o
     */
	void setVolume(float v);
	/**
      * @brief Pega o valor do volume da musica em execu��o 
     */
	float getVolume();
	/**
      * @brief Paraliza a musica em execu��o 
     */
	void pause();
	/**
      * @brief Inicia a musica em execu��o 
     */
	void play();
	/**
      * @brief Para a musica em execu��o 
     */
	void stop();

	void setAudioFileName(const std::string& name);
	std::string getAudioFileName();

	virtual void setEnabled(bool enable);
	virtual bool isEnabled();

private:

	OgreAL::Sound* getOgreALSound();
	
	void applyDesc(int key, const std::string& value);

private:

	OgreAL::Sound* sound;

	ComponentDescMap mAudioSourceDescMap;

	float lastVolBeforeMute;
	bool mIsMute;
	std::string filename;

	bool enabled;
};

#endif