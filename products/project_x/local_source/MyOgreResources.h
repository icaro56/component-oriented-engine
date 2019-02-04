#ifndef OGRERESOURCE_H
#define OGRERESOURCE_H

#include <string>
#include "engine/Engine.h"
#include "engine/RenderingEngine.h"
#include "system/ProjectConfig.h"

void initResources()
{
	///Adicionando recursos da Ogre
	std::string gMediapath = GlobalConfig::GMEDIAPATH.toStdString();
	std::string mediapath = GlobalConfig::MEDIAPATH.toStdString();

    Rendering->addResourceLocation(".", "FileSystem", "General");
    //GRender->addResourceLocation(gMediapath + "/mygui_media", "FileSystem", "General", true);
    
    //GRender->addResourceLocation(mediapath, "FileSystem", "General");
    Rendering->addResourceLocation(gMediapath + "/fonts.dll", "Zip", "General");
    //GRender->addResourceLocation(gMediapath + "/caelum.dll", "Zip", "General");
    Rendering->addResourceLocation(gMediapath + "/materials/programs.dll", "Zip", "General");
    Rendering->addResourceLocation(gMediapath + "/materials/textures.dll", "Zip", "General");
    Rendering->addResourceLocation(gMediapath + "/materials/scripts.dll", "Zip", "General");
	Rendering->addResourceLocation(mediapath + "/materials/terrain.dll", "Zip", "General");
    Rendering->addResourceLocation(gMediapath + "/particles.dll", "Zip", "General");
    Rendering->addResourceLocation(gMediapath + "/models/core.dll", "Zip", "General");
    
    //GRender->addResourceLocation(Globals::MEDIAPATH + "/terrenos","FileSystem","Terrenos");
	Rendering->addResourceLocation(mediapath + "/blocks/material","FileSystem","General");
    //GRender->addResourceLocation(mediapath + "/blocks/blocks","FileSystem","General");

	Rendering->addResourceLocation(gMediapath + "/skyx", "FileSystem", "skyx");
	Rendering->addResourceLocation(mediapath + "/sounds", "FileSystem", "General");
	Rendering->addResourceLocation(gMediapath + "/models/stage.dll", "Zip", "General");
	/*GRender->addResourceLocation(mediapath + "/models", "FileSystem", "General");
    GRender->addResourceLocation(mediapath + "/models/equipaments.dll", "Zip", "General", true);
    GRender->addResourceLocation(mediapath + "/models/bsms.dll", "Zip", "General", true);
    GRender->addResourceLocation(mediapath + "/models/structures.dll", "Zip", "General", true);*/


    ///Inicializando recursos
    Rendering->initialiseAllResourceGroups();
}

#endif