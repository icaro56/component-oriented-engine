#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

#include <QDir>
#include <QTime>
#include <cstdio>
#include <cmath>
#include <QStandardPaths>
#include <qdebug.h>
#include <iostream>

#include "ProjectPrerequisites.h"
 
namespace GlobalConfig
{
	//caminho para a pasta de daddos locais
	static const QString LOCALDATA = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Products/";
	
	//caminho para a pasta de recursos globais
	static const QString GMEDIAPATH = "../../../products/global_resource";

	//caminho para a pasta de dados do usuário
	static const QString DATAPATH = LOCALDATA + ProjectInfo::PROJECT_NAME;

	//caminho para a pasta de log
	static const QString LOGPATH = DATAPATH + "/log";


	static const QString SERVER = "dir_Server";
	
	static const QString ICONPATH = GMEDIAPATH + "/icons/";
	
	static const QString CURSORPATH = GMEDIAPATH + "/icons/cursors/";

	//imagens de splash
	static const QString IMG_NAME = "/images/";
	static const QString IMG_FOLDER = GMEDIAPATH + IMG_NAME;

	//caminho para a pasta modelos
	static const QString MODELS_NAME = "/models/";
	static const QString GMODELS_FOLDER = GMEDIAPATH + MODELS_NAME;

	//caminho da documentação de ajuda
	static const QString HELPDOCS_PATH = GMEDIAPATH + "/help/documentation";

	//caminho das traduções
	static const QString TRANSLATIONS_PATH = GMEDIAPATH + "/translations";

	static QString TERRAIN_DIR = "/terrains/";
	
	static const QString USER_LOG = LOGPATH + "/user/";

	static const QString BLOCK_LOG = LOGPATH + "/blocks/";
	static const QString BLOCK_DIR = "/blocks/";

    /*!
        Converte um número em formato de hora "hh:mm"
        \param time double, tempo em double
    */
    static const std::string formatTime(double time)
    {
        QTime tempTime(0,0);

        tempTime = tempTime.addSecs(time);
        QString strTime = tempTime.toString("hh:mm:ss");

        return strTime.toStdString();
    }


    /*!
        Separa o nome do arquivo e o caminho de um endereço
        \param cN QString cN, nome completo
        \param fN QString fN, nome do arquivo
        \param dN QString dN, nome do diretório
        \author Sergio
    */
    static void splitNameAndDir(QString cN, QString* fN, QString* dN)
    {
        int size = cN.size() - 1;
        while(size >= 0)
        {
			if(cN.at(size).toLatin1() == '/')
            {
                cN.resize(size);
                dN->append(cN);
                break;
            }
            else
                fN->push_front(cN.at(size));
            size--;
        }
    }


	inline static void checkAndCreateFolder(const QString& name)
	{
		QDir dir;
		if(!dir.exists(name))
		{
			dir.mkdir(name);
		}
	}

	static void createDatFolders()
	{
		checkAndCreateFolder(LOCALDATA);
		checkAndCreateFolder(DATAPATH);
		
		checkAndCreateFolder(LOGPATH);
		checkAndCreateFolder(USER_LOG);
		checkAndCreateFolder(BLOCK_LOG);
		
		checkAndCreateFolder(DATAPATH + BLOCK_DIR);
		checkAndCreateFolder(DATAPATH + TERRAIN_DIR);
	}
};


#endif // GLOBAL_CONFIG_H
