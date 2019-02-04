#include "SurfaceManager.h"
#include "Surface.h"
#include "SurfaceListerner.h"
#include "SurfacePath.h"
#include "SurfacePathManager.h"
#include "SurfaceData.h"
#include "SurfaceException.h"
#include "SurfaceFileConfig.h"
#include "SurfaceFileConfigData.h"

#include "system\ProjectConfig.h"
#include "pattern\AbstractFactory.h"
#include "persistence\filesystem\KeyValueFileSystem.h"

#include <iostream>

#include <QObject>
#include <QMessageBox>
#include <QDebug>

SurfaceManager* SurfaceManager::instance = 0;

//#define DEBUGSURFACELSS

#ifdef DEBUGSURFACELSS
#include <QDebug>
#endif

//-----------------------------------------------------------------------------
SurfaceManager::SurfaceManager()
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "SurfaceManager";
#endif
	isInit = false;
	isInitListerner = true;
	current = 0;
	listerner = SurfaceListerner::getInstance();	
}

//-----------------------------------------------------------------------------
SurfaceManager::~SurfaceManager()
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "~SurfaceManager";
#endif
}	

//-----------------------------------------------------------------------------
SurfaceManager * SurfaceManager::getInstance()
{
	if(!instance)
	{
		instance = new SurfaceManager();
	}
	return instance;
}

//-----------------------------------------------------------------------------
void SurfaceManager::destroy()
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "destroy";
#endif
	delete instance;
	instance = 0;
}

//-----------------------------------------------------------------------------
void SurfaceManager::init()
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "init";
#endif
	QString file = fileConfiguration();

	SurfaceFileConfigData configData;
	SurfaceFileConfig configFile(&configData);

	configFile.importData(file);

	int cs = configData.getCurrentSurface();
	QList<ObjectFileSystem*> list = configData.getObjects();
	foreach(ObjectFileSystem* item,list)
	{
		SurfaceData * value = dynamic_cast<SurfaceData*>(item);			
		if(!listSurfaceData.contains(value->getId()))
		{
			value->init();	
			listSurfaceData.insert(value->getId(),value);
		}
		else
		{
			QString str = QObject::tr("Id de terreno duplicado.");
			std::cout << str.toStdString() << std::endl;
			SurfaceException e;
			e.setMessage(str);
			throw e;
		}
	}	

	isInit = true;

	if(cs)
	{
		if(loadSurface(cs))
		{
			current = listLoad[cs];
		}		
	}

	pathManager = SurfacePathManager::getInstance();
}

//-----------------------------------------------------------------------------
void SurfaceManager::update()
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "update";
#endif

	if(isInitListerner)
	{
		isInitListerner = false;
		listerner->notify(TypeSurfaceEvent::ChangedListName);
	}
	
	mutex.lock();
	foreach(Surface * t,listLoad)
	{		
		t->update();		
	}
	mutex.unlock();
	
}

//-----------------------------------------------------------------------------
void SurfaceManager::isInitiated() const
{
	if(!isInit)
	{
		QString str = QObject::tr("SurfaceManager não foi iniciado.");
		std::cout << str.toStdString() << std::endl;
		SurfaceException e;
		e.setMessage(str);
		throw e;
	}
}

//-----------------------------------------------------------------------------
QString SurfaceManager::fileConfiguration() const
{
	QFile file(GlobalConfig::TERRAIN_LIST_DEFAULT_PATH);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
		file.setFileName(GlobalConfig::TERRAIN_LIST_PATH);
		if (!file.open(QFile::ReadOnly | QFile::Text))
		{	
			QString str = QObject::tr("Arquivo de configuração do terreno não localizado.");
			std::cout << str.toStdString() << std::endl;
			SurfaceException e;
			e.setMessage(str);
		} 
		return GlobalConfig::TERRAIN_LIST_PATH;
    }
	return GlobalConfig::TERRAIN_LIST_DEFAULT_PATH;
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
bool SurfaceManager::hasSurfaceCurrent() const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "hasSurfaceCurrent";
#endif
	return (current)?true:false;
}

//-----------------------------------------------------------------------------
Surface * SurfaceManager::getSurface(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getSurface";
#endif
	isInitiated();
	Surface * r = (!idSurface)?current:listLoad[idSurface];
	return r;
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
const QHash<int , const SurfaceData*>  SurfaceManager::getDataInfo() const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getDataInfo";
#endif
	isInitiated();

	QHash<int ,const SurfaceData*> r;

	QHash<int , SurfaceData*>::const_iterator it;
	for(it=listSurfaceData.constBegin();it!=listSurfaceData.constEnd();it++)
	{
		r.insert(it.key(),it.value());
	}

	return r;
}

