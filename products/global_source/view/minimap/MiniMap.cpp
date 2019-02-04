#include "MiniMap.h"
//#include "moc_MiniMap.cpp"

#include <OgreOverlayManager.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>

#include <OgreRenderWindow.h>
#include <engine/RenderView.h>
#include <input/InputManager.h>

#include <QMenu>
#include <QMainWindow>

#include "view/UiUserLog.h"
#include "utils/Functions.h"

//-----------------------------------------------------------------------------
MiniMap::MiniMap()
	:UiRepresentation(Ui_Representation_MiniMap),
	onMap(false),
	moved(false),
    resized(false),
	optionMenu(0),
	newDim(0)
{
	// iniciando posição inicial para cálculo de reposicionamento
    newPos = Vector2(-2400, 0);

	map = Ogre::OverlayManager::getSingleton().getByName("TerraVR/MiniMap");
	map->hide();

	// pegando o container com todos os elements e containers do mini mapa
    mapPanel = map->getChild("TerraVR/MiniMapPanel");

    // pegando container do mini mapa apenas, ou seja, apenas área do mapa
    containerMap = static_cast<Ogre::OverlayContainer*>
                   (mapPanel->getChild("TerraVR/MiniMapContainer"));
    setMaterialName("TerraVR/MiniMapa");

    //ícones de reposicionamento e redimensionamento
    elementMove = containerMap->getChild("TerraVR/MiniMapMove");
    elementMove->hide();
    elementResize = containerMap->getChild("TerraVR/MiniMapResize");
    elementResize->hide();

    //ícones abaixo dos mapas.
    elementIcon = mapPanel->getChild("TerraVR/MiniMapIconElement");
    elementIconColor = mapPanel->getChild("TerraVR/MiniMapIconElement_lightmap");
    elementTag = static_cast<Ogre::TextAreaOverlayElement*>(mapPanel->getChild("TerraVR/EquipmentTagDisplay"));

	optionMenu = new QMenu();
	actVisible = optionMenu->addAction(tr("Visível"));
	actVisible->setCheckable(true);

	QObject::connect(actVisible, SIGNAL(toggled(bool)), 
		this, SIGNAL(visibilityChanged(bool)) );
}

//-----------------------------------------------------------------------------
MiniMap::~MiniMap()
{
	
}

//-----------------------------------------------------------------------------
double MiniMap::getWidth() const
{
	return containerMap->getWidth();
}

//-----------------------------------------------------------------------------
void MiniMap::close()
{
	setVisible(false);
}

//-----------------------------------------------------------------------------
void MiniMap::setMaterialName(const QString& name)
{
	containerMap->setMaterialName(name.toStdString());
}

//-----------------------------------------------------------------------------
void MiniMap::setVisible(bool cond)
{
	bool aux = map->isVisible();
	if(aux != cond)
	{
		emit visibilityChanged(cond);
		if(cond)
		{
			map->show();
		}
		else
		{
			map->hide();
		}
	}
}

//-----------------------------------------------------------------------------
void MiniMap::changedView(const double& x,const double& y)
{	
	//-------------
	// sobre o mapa
	if (mapPanel->contains(x, y))
	{
		onMap = true;
	}
	else
	{
		onMap = false;
	}

	//-------------
	// move
	{
		bool aux = elementMove->isVisible();
		bool cond = elementMove->contains(x, y);
		if(cond != aux)
		{
			if(cond)
			{
				if (!resized)
				{
					elementMove->show();
				}
			}
			else
			{
				if (!moved)
				{
					elementMove->hide();
				}
			}
		}
	}
	//-------------
	// resize
	{
		bool aux = elementResize->isVisible();
		bool cond = elementResize->contains(x, y);
		if(cond != aux)
		{
			if(cond)
			{
				if (!moved)
				{
					elementResize->show();
				}
			}
			else
			{
				if (!resized)
				{
					elementResize->hide();
				}
			}
		}
	}
	
	if (onMap)
	{
		//if (arg.state.X.rel > 0 ||  arg.state.Y.rel > 0)
		{
			displayTag(x, y);
		}
	} 
	else
	{
		hideDisplayTag();
	}

	if(Input->getMouseButton(OIS::MB_Left))
	{
		moveOrResize();
	}
	else
	{
		if (wasMoving())
			setNewPosition();
		else if (wasResizing())
			setNewDimension();
	}
}

//-----------------------------------------------------------------------------
bool MiniMap::isVisibledRepresentation()
{
	return map->isVisible();
}

//-----------------------------------------------------------------------------
void MiniMap::setVisibledRepresentation(bool cond)
{
	setVisible(cond);
}

