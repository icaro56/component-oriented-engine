#ifndef MINI_MAP_ELEMENT_H
#define MINI_MAP_ELEMENT_H

#include <QString>

namespace Ogre
{
    class OverlayElement;
	class OverlayContainer;
	class Vector3;
}

class MiniMapElement
{
public:
	MiniMapElement(const QString& name);
	~MiniMapElement();

	static Ogre::OverlayContainer* getParent();  

	void setMaterial(const QString &material);

	/*
		\brief Configura dimensão proporcional ao pai. Se configurado largura igual
		a 0.5, então elemento terá a metade da largura em relação ao pai
	*/
	void setDimensionsProportionalToTheParent(float w, float h);

	/*!
        \brief Atualiza a visualização do elemento.
        \param visible bool diz se está ou não visível.
    */
    void setVisible(const bool visible);

	/*!
        \brief Atualiza a posição do elemento de acordo com a posição do equipamento.
        observação
    */
    void update(const Vector3 position);

	/*!
        \brief Atualiza a posição do elemento de acordo com a posição do equipamento.
		E atualiza sua orientação de acordo com o grau de rotação
        observação
    */
    void update(const Vector3 position, const float degree);

private:
	Ogre::OverlayElement* element;

    static Ogre::OverlayContainer* parent;
};

#endif