//-----------------------------------------------------------------------------
unsigned int SurfaceManager::idCurrentSurface() const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "nameCurrentSurface";
#endif
	isInitiated();
	if(!hasSurfaceCurrent())
	{
		return 0;
	}
	return current->getId();
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void SurfaceManager::addSurface(const QString& name,const QString& file)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "addSurface";
#endif
	isInitiated();
	//falta
}

//-----------------------------------------------------------------------------
void SurfaceManager::removeSurface(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "removeSurface";
#endif
	isInitiated();
	bool cond = false;
	if(listLoad.contains(idSurface))
	{
		Surface * aux = listLoad[idSurface];		
		current = (current == aux)?0:aux;
		unLoadSurface(idSurface);	
		cond = true;
	}	

	SurfaceData * data = listSurfaceData[idSurface];
	listSurfaceData.remove(idSurface);
	delete data;
	fileConfigurationSave();	
	listerner->notify(TypeSurfaceEvent::ChangedLoad);	
	fileConfigurationDelete(idSurface);
}

//-----------------------------------------------------------------------------
void SurfaceManager::exportSurface(const QString& id,const QString& path)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "exportSurface";
#endif
	isInitiated();

	SurfacePath::writeInfo(path,listSurfaceData[id.toInt()]);
}

//-----------------------------------------------------------------------------
void SurfaceManager::importSurface(unsigned int path,const QStringList& list)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "importSurface";
#endif
	isInitiated();

	pathManager->importData(path,list);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::loadSurface(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "loadSurface";
#endif

	int n = 8193;

#ifndef _WIN64 
	n = 4097;
#endif

	bool cond = true;

	int * aux = 0;

	try
	{
		aux = new int[n*n];
	}
	catch(...)
	{	
		delete aux;
		int ret = QMessageBox::warning(0,
				QObject::tr("Memória Insuficiente"),
            QObject::tr("O terreno não foi carregado por não ter \nmemória suficiente para suporta a operação."));
		return false;
	}

	delete aux;
	// talvez melhor usar delete [] aux;

	mutex.lock();
	if(listSurfaceData.contains(idSurface) && !listLoad.contains(idSurface))
	{	
		Surface *t = 0;
		AbstractFactory *fac = AbstractFactory::getFactory();
		t = fac->createSurface();	
		*t = (*listSurfaceData[idSurface]);

		try
		{	 			
			t->init();	
		}
		catch (SurfaceException& e)
		{		
			cond = false;
			int ret = QMessageBox::warning(0,
				QObject::tr("Falha na leitura de arquivos"),
            QObject::tr("Arquivos básicos de terrenos inexistentes. \nIdentificador do terreno falho: ") 
			+ QString::number(t->getId()));

			delete t;
			t = 0;
		}

		if(t)
		{
			listLoad.insert(idSurface,t);
		}
	}
	else
	{
		cond = false;
	}
	mutex.unlock();

	return cond;
}

//-----------------------------------------------------------------------------
void SurfaceManager::unLoadSurface(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "unLoadSurface";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(surface)
	{
		mutex.lock();
		listLoad.remove(idSurface);
		delete surface;
		mutex.unlock();
	}

}
	