//-----------------------------------------------------------------------------
bool MiniMap::isValidName(const QString& id)
{	
	bool cond = false;
	if(id == "TerraVR/MiniMapContainer")
	{
		cond = true;
	}
	else if(id == "TerraVR/MiniMapIconElement")
	{
		cond = true;
	}
	else if(id == "TerraVR/MiniMapIconElement_lightmap")
	{
		cond = true;
	}
	else if(id == "TerraVR/MiniMap")
	{
		cond = true;
	}
	else if(id == "TerraVR/EquipmentTagDisplay")
	{
		cond = true;
	}
	return cond;
}

//-----------------------------------------------------------------------------
void MiniMap::startEditing()
{	
}

//-----------------------------------------------------------------------------
void MiniMap::endEditing()
{
}

//-----------------------------------------------------------------------------
void MiniMap::updateRepresentation()
{	
}

//-----------------------------------------------------------------------------
bool MiniMap::keyPressed(const OIS::KeyEvent &arg)
{
	RenderView * rw = Input->getRenderView();
	if(rw->isOnRender())
	{
		if (arg.key == OIS::KC_D)
		{
			if (Input->getKey(OIS::KC_LCONTROL))
			{
				this->defaut();
			}
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
bool MiniMap::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

//-----------------------------------------------------------------------------
bool MiniMap::mouseMoved( const OIS::MouseEvent &arg,const QString& str)
{
	RenderView * rw = Input->getRenderView();
	if(rw->isActiveWindow() && rw->isOnRender())
    {
		double x;
		double y;

		Position::mouseRelativePosition(arg.state.X.abs,arg.state.Y.abs,x,y);
		changedView(x,y);
	}
	return true;
}

//-----------------------------------------------------------------------------
bool MiniMap::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id
	,const QString& str)
{
	return true;
}

//-----------------------------------------------------------------------------
bool MiniMap::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id
	,const QString& str)
{		
	RenderView * rw = Input->getRenderView();
	if(rw->isOnRender() && rw->isActiveWindow() && !Input->isMouseGrabbed())
	{
		switch(id)
		{
			case OIS::MB_Right:
			{
				if(!optionMenu->isVisible())
				{		
					actVisible->setChecked(true);
					QPoint p = ((QMainWindow*)rw->parentWidget())->pos();
					p += QPoint(arg.state.X.abs+20,arg.state.Y.abs+20);

					optionMenu->popup(p);
					optionMenu->show();		
				}
			}
			break;
			case OIS::MB_Left:
			{					
				if (onMap)
				{
					double x;
					double y;

					Position::mouseRelativePosition(arg.state.X.abs,arg.state.Y.abs,x,y);

					if (elementIconColor->contains(x,y))
					{
						containerMap->setMaterialName("TerraVR/MiniMapaColorido");
						emit changedTextureTerrain(TT_Satellite);
					}
					else if (elementIcon->contains(x,y))
					{
						containerMap->setMaterialName("TerraVR/MiniMapa");						
						emit changedTextureTerrain(TT_ShadesGray);
					}
					else if (!elementMove->isVisible() && !elementResize->isVisible())
					{	
						mousePosition(arg.state.X.abs,arg.state.Y.abs,x,y);
						emit changedPosition(x,y);
					}

				}				
			}
			break;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
void MiniMap::mousePosition(int mX,int mY, double& x, double& y)
{
	RenderView * rw = Input->getRenderView();

	Vector2 posRender = rw->getAbsolutePosition();
	double w = double(rw->getRenderWindow()->getWidth());
    double h = double(rw->getRenderWindow()->getHeight());

	double auxX  = (mX - posRender.x) * 10000 /h;
    double auxY  = (mY - posRender.y) / h * 10000;

	Ogre::Vector2 realPosition = getRealPosition(w, h);

	x = (auxX - realPosition.x)/ containerMap->getWidth();
    y = (auxY - realPosition.y)/ containerMap->getWidth();
}

//-----------------------------------------------------------------------------
void MiniMap::defaut()
{
    if ((getWidth() != 2400) || (mapPanel->getTop() != 0) && (mapPanel->getLeft() != -2400))
    {
        float newWidth,newHeight;
        newWidth = getWidth();
        float scale = Ogre::Math::Abs (2400 / newWidth);

        mapPanel->setWidth(2400);
        mapPanel->setHeight(2700);
        mapPanel->setLeft(-2400);
        mapPanel->setTop(0);

        containerMap->setWidth(2400);
        containerMap->setHeight(2400);

        //pegando filhos
        Ogre::OverlayContainer::ChildIterator it = containerMap->getChildIterator();
        while(it.hasMoreElements())
        {
            Ogre::OverlayElement * element = it.getNext();

            newWidth = element->getWidth() * scale;
            newHeight = element->getHeight() * scale;
            element->setWidth(newWidth);
            element->setHeight(newHeight);
            element->setPosition(element->getLeft()*scale, element->getTop()*scale);

            if (element->getVerticalAlignment() == Ogre::GVA_BOTTOM)
            {
                element->setTop(-newHeight);
            }
        }

        //Ícones
        newWidth = elementIcon->getWidth() * scale;
        newHeight = elementIcon->getHeight() * scale;
        elementIcon->setWidth(newWidth);
        elementIcon->setHeight(newHeight);
        elementIcon->setLeft(-newWidth);
        elementIcon->setTop(containerMap->getHeight());

        newWidth = elementIconColor->getWidth() * scale;
        newHeight = elementIconColor->getHeight() * scale;
        elementIconColor->setWidth(newWidth);
        elementIconColor->setHeight(newHeight);
        elementIconColor->setLeft(newWidth * -2);
        elementIconColor->setTop(containerMap->getHeight());

        //texto
        newWidth = elementTag->getWidth() * scale;
        newHeight = elementTag->getHeight() * scale;
        elementTag->setWidth(newWidth);
        elementTag->setHeight(newHeight);
        elementTag->setLeft(newWidth * -3);
        elementTag->setTop(containerMap->getHeight());
        elementTag->setCharHeight(newHeight);
    }
}

//-----------------------------------------------------------------------------
void MiniMap::displayTag(double x, double y)
{
    Ogre::OverlayElement* dotOnMouse = containerMap->findElementAt(x, y);

    if(dotOnMouse != NULL)
    {

        int posi = -1;

        posi = dotOnMouse->getName().find("_dot");

        if(posi != -1)
        {
            size_t pos;
            pos = dotOnMouse->getName().find("_");

            elementTag->show();

            if(dotOnMouse->getMaterialName() == "TerraVR/Pink")
                elementTag->setColour(Ogre::ColourValue(1,0,0.5));
            else if(dotOnMouse->getMaterialName() == "TerraVR/Yellow")
                elementTag->setColour(Ogre::ColourValue(1,1,0));
            else if(dotOnMouse->getMaterialName() == "TerraVR/Blue")
                elementTag->setColour(Ogre::ColourValue(0,0,1));

            elementTag->setCaption(dotOnMouse->getName().substr(0,pos));
        }
        else
        {
            hideDisplayTag();
        }
    }
}

//-----------------------------------------------------------------------------
void MiniMap::hideDisplayTag()
{
    elementTag->hide();
}


//-----------------------------------------------------------------------------
void MiniMap::moveOrResize()
{
	RenderView * rw = Input->getRenderView();
    if( rw->isActiveWindow() )
    {
        //if (isVisible())
        {
            float mX = Input->mouseX();
            float mY = Input->mouseY();

            Vector2 posRender = rw->getAbsolutePosition();

            float largura = float(rw->getRenderWindow()->getWidth());
            float altura = float(rw->getRenderWindow()->getHeight());

            // posição em pixels
            Vector2 pos = Vector2(mX - posRender.x, mY - posRender.y);

            if (!moved)
            {
                if (!resized)
                {
                    if (elementMove->contains( pos.x / largura, pos.y / altura))
                    {
                        moved = true;
                    }
                }
            }
            else
            {
                float mouseVirtualX = -((largura - pos.x) * 10000.0f /altura);
                float mouseVirtualY = (pos.y) / altura * 10000.0f;

                float virtualWidth = -(10000.0f * (largura/altura));
                float virtualHeight = 10000.0f;

                if (mouseVirtualX < virtualWidth)
                    mouseVirtualX = virtualWidth;
                if (mouseVirtualX  > -getWidth())
                    mouseVirtualX = -getWidth();

                if (mouseVirtualY < 0)
                    mouseVirtualY = 0;
                if (mouseVirtualY > virtualHeight - getWidth() * 1.125)
                    mouseVirtualY = virtualHeight - getWidth() * 1.125;

                newPos = Vector2(mouseVirtualX, mouseVirtualY);
				mapPanel->setPosition(newPos.x, newPos.y);
            }

            if (!resized)
            {
                if (!moved)
                {
                    if (elementResize->contains( (pos.x) / largura,
                                                 (pos.y) / altura))
                    {
                        resized = true;

						float mouseVirtualX = -((largura - pos.x) * 10000.0f /altura);
						float mouseVirtualY = (pos.y) / altura * 10000.0f;

						float virtualWidth = -(10000.0f * (largura/altura));
						float virtualHeight = 10000.0f;

						if (mouseVirtualX < virtualWidth)
							mouseVirtualX = virtualWidth;
						if (mouseVirtualX  > -getWidth())
							mouseVirtualX = -getWidth();

						if (mouseVirtualY < 0)
							mouseVirtualY = 0;
						if (mouseVirtualY > virtualHeight - getWidth() * 1.125)
							mouseVirtualY = virtualHeight - getWidth() * 1.125;

						posDim = Vector2(mouseVirtualX, mouseVirtualY);
                    }
                }
            }
            else
            {
				float mouseVirtualX = -((largura - pos.x) * 10000.0f /altura);
                float mouseVirtualY = (pos.y) / altura * 10000.0f;

                //float virtualWidth = -(10000.0f * (largura/altura));
                float virtualHeight = 10000.0f;

                newPos = Vector2(mouseVirtualX, mouseVirtualY);

                float deltaX = (posDim.x - (newPos.x));
                float deltaY = (posDim.y - (newPos.y));

				posDim = newPos;

                if (Ogre::Math::Abs(deltaX) < Ogre::Math::Abs(deltaY))
                {
                    newDim = deltaX;
                }
                else
                {
                    newDim = deltaY;
                }
				newDim = deltaX;

				setNewDimension();
				resized = true;
            }
        }
    }
}

//-----------------------------------------------------------------------------
bool MiniMap::wasMoving()
{
    return moved;
}

//-----------------------------------------------------------------------------
bool MiniMap::wasResizing()
{
    return resized;
}

//-----------------------------------------------------------------------------
void MiniMap::setNewPosition()
{
    mapPanel->setPosition(newPos.x, newPos.y);
    moved = false;
}

//-----------------------------------------------------------------------------
void MiniMap::setNewDimension()
{
    float newWidth, newHeight;
    float scale, tempWidth;

    // guardando informação da largura antiga do mapa para cálculo da escala
    tempWidth = mapPanel->getWidth();

    //pegando o tamanho da janela em pixels virtuais

    float virtualHeight = 10000.0f * 0.888888f;
	//float virtualWidth = -(10000.0f * (largura/altura));

    // testando se a nova largura é maior que a altura da janela. Sim, a altura
    //, pois redimensionamos sempre com o menor das dimensões
    newWidth = tempWidth + newDim;
    if (newWidth > virtualHeight)
        newWidth = virtualHeight;


    newHeight = mapPanel->getHeight() + newDim * 1.125;
    if (newHeight > virtualHeight)
        newHeight = virtualHeight * 1.125;

    mapPanel->setWidth(newWidth);
    mapPanel->setHeight(newHeight);
    mapPanel->setLeft(-newWidth);

    //newHeight = containerMap->getHeight() + newDim;
    containerMap->setWidth(newWidth);
    containerMap->setHeight(newWidth);

	scale = Ogre::Math::Abs (newWidth / tempWidth);

    //pegando filhos
    Ogre::OverlayContainer::ChildIterator it = containerMap->getChildIterator();
    while(it.hasMoreElements())
    {
        Ogre::OverlayElement * element = it.getNext();

        newWidth = element->getWidth() * scale;
        newHeight = element->getHeight() * scale;
        element->setWidth(newWidth);
        element->setHeight(newHeight);
        element->setPosition(element->getLeft()*scale, element->getTop()*scale);

        if (element->getVerticalAlignment() == Ogre::GVA_BOTTOM)
        {
            element->setTop(-newHeight);
        }
    }

    //Ícones
    newWidth = elementIcon->getWidth() * scale;
    newHeight = elementIcon->getHeight() * scale;
    elementIcon->setWidth(newWidth);
    elementIcon->setHeight(newHeight);
    elementIcon->setLeft(-newWidth);
    elementIcon->setTop(containerMap->getHeight());

    newWidth = elementIconColor->getWidth() * scale;
    newHeight = elementIconColor->getHeight() * scale;
    elementIconColor->setWidth(newWidth);
    elementIconColor->setHeight(newHeight);
    elementIconColor->setLeft(newWidth * -2);
    elementIconColor->setTop(containerMap->getHeight());

    //texto
    newWidth = elementTag->getWidth() * scale;
    newHeight = elementTag->getHeight() * scale;
    elementTag->setWidth(newWidth);
    elementTag->setHeight(newHeight);
    elementTag->setLeft(newWidth * -3);
    elementTag->setTop(containerMap->getHeight());
    elementTag->setCharHeight(newHeight);

    resized = false;
}

//-----------------------------------------------------------------------------
Ogre::Vector2 MiniMap::getRealPosition(const float largura, const float altura)
{
    //para retornar em pixels virtuais

    //código de escala tirado do código fonte ogre. OgreOverlayElement.cpp
    float scaleX = (10000.0f * (largura/altura));
    float scaleY = 10000.0f;

    return Ogre::Vector2(containerMap->_getDerivedLeft() * scaleX,
                         containerMap->_getDerivedTop() * scaleY);
}