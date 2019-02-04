#include "IniConfig.h"

#include "system/ProjectConfig.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QObject>

//#include "log/EventLog.h"
//falta
//#include "log/ProcessorLogSystem.h"

IniConfig * IniConfig::instance = 0;

//-----------------------------------------------------------------------------
IniConfig::IniConfig()
	:KeyValueFileSystem()
{		
	setSeparator("=");
}

//-----------------------------------------------------------------------------
IniConfig::~IniConfig()
{ 
}

//-----------------------------------------------------------------------------
IniConfig * IniConfig::getInstance()
{
	if(!instance)
	{
		instance = new IniConfig();
	}
	return instance;
}

//-----------------------------------------------------------------------------
void IniConfig::init()
{ 
}

//-----------------------------------------------------------------------------
void IniConfig::destroy()
{ 
	if(instance)
	{
		delete instance;
	}
}
 
//-----------------------------------------------------------------------------
void IniConfig::read()
{
    QFile file(GlobalConfig::CONFIG_FILE_PATH);
	if(file.exists())
	{
		setFileName(GlobalConfig::CONFIG_FILE_PATH);
	}
	else
	{
		file.setFileName(GlobalConfig::CONFIG_DEFAULT_FILE_PATH);
		if(file.exists())
		{
			setFileName(GlobalConfig::CONFIG_DEFAULT_FILE_PATH);
		}
		else
		{
			QMessageBox::warning(0, QObject::tr("Projeto X"),
                    QObject::tr("Arquivo de configuração corrompido."),
						QMessageBox::Ok,
                    QMessageBox::Ok);
			exit(0);
			return;
		}
	}

	if(KeyValueFileSystem::read())
	{
		/*EventLog ev;
		ev.setValueProperties(ProcessorLogSystem::Type,QObject::tr("váriavel inválida"));
		ev.setValueProperties(ProcessorLogSystem::Time,QDateTime::currentDateTime().toString("hh:mm:ss"));
		ProcessorLogSystem::getInstance()->receives(ev);*/
	}
}