//-----------------------------------------------------------------------------
void SurfaceManager::swapSurface(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "swapSurface";
#endif
	isInitiated();

	int aux = 0;

	if(current)
	{
		aux = current->getId();
		unLoadSurface(current->getId());
		current = 0;
	}

	if(loadSurface(idSurface))
	{
		current = listLoad[idSurface];
		fileConfigurationSave();
		listerner->notify(TypeSurfaceEvent::ChangedLoad);
	}
	else
	{
		if(loadSurface(aux))
		{
			current = listLoad[aux];
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
double SurfaceManager::getHeightAt( const double& vX, const double& vZ, const double& rBonus , 
	bool bUseRoot , bool bUseGeoMorphing , 
	bool onlyVisible , unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getHeightAt";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return 0;
	}
	return surface->getHeightAt( vX, vZ, rBonus, bUseRoot, bUseGeoMorphing, 
	 onlyVisible);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::getHeightAt(Vector3& vPos, const double& rBonus , 
	bool bUseRoot , bool bUseGeoMorphing , 
	bool onlyVisible , unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getHeightAt";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}
	return surface->getHeightAt(vPos, rBonus, bUseRoot, bUseGeoMorphing, 
	onlyVisible);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::mousePosition(int mx, int my,double& x, double& y, 
									double & z,unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "mousePosition";
#endif	
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}
	return surface->mousePosition(mx,my,x,y,z);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::isSurfaceCollision(Vector3& oldPos, const double& height, 
						Vector3 &newPos, 
						int mask ,
						bool onlyVisible , unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "isSurfaceCollision";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}
	return surface->isSurfaceCollision(oldPos, height, newPos, mask, onlyVisible);
}
		
//-----------------------------------------------------------------------------
Vector3 SurfaceManager::mouseWorldPosition(const double& xMouse, const double& yMouse, 
	const double& wWindow, const double& hWindow, unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "mouseWorldPosition";
#endif

	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return Vector3(0,0,0);
	}
	return surface->mouseWorldPosition(xMouse, yMouse, wWindow, hWindow);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::thisVisible(const Vector3& v,const QString& name,
	unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "thisVisible";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}
	return surface->thisVisible(v, name);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::isMine(int x,const double& d,unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "isMine";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}
	return surface->isMine(x, d);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::inside(const Vector3& point,unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "inside";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}
	return surface->inside(point);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::isInside(Vector3& worldPosition,unsigned int idSurface)
{
	#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "surfaceBounds";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}

	return surface->isInside(worldPosition);
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
double SurfaceManager::getOpacity(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getOpacity";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return 0;
	}
	return surface->getOpacity();
}
	
//-----------------------------------------------------------------------------
void SurfaceManager::setOpacity(const double& alpha, unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "setOpacity";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	surface->setOpacity(alpha);
}

//----------------------------------------------------------------------------
void SurfaceManager::setLod( int l,unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "setLod";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	surface->setLod(l);
}
//-----------------------------------------------------------------------------

int SurfaceManager::getLod(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getLod";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return 0;
	}
	return surface->getLod();
}
//------------------------------------------------------------------------------

void SurfaceManager::configureSurfaceMaterial(bool isColor, const QColor& color,
	unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "configureSurfaceMaterial";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	surface->configureSurfaceMaterial(isColor, color);
}

//-----------------------------------------------------------------------------
void SurfaceManager::configureSurfaceColor(const QColor& color, unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "configureSurfaceColor";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	surface->configureSurfaceColor(color);
}

//-----------------------------------------------------------------------------
void SurfaceManager::setWireFrame(bool actived, unsigned int idSurface)
{
	isInitiated();

	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}

	surface->setWireFrame(actived);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::isWireFrame(unsigned int idSurface)
{
	isInitiated();

	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}

	return surface->isWireFrame();
}

//-----------------------------------------------------------------------------
void SurfaceManager::setSolidFrame(bool actived, unsigned int idSurface)
{
	isInitiated();

	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}

	surface->setSolidFrame(actived);
}

//-----------------------------------------------------------------------------
bool SurfaceManager::isSolidFrame(unsigned int idSurface)
{
	isInitiated();

	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return false;
	}

	return surface->isSolidFrame();
}

void SurfaceManager::configureEffectiveMining(unsigned int idSurface)
{
	isInitiated();

	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}

	surface->configureEffectiveMining();
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
double SurfaceManager::kRealTo3D(unsigned int idSurface) const
{
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return 1;
	}
	return surface->kRealTo3D();
}

//-----------------------------------------------------------------------------
double SurfaceManager::k3DToReal(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "k3DToReal";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return 1;
	}
	return surface->k3DToReal();
}

//-----------------------------------------------------------------------------
void SurfaceManager::convertRealTo3D(Vector3& point,
	unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "convertRealTo3D";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	return surface->convertRealTo3D(point);
}

//-----------------------------------------------------------------------------
void SurfaceManager::convertRealTo3D(double& x,double& y,double& z,
	unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "convertRealTo3D";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	return surface->convertRealTo3D(x, y, z);
}

//-----------------------------------------------------------------------------
void SurfaceManager::convert3DToReal(Vector3& point,
	unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "convert3DToReal";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	return surface->convert3DToReal(point);
}
	
//-----------------------------------------------------------------------------
void SurfaceManager::convert3DToReal(double& x,double& y,double& z,
	unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "convert3DToReal";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		return;
	}
	return surface->convert3DToReal(x,y,z);
}

//-----------------------------------------------------------------------------
QList<double>  SurfaceManager::getDimensionReal(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getDimensionReal";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		QList<double> l;
		l << 1;
		l << 1;
		l << 1;
		l << 1;
		l << 1;
		l << 1;
		return l;
	}
	return surface->getDimensionReal();
}

