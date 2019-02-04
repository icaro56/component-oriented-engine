#ifndef LOCALS_H
#define LOCALS_H
#include "system/GlobalConfig.h"

#include "ProjectPrerequisites.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace GlobalConfig
{
	//nome do executável
	static const QString NAME_EXE = "Project_X.exe";
	
	//caminho para a pasta de recursos do projeto
	static const QString MEDIAPATH = "../../../products/project_x/local_resource";

	static const QString MODELS_FOLDER = MEDIAPATH + MODELS_NAME;

	// caminho relativo para o arquivo de configuração
	static const QString CONFIG_FILE_PATH = DATAPATH + "/config.ini";
	static const QString CONFIG_DEFAULT_FILE_PATH = MEDIAPATH + "/config_default.ini";
	
	static const QString TERRAINS_NAME = "/terrains/";
	static const QString TERRAIN_FOLDER = DATAPATH + TERRAINS_NAME;
	static const QString TERRAIN_FOLDER_DEFAULT = MEDIAPATH + TERRAINS_NAME;

	// caminho relativo para o arquivo de dem (atributos de conexão com o servidor ftp)
    static const QString DEM_FILE_PATH = TERRAIN_FOLDER + "dem.dll";
	static const QString DEM_FILE_DEFAULT_PATH = TERRAIN_FOLDER_DEFAULT + "dem_default.dll";

	// caminho relativo para o arquivo de listagem de terrenos
	static const QString TERRAIN_LIST_PATH = DATAPATH + "/terrainList.xml";
	static const QString TERRAIN_LIST_DEFAULT_PATH = MEDIAPATH + "/terrainList_default.xml";

	// caminho relativo para o arquivo de listagem de terrenos
	static const QString TERRAIN_LIST_DIR_PATH = DATAPATH + "/terrainPathList.xml";
	static const QString TERRAIN_LIST_DIR_DEFAULT_PATH = MEDIAPATH + "/terrainPathList_default.xml";

	// caminho relativo para o arquivo de dimensões dos terrenos
    static const QString TERRAIN_PATH = TERRAIN_FOLDER + "dimensions";
	static const QString TERRAIN_PATH_DEFAULT = TERRAIN_FOLDER_DEFAULT + "dimensions";

	static const QString BLOCKS_NAME = "/blocks/";
    static const QString BLOCKS_DIR = MEDIAPATH + BLOCKS_NAME;
	static const QString BLOCKS_ID = "IJK";   //id do bloco
    static const QString BLOCKS_DIMX = "XINC";   //dimensão em X
    static const QString BLOCKS_DIMY = "YINC";   //dimensão em Y
    static const QString BLOCKS_DIMZ = "ZINC";   //dimensão em Z
    static const QString BLOCKS_POSX = "XC";   //posição em X
    static const QString BLOCKS_POSY = "YC";  //posição em Y
    static const QString BLOCKS_POSZ = "ZC";   //posição em Z
    static const QString BLOCKS_DENS = "DENS";//densidade
	static QString BLOCKS_LITO = "LITO";   //litologia

	static const Vector3 INITIAL_POS_CAM = Vector3(899, 873, 700);
	static const Quaternion INITIAL_ORI_CAM = Quaternion(0.90081, -0.100218, 0.351449, 0.0378691);
	static const float SPEED_CAM = 60.0f;
	static const float SPEED_AVATAR = 2.0f;
	
	static const Vector3 INITIAL_POS_AVATAR = Vector3(899, 873, 300);
};

#endif