#include "ConfigTerrainControl.h"
//#include "moc_ConfigTerrainControl.cpp"

#include "ConfigTerrain.h"
#include "ConfigTerrainDir.h"
#include "ConfigTerrainFtp.h"
#include "system/ProjectConfig.h"
#include "surface/SurfaceManager.h"
#include "surface/SurfaceData.h"
#include "surface/SurfacePath.h"
#include "surface/SurfacePathManager.h"
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QDebug>

ConfigTerrainControl* ConfigTerrainControl::instance = 0;

//-----------------------------------------------------------------------------
ConfigTerrainControl::ConfigTerrainControl()
	: UiBasic(Ui_ConfigTerrain),
	configTerrain(0),
	configTerrainDir(0),
	configTerrainFtp(0)
{	
}

//-----------------------------------------------------------------------------
ConfigTerrainControl::~ConfigTerrainControl()
{
	if (configTerrain)
	{
		delete configTerrain;
		configTerrain =0;
	}

	if (configTerrainDir)
	{
		delete configTerrainDir;
		configTerrainDir =0;
	}

	if (configTerrainFtp)
	{
		delete configTerrainFtp;
		configTerrainFtp =0;
	}	
}

//-----------------------------------------------------------------------------
ConfigTerrainControl* ConfigTerrainControl::getInstance()
{
	if (!instance)
	{
		instance = new ConfigTerrainControl();
	}

	return instance;
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::destroy()
{
	if (instance)
	{
		delete instance;
		instance = 0;
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::addActionsInMenu(QMenu* menu)
{	
	if(menu)
	{
		configTerrain = new ConfigTerrain((QWidget*)menu->parent());
		
		configTerrainDir = new ConfigTerrainDir(configTerrain);
		
		configTerrainFtp = new ConfigTerrainFtp(configTerrain);	
		
		actVisible = new QAction(QObject::tr("Controle de &Atualização de Terreno"),menu);
		actVisible->setIcon(QIcon(GlobalConfig::ICONPATH + "ico_atualizacao_de_terreno.png"));
		actVisible->setCheckable(true);
		actVisible->setChecked(false);
		actVisible->setStatusTip(QObject::tr("Ativa/desativa o Controle de Atualização de Terreno"));

		menu->addAction(actVisible);

		connects();		
	}	
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::addActionsInToolbar(QToolBar* toolBar)
{
	if (toolBar)
	{
		toolBar->addAction(actVisible);
	}			
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::setEnabledUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	if(!cond)
	{
		configTerrain->close();
	}
	actVisible->setEnabled(cond);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::setVisibleUi(bool cond)
{	
	if(!cond)
	{
		configTerrain->close();
	}
	actVisible->setVisible(cond);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::setCheckedUi(bool cond)
{
	if(!actVisible->isVisible()) return;

	actVisible->setChecked(cond);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::connects()
{
	connect(actVisible, SIGNAL(toggled(bool)), 
		configTerrain, SLOT(setVisible(bool)) );	

	//-----

	connect(configTerrain, SIGNAL(visibilityChanged(bool)), 
		actVisible, SLOT(setChecked(bool)) );	

	connect
	(
		configTerrain, SIGNAL(showFtp(bool)), 
		this, SLOT(showFtp(bool))
	);

	connect
	(
		configTerrain, SIGNAL(showDir(bool)), 
		this, SLOT(showDir(bool))
	);

	connect
	(
		configTerrain, SIGNAL(removeTerrain(const QStringList &)), 
		this, SLOT(removeTerrain(const QStringList &)) 
	);

	connect
	(
		configTerrain, SIGNAL(exportTerrain(const QStringList &)), 
		this, SLOT(exportTerrain(const QStringList &)) 
	);

	connect
	(
		configTerrain, SIGNAL(loadTerrain(const QStringList &)), 
		this, SLOT(loadTerrain(const QStringList &)) 
	);

	connect
	(
		configTerrain, SIGNAL(nameChanged(int,const QString&)), 
		this, SLOT(nameChangedTerrain(int,const QString&)) 
	);

	connect
	(
		configTerrain, SIGNAL(descriptionChanged(int,const QString&)), 
		this, SLOT(descriptionChangedTerrain(int,const QString&)) 
	);

	//-----

	connect
	(
		configTerrainDir, SIGNAL(visibilityChanged(bool)), 
		this, SLOT(visibilityConfigTerrain(bool))
	);	

	connect
	(
		configTerrainDir, SIGNAL(addPath(const QStringList&)), 
		this, SLOT(addPath(const QStringList&))
	);	

	connect
	(
		configTerrainDir, SIGNAL(editPath(const QStringList&)), 
		this, SLOT(editPath(const QStringList&))
	);	

	connect
	(
		configTerrainDir, SIGNAL(removePath(const QStringList&)), 
		this, SLOT(removePath(const QStringList&))
	);

	connect
	(
		configTerrainDir, SIGNAL(currentPathObtain(int)), 
		this, SLOT(currentPathObtain(int))
	);

	connect
	(
		configTerrainDir, SIGNAL(download(int,const QStringList&)), 
		this, SLOT(download(int,const QStringList&))
	);
	

	//-----

	connect
	(
		configTerrainFtp, SIGNAL(visibilityChanged(bool)), 
		this, SLOT(visibilityConfigTerrain(bool))
	);	

}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::changedListName()
{
	QHash<int , QStringList > list;
	SurfaceManager * sm = SurfaceManager::getInstance();
	const QHash<int ,const SurfaceData*> aux = sm->getDataInfo();
	QHash<int , const SurfaceData*>::const_iterator it;
	for(it=aux.constBegin();it!=aux.constEnd();it++)
	{
		const SurfaceData * sd = it.value();
		QStringList d;
		d << "";
		d << sd->getName();			
		d << QDateTime::fromTime_t(sd->getRegistrationDate()).toString("dd/MM/yyyy - hh:mm:ss");
		d << sd->getDescription();			
		d << sd->getCreationDate();
		d << QString::number(sd->getId());
		list.insert(sd->getId(),d);
	}

	int c = sm->idCurrentSurface();

	configTerrain->setDataInfo(QString::number(c),list);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::changedLoad()
{
	QHash<int , QStringList > list;
	SurfaceManager * sm = SurfaceManager::getInstance();
	const QHash<int ,const SurfaceData*> aux = sm->getDataInfo();
	QHash<int ,const SurfaceData*>::const_iterator it;
	for(it=aux.constBegin();it!=aux.constEnd();it++)
	{
		const SurfaceData * sd = it.value();
		QStringList d;
		d << "";
		d << sd->getName();				
		d << QDateTime::fromTime_t(sd->getRegistrationDate()).toString("dd/MM/yyyy - hh:mm:ss");
		d << sd->getDescription();			
		d << sd->getCreationDate();
		d << QString::number(sd->getId());
		list.insert(sd->getId(),d);
	}

	int c = sm->idCurrentSurface();

	configTerrain->setDataInfo(QString::number(c),list);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::removeTerrain(const QStringList & idList)
{
	int s = idList.size();
	if(s)
	{
		SurfaceManager * sm = SurfaceManager::getInstance();
		foreach(QString id,idList)
		{
			sm->removeSurface(id.toInt());
		}
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::exportTerrain(const QStringList & idList)
{
	int s = idList.size();
	if(s)
	{
		QString dir = QFileDialog::getExistingDirectory(configTerrain, tr("Exportar terreno"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
		if(dir.size())
		{
			SurfaceManager * sm = SurfaceManager::getInstance();
			foreach(QString id,idList)
			{
				sm->exportSurface(id,dir);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::loadTerrain(const QStringList & idList)
{
	int s = idList.size();
	if(s)
	{
		SurfaceManager * sm = SurfaceManager::getInstance();
		if(s == 1)
		{
			sm->swapSurface(idList[0].toInt());
		}
		else
		{
			foreach(QString id,idList)
			{
				sm->loadSurface(id.toInt());
			}
		}
	}	
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::showFtp(bool cond)
{
	configTerrainFtp->show();
	configTerrain->setVisible(false);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::showDir(bool cond)
{
	QStringList l;
	l << "";
	l << tr("FTP");
	l << tr("Diretório");	
	configTerrainDir->setTypes(l);

	setDataConfigDir();

	configTerrainDir->show();
	configTerrain->setVisible(false);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::visibilityConfigTerrain(bool cond)
{	
	configTerrain->setVisible(!cond);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::nameChangedTerrain(int id,const QString& v)
{
	SurfaceManager * sm = SurfaceManager::getInstance();
	sm->setAttributeDataSurface(id,TSDE_Name,v);
}
	
//-----------------------------------------------------------------------------
void ConfigTerrainControl::descriptionChangedTerrain(int id,const QString& v)
{
	SurfaceManager * sm = SurfaceManager::getInstance();
	sm->setAttributeDataSurface(id,TSDE_Descripition,v);
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::addPath(const QStringList& list)
{
	SurfaceManager * sm = SurfaceManager::getInstance();
	SurfacePath path;

	int type =  SurfacePath::getType(list[list.size()-1]);
	path.setType(type);

	switch(type)
	{
		case TSP_Dir:	
		{			
			path.setAttribute(TSPA_Path,list[0]);
			path.setAttribute(TSPA_TimeMonitoring,list[1]);
		}
		break;
		case TSP_FTP:
		{			
			path.setAttribute(TSPA_Path,list[0]);
			path.setAttribute(TSPA_Dir,list[1]);
			path.setAttribute(TSPA_Port,list[2]);
			path.setAttribute(TSPA_UserName,list[3]);
			path.setAttribute(TSPA_Password,list[4]);			
			path.setAttribute(TSPA_TimeMonitoring,list[5]);		
		}
		break;
	}
	sm->addPathSurface(path);

	setDataConfigDir();
}
	
//-----------------------------------------------------------------------------
void ConfigTerrainControl::editPath(const QStringList& list)
{		
	int id = list[list.size()-1].toInt();
	int type = list[list.size()-2].toInt();

	SurfacePath path;
	path.setId(id);
	path.setType(type);

	switch(type)
	{
		case TSP_Dir:	
		{			
			path.setAttribute(TSPA_Path,list[0]);
			path.setAttribute(TSPA_TimeMonitoring,list[1]);
		}
		break;
		case TSP_FTP:
		{			
			path.setAttribute(TSPA_Path,list[0]);
			path.setAttribute(TSPA_Dir,list[1]);
			path.setAttribute(TSPA_Port,list[2]);
			path.setAttribute(TSPA_UserName,list[3]);
			path.setAttribute(TSPA_Password,list[4]);			
			path.setAttribute(TSPA_TimeMonitoring,list[5]);		
		}
		break;
	}

	SurfaceManager * sm = SurfaceManager::getInstance();
	sm->editPathSurface(path);	

	setDataConfigDir();
}
	
//-----------------------------------------------------------------------------
void ConfigTerrainControl::removePath(const QStringList& list)
{
	SurfaceManager * sm = SurfaceManager::getInstance();
	QList<unsigned int> aux;
	foreach(QString v,list)
	{
		aux << v.toInt();
	}

	sm->removePathSurface(aux);

	setDataConfigDir();
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::setDataConfigDir()
{
	QList<QStringList> list;

	SurfaceManager * sm = SurfaceManager::getInstance();
	QList<const SurfacePath*> paths = sm->getPathsSurface();
	foreach(const SurfacePath* sp,paths)
	{
		QStringList aux;
		switch(sp->getType())
		{
			case TSP_Dir:	
			{
				aux << sp->getAttribute(TSPA_Path);
				aux << sp->getAttribute(TSPA_TimeMonitoring);
				aux << QString::number(sp->getType());
				aux << QString::number(sp->getId());
			}
			break;
			case TSP_FTP:
			{
				aux << sp->getAttribute(TSPA_Path);
				aux << sp->getAttribute(TSPA_Dir);
				aux << sp->getAttribute(TSPA_Port);
				aux << sp->getAttribute(TSPA_UserName);
				aux << sp->getAttribute(TSPA_Password);
				aux << sp->getAttribute(TSPA_TimeMonitoring);
				aux << QString::number(sp->getType());
				aux << QString::number(sp->getId());
			}
			break;
		}
		list << aux;
	}

	if(configTerrainDir)
	{
		configTerrainDir->setPaths(list);
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::currentPathObtain(int id)
{
	SurfacePathManager * spm = SurfacePathManager::getInstance();
	if(configTerrainDir)
	{
		configTerrainDir->setInfoObtain(spm->filesInfo(id));
	}
}

//-----------------------------------------------------------------------------
void ConfigTerrainControl::download(int id,const QStringList& list)
{
	SurfacePathManager * spm = SurfacePathManager::getInstance();
	spm->importData(id,list);
}
