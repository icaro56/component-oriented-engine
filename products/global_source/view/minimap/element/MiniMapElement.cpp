#include "MiniMapElement.h"

#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreVector3.h>

#include "surface/SurfaceManager.h"

#include <engine/Engine.h>
#include <engine/RenderView.h>

Ogre::OverlayContainer* MiniMapElement::parent = 0;

MiniMapElement::MiniMapElement(const QString& name)
	: element(NULL)
{
	getParent();

    element = Ogre::OverlayManager::getSingleton().createOverlayElement(
              "Panel", name.toStdString());

	int mapSize = parent->getWidth();

    element->setMetricsMode(Ogre::GMM_RELATIVE_ASPECT_ADJUSTED);
    element->setDimensions(0.015 * mapSize,0.015 * mapSize);
    element->setPosition(0,0);
    parent->addChild(element);
}

MiniMapElement::~MiniMapElement()
{
	Ogre::OverlayManager::getSingleton().destroyOverlayElement(element);
    element = 0;
}

Ogre::OverlayContainer* MiniMapElement::getParent()
{
    if(parent == NULL)
    {
        parent = static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().getByName("TerraVR/MiniMap")->
        getChild("TerraVR/MiniMapPanel")->getChild("TerraVR/MiniMapContainer"));
    }

    return parent;
}

void MiniMapElement::setMaterial(const QString &material)
{
	element->setMaterialName(material.toStdString());
}

void MiniMapElement::setDimensionsProportionalToTheParent(float w, float h)
{
	int mapSize = parent->getWidth();

    element->setDimensions(w * mapSize, h * mapSize);
}

void MiniMapElement::setVisible(const bool visible)
{
    if(visible)
    {
        element->show();
    }
    else
    {
        element->hide();
    }
}

void MiniMapElement::update(const Vector3 position)
{
	float terrainWidth = SurfaceManager::getInstance()->getWidth();
    Vector2 pos(0,0);
    Vector2 winSize = Rendering->getRenderView("render1")->getDimensions();

    float halfWidth = terrainWidth * 0.5;

	int mapSize = parent->getWidth();
    pos.x = (position.x + halfWidth) * mapSize / terrainWidth;
    pos.y = (position.z + halfWidth) * mapSize / terrainWidth;

    int tam = element->getWidth() / 2;

	//testando limites
    if(pos.x < 0)
    {
        pos.x = 0;
    }
    else if( pos.x > mapSize )
    {
        pos.x = mapSize - tam;
    }

    if(pos.y < 0)
    {
        pos.y = tam;
    }
    else if( pos.y > mapSize )
    {
        pos.y = mapSize - tam;
    }

    element->setPosition(pos.x - tam, pos.y - tam);
}

void MiniMapElement::update(const Vector3 position, const float degree)
{
	float terrainWidth = SurfaceManager::getInstance()->getWidth();
    Vector2 pos(0,0);
	Vector2 winSize = Rendering->getRenderView("render1")->getDimensions();

    float halfWidth = terrainWidth * 0.5;

	int mapSize = parent->getWidth();
    pos.x = (position.x + halfWidth) * mapSize / terrainWidth;
    pos.y = (position.z + halfWidth) * mapSize / terrainWidth;

    int tam = element->getWidth() / 2;
	Ogre::Vector2 posCenter = Ogre::Vector2(pos.x - tam, pos.y - tam);

	//Calculando posição com rotação
	pos.x = posCenter.x + tam * Ogre::Math::Sin(Ogre::Degree(degree));
    pos.y = posCenter.y + tam * Ogre::Math::Cos(Ogre::Degree(degree));

	//testando limites
    if(pos.x < 0)
    {
        pos.x = 0;
    }
    else if( pos.x > mapSize )
    {
        pos.x = mapSize - tam;
    }

    if(pos.y < 0)
    {
        pos.y = tam;
    }
    else if( pos.y > mapSize )
    {
        pos.y = mapSize - tam;
    }

    element->setPosition(pos.x, pos.y);
    Rendering->rotOverlayElement(element, degree);
}