//-----------------------------------------------------------------------------
QList<double>  SurfaceManager::getDimension3D(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getDimension3D";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{
		QList<double> l;
		l << 1;
		l << 1;
		l << 1;
		l << 1;
		l << 1;
		l << 1;
		return l;
	}
	return surface->getDimension3D();
}
    
//-----------------------------------------------------------------------------
double SurfaceManager::getWidth(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getWidth";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{		
		return 0;
	}
	return surface->getWidth();
}

//-----------------------------------------------------------------------------
double SurfaceManager::getSurfaceHeight(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getSurfaceHeight";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{		
		return 0;
	}
	return surface->getHeight();
}

//-----------------------------------------------------------------------------
Vector3 SurfaceManager::getCenterPosition(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getCenterPosition";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{		
		return Vector3(0,0,0);
	}
	return surface->getCenterPosition();
}
		
//-----------------------------------------------------------------------------
double SurfaceManager::getDiffXReal(unsigned int idSurface) const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getDiffXReal";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{		
		return 0;
	}
	return surface->getDiffXReal();
}

HEIGHTMAPTYPE* SurfaceManager::getHeightMapData(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getDiffXReal";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{		
		return nullptr;
	}

	return surface->getHeightMapData();
}

size_t SurfaceManager::getHeightMapWidth(unsigned int idSurface)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getDiffXReal";
#endif
	isInitiated();
	Surface * surface = getSurface(idSurface);
	if(!surface)
	{		
		return 0;
	}

	return surface->getHeightMapWidth();
}

//-----------------------------------------------------------------------------
void SurfaceManager::fileConfigurationSave() const
{
	QString file = fileConfiguration();

	SurfaceFileConfigData configData;

	foreach(SurfaceData* obj,listSurfaceData)
	{
		configData.addObject(obj);
	}	

	configData.setCurrentSurface(0);
	if(current)
	{
		configData.setCurrentSurface(current->getId());
	}

	SurfaceFileConfig configFile(&configData);
	configFile.exportData(file);
}

//-----------------------------------------------------------------------------
void SurfaceManager::fileConfigurationDelete(unsigned int idSurface) const
{
	QFile::remove(GlobalConfig::TERRAIN_FOLDER + QString::number(idSurface) 
								+ ".dll");

	QFile::remove(GlobalConfig::TERRAIN_FOLDER + QString::number(idSurface) 
								+ ".zip");

}

//-----------------------------------------------------------------------------
void SurfaceManager::setAttributeDataSurface(unsigned int idSurface,unsigned int i,
		const QString& v)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "setAttributeDataSurface";
#endif
	isInitiated();
	if(listSurfaceData.contains(idSurface))
	{
		SurfaceData * sd = listSurfaceData[idSurface];
		bool cond = true;
		switch(i)
		{
			case TSDE_Name:
			{
				sd->setName(v);
			}
			break;
			case TSDE_Descripition:
			{
				sd->setDescription(v);
			}
			break;
			default:
			{
				cond = false;
			}
			break;
		}

		if(cond)
		{
			fileConfigurationSave();
		}
	}
}

//-----------------------------------------------------------------------------
QString SurfaceManager::getAttributeDataSurface(unsigned int idSurface,
	unsigned int i)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getAttributeDataSurface";
#endif
	isInitiated();

	QString r = "";

	if(listSurfaceData.contains(idSurface))
	{
		SurfaceData * sd = listSurfaceData[idSurface];
		switch(i)
		{
			case TSDE_Name:
			{
				return sd->getName();
			}
			break;
			case TSDE_Descripition:
			{
				return sd->getDescription();
			}
			break;
		}
	}
	
	return r;
}

//-----------------------------------------------------------------------------
QList<const SurfacePath*> SurfaceManager::getPathsSurface() const
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "getPathsSurface";
#endif
	isInitiated();

	return pathManager->getPaths();
}

//-----------------------------------------------------------------------------
void SurfaceManager::addPathSurface(const SurfacePath& path)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "addPathSurface";
#endif
	isInitiated();

	pathManager->addPath(path);
}

//-----------------------------------------------------------------------------
void SurfaceManager::removePathSurface(const QList<unsigned int>& list)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "removePathSurface";
#endif
	isInitiated();

	pathManager->removePaths(list);
}

//-----------------------------------------------------------------------------
void SurfaceManager::editPathSurface(const SurfacePath& path)
{
#ifdef DEBUGSURFACELSS
	qDebug() << "---------------" << "editPathSurface";
#endif
	isInitiated();

	pathManager->editPath(path);
}


