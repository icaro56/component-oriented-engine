#ifndef RENDERER_H
#define RENDERER_H

#include "KernelPrerequisites.h"

#include "../Component.h"
#include "math/Bounds.h"

#include <map>

namespace Ogre
{
	class Entity;
}

class KERNEL_VS_EXPORT Renderer : public Component
{
	friend class SceneCallback;

public:
	enum RendererDesc
	{
		RD_Enabled,
		RD_CastShadows,
		RD_QueryFlags,
		RD_VisibilityFlags,
		RD_MaterialName,
		RD_Size,
	};

	Renderer();
	Renderer(std::string idComponent);
	Renderer(const ComponentDescMap& d);
	virtual ~Renderer();

	virtual void prerequisites();
	virtual void init(const std::string& name);
	virtual void nextInit(){}

	virtual void setEnabled(bool enable);
	virtual bool isEnabled();

	// retorna se mesh est� sendo renderizado por alguma c�mera
	bool isVisible();

	const Bounds getBounds() const;

	/**
     * @brief Ativa gera��o de sombras por esta entidade.
     * @remarks Define a gera��o de sombras pelo metodo setCastShadows()
     * do Ogre
     * @param enable Habilita ou n�o a gera��o de sombras
    */
    void setCastShadows(bool enable);

	/**
     * @brief Define flag usada em m�scaras de sele��o.
     * @remarks Define o flag pelo metodo setQueryFlags() do Ogre
     * @param flags Posi��o de deslocamento de bits
    */
    void setLayer(unsigned flags);
	unsigned getLayer();

	void setVisibilityFlags(int flags);
	unsigned getVisibilityFlags();

	void setMaterialName(const std::string& name);

	Ogre::Entity* getOgreEntity();

protected:
	void applyDesc(int key, const std::string& value);

	void setOgreEntity(Ogre::Entity* ent);

protected:
	Ogre::Entity* mEntity;

	ComponentDescMap mRendererDescMap;
};

#endif