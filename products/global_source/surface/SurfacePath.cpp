#include "SurfacePath.h"
#include "SurfaceData.h"
//#include "moc_SurfacePath.cpp"

#include <QDateTime>
#include <QDir>

#include "system\ProjectConfig.h"
#include "persistence\filesystem\KeyValueFileSystem.h"

const QString SurfacePath::FileTerrainFormat = "terrain_*.ter";
const QString SurfacePath::FileTerrainInfoFormat = "terrain_*.inf";

//-----------------------------------------------------------------------------
SurfacePath::SurfacePath()
{
	id = 0;
	type = 0;
	connect(&timer, SIGNAL(timeout()), this, SLOT(scanFiles()) );
}

//-----------------------------------------------------------------------------
SurfacePath::~SurfacePath()
{
	timer.stop();
}

//-----------------------------------------------------------------------------
SurfacePath::SurfacePath(const SurfacePath& v)
{
	id = v.id;
	type = v.type;
	attributes = v.attributes;
}

//-----------------------------------------------------------------------------
SurfacePath& SurfacePath::operator = (const SurfacePath& v)
{
	if (&v == this)
        return *this; 

	id = v.id;
	type = v.type;
	attributes = v.attributes;

	return *this; 
}

//-----------------------------------------------------------------------------
unsigned int SurfacePath::getId() const
{
	return id;
}

//-----------------------------------------------------------------------------
void SurfacePath::setId(unsigned int v)
{
	id = v;
}

//-----------------------------------------------------------------------------
unsigned int SurfacePath::getType() const
{
	return type;
}

//-----------------------------------------------------------------------------
void SurfacePath::setType(unsigned int v)
{
	type = v;
}

//-----------------------------------------------------------------------------
QString SurfacePath::getTypeName(unsigned int v)
{
	QString r = "";
	switch(v)
	{
		case TSP_Dir:
		{
			r = QObject::tr("Diretório");
		}
		break;
		case TSP_FTP:
		{
			r = QObject::tr("FTP");
		}
		break;
	}
	
	return r;
}

//-----------------------------------------------------------------------------
unsigned int SurfacePath::getType(const QString& v)
{
	unsigned int r = 0;
	QString aux = v.toUpper();
	if(aux == QObject::tr("Diretório").toUpper())
	{
		r = TSP_Dir;
	}
	else if(aux == QObject::tr("FTP").toUpper())
	{
		r = TSP_FTP;
	}
	return r;
}

//-----------------------------------------------------------------------------
void SurfacePath::setAttribute(unsigned int i,const QString& v)
{
	attributes.insert(i,v);
}

//-----------------------------------------------------------------------------
QString SurfacePath::getAttribute(unsigned int i) const
{
	QString r = "";
	if(attributes.contains(i))
	{
		r = attributes[i];
	}
	return r;
}

//-----------------------------------------------------------------------------
bool SurfacePath::initTimer()
{
	QString aux = attributes[TSPA_LastTime];
	if(aux.size())
	{
		QDateTime t1 = QDateTime::fromTime_t(aux.toInt());		
		QDateTime t2 = t1.addDays(attributes[TSPA_TimeMonitoring].toInt());
		if(t2.date() <= QDateTime::currentDateTime().date())
		{
			QString aux = QString::number(QDateTime::currentDateTime().toTime_t());
			attributes.insert(TSPA_LastTime,aux);
			timer.stop();
			timer.setInterval(3600000);
			timer.start();
			return true;
		}

	}
	return false;
}

//-----------------------------------------------------------------------------
void SurfacePath::scanFiles()
{	
}

//-----------------------------------------------------------------------------
QList<QStringList> SurfacePath::readInfo() const
{	
	QList<QStringList> r;
	switch(type)
	{
		case TSP_Dir:
		{
			QString dirName = attributes[TSPA_Path];
			QDir dir(dirName);
			QStringList list;
			list << FileTerrainInfoFormat;	
			list = dir.entryList(list);
			foreach(QString v,list)
			{
				KeyValueFileSystem fileInf(dir.path() + "//" +  v,"=");
				fileInf.read();
				QStringList aux;
				aux << fileInf.getAttribute(SurfaceData::Name);
				aux << fileInf.getAttribute(SurfaceData::CreationDate);
				int rd = fileInf.getAttribute(SurfaceData::RegistrationDate).toInt();
				aux << QDateTime::fromTime_t(rd).toString("dd/MM/yyyy - hh:mm:ss");
				aux << fileInf.getAttribute(SurfaceData::Description);
				QString id = v.remove("terrain_");
				id = id.remove(".inf");
				aux << id;
				r << aux;
			}
		}
		break;
		case TSP_FTP:
		{
		}
		break;
	}

	return r;
}

//-----------------------------------------------------------------------------
void SurfacePath::writeInfo(const QString& path,const SurfaceData* data)
{
	QDir dir(path);

	QStringList list;
	list << SurfacePath::FileTerrainInfoFormat;	
	int n = 0;
	list = dir.entryList(list);
	foreach(QString name,list)
	{		
		name = name.remove("terrain_");
		name = name.remove(".inf");
		bool cond = false;
		int v = name.toInt(&cond);
		if(cond && v > 0)
		{
			n = (n < v)?v:n;
		}
	}	

	QString aux = SurfacePath::FileTerrainInfoFormat;
	aux = aux.replace("*",QString::number(n+1));
	QString fileInfoTerrain(path);
	fileInfoTerrain.append("//");
	fileInfoTerrain.append(aux);

	KeyValueFileSystem fileInf(fileInfoTerrain,"=");

	QMap<QString,QString> kv = data->getAtributes();
	QMap<QString,QString>::iterator it;
	for(it=kv.begin();it!=kv.end();it++)
	{
		fileInf.setAttribute(it.key(),it.value());
	}   

	fileInf.write();

	QString file(GlobalConfig::TERRAIN_FOLDER);
	file.append(data->getId());
    file.append(".dll");

	aux = SurfacePath::FileTerrainFormat;
	aux = aux.replace("*",QString::number(n+1));
	QString fileTerrain(path);
	fileTerrain.append("//");
	fileTerrain.append(aux);

	QFile::copy(file,fileTerrain);
}

///////////////////////////////////////////////////////////////////////////////


