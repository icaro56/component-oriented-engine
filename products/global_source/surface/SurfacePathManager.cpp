#include "SurfacePathManager.h"
#include "SurfaceListerner.h"
#include "SurfacePath.h"
#include "SurfacePathFileConfig.h"
#include "SurfacePathFileConfigData.h"
#include "SurfaceException.h"
#include "persistence/filesystem/DataFileSystem.h"
#include "system\ProjectConfig.h"

SurfacePathManager* SurfacePathManager::instance = 0;

//-----------------------------------------------------------------------------
SurfacePathManager::SurfacePathManager()
{
	isInit = true;
	listerner = SurfaceListerner::getInstance();
}

//-----------------------------------------------------------------------------
SurfacePathManager::~SurfacePathManager()
{
	listerner = 0;
}	

//-----------------------------------------------------------------------------
SurfacePathManager * SurfacePathManager::getInstance()
{
	if(!instance)
	{
		instance = new SurfacePathManager();
		instance->init();
	}
	return instance;
}

//-----------------------------------------------------------------------------
void SurfacePathManager::destroy()
{
	delete instance;
	instance = 0;
}

//-----------------------------------------------------------------------------
void SurfacePathManager::init()
{
	if(isInit)
	{
		isInit = false;
		fileConfigurationRead();
	}
}

//-----------------------------------------------------------------------------
void SurfacePathManager::addPath(const SurfacePath& v)
{
	SurfacePath path = v;	
	nId+=1;
	path.setId(nId);
	paths.insert(nId,path);
	paths[nId].initTimer();
	fileConfigurationSave();
}

//-----------------------------------------------------------------------------
void SurfacePathManager::editPath(const SurfacePath& v)
{
	SurfacePath sp  = v;
	unsigned int id = sp.getId();
	if(paths.contains(id))
	{
		if(sp.getAttribute(TSPA_TimeMonitoring) != 
			paths[id].getAttribute(TSPA_TimeMonitoring))
		{
			QString aux = QString::number(QDateTime::currentDateTime().toTime_t());
			sp.setAttribute(TSPA_LastTime,aux);
		}
		paths.insert(id,sp);
		paths[id].initTimer();
		fileConfigurationSave();		
	}
}

//-----------------------------------------------------------------------------
void SurfacePathManager::removePath(unsigned int i)
{
	paths.remove(i);
	fileConfigurationSave();
}

//-----------------------------------------------------------------------------
void SurfacePathManager::removePaths(const QList<unsigned int>& ids)
{
	foreach(int id,ids)
	{
		paths.remove(id);
	}	
	fileConfigurationSave();
}

//-----------------------------------------------------------------------------
const SurfacePath* SurfacePathManager::getPath(unsigned int id) const
{
	const SurfacePath* r;
	r = &paths[id];
	return r;
}

//-----------------------------------------------------------------------------
QList<const SurfacePath*> SurfacePathManager::getPaths() const
{
	QList<const SurfacePath*> r;
	QHash<unsigned int,SurfacePath>::const_iterator it;
	for(it=paths.constBegin();it!=paths.constEnd();it++)
	{
		r << &it.value();
	}
	return r;
}
	
//-----------------------------------------------------------------------------
void SurfacePathManager::setAttributePath(unsigned int i,unsigned int t,
	const QString& v)
{
	paths[i].setAttribute(t,v);
	fileConfigurationSave();
}

//-----------------------------------------------------------------------------
QString SurfacePathManager::getAttributePath(unsigned int i,unsigned int t)
{
	return paths[i].getAttribute(t);
}

//-----------------------------------------------------------------------------
void SurfacePathManager::fileConfigurationSave()
{
	QString file = fileConfiguration();

	SurfacePathFileConfigData configData;

	QHash<unsigned int,SurfacePath>::iterator it;
	for(it=paths.begin();it!=paths.end();it++)
	{		
		configData.addObject(&it.value());
	}

	SurfacePathFileConfig configFile(&configData);
	configFile.exportData(file);
}

//-----------------------------------------------------------------------------
void SurfacePathManager::fileConfigurationRead()
{
	QString file = fileConfiguration();

	SurfacePathFileConfigData configData;
	SurfacePathFileConfig configFile(&configData);
	 
	configFile.importData(file);

	nId = 0;

	QList<ObjectFileSystem*> list = configData.getObjects();
	foreach(ObjectFileSystem* item,list)
	{
		SurfacePath * value = dynamic_cast<SurfacePath*>(item);			
		if(!paths.contains(value->getId()))
		{
			unsigned int id = value->getId();
			paths.insert(id,*value);
			nId = (nId < id)?id:nId;
		}
		else
		{
			QString str = QObject::tr("Id de pasta duplicado.");
			std::cout << str.toStdString() << std::endl;
			SurfaceException e;
			e.setMessage(str);
			throw e;
		}
		delete value;
	}	
}

//-----------------------------------------------------------------------------
QString SurfacePathManager::fileConfiguration() const
{
	QFile file(GlobalConfig::TERRAIN_LIST_DEFAULT_PATH);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
		file.setFileName(GlobalConfig::TERRAIN_LIST_PATH);
		if (!file.open(QFile::ReadOnly | QFile::Text))
		{	
			QString str = QObject::tr("Arquivo de configuração do diretório de terrenos não localizado.");
			std::cout << str.toStdString() << std::endl;
			SurfaceException e;
			e.setMessage(str);
		} 
		return GlobalConfig::TERRAIN_LIST_DIR_PATH;
    }
	return GlobalConfig::TERRAIN_LIST_DIR_DEFAULT_PATH;
}

//-----------------------------------------------------------------------------
QList<QStringList> SurfacePathManager::filesInfo(unsigned int id) const
{	
	return paths[id].readInfo();
}

//-----------------------------------------------------------------------------
void SurfacePathManager::importData(unsigned int id,const QStringList& list)
{

}