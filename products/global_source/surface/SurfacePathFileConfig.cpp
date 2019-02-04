#include "SurfacePathFileConfig.h"
#include "SurfacePathFileConfigData.h"
#include "SurfacePath.h"
#include "persistence\filesystem\DataFileSystem.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QDir>
#include <QFile> 

//-----------------------------------------------------------------------------
SurfacePathFileConfig::SurfacePathFileConfig(DataFileSystem * v)
{
	data = v;
}
	
//-----------------------------------------------------------------------------
SurfacePathFileConfig::SurfacePathFileConfig(const SurfacePathFileConfig& v)
{
	data = v.data;
}

//-----------------------------------------------------------------------------
SurfacePathFileConfig::~SurfacePathFileConfig()
{
	data = 0;
}

//-----------------------------------------------------------------------------
SurfacePathFileConfig& SurfacePathFileConfig::operator = (const SurfacePathFileConfig& v)
{
	if (&v == this)
        return *this; 
	data = v.data;
	return *this; 
}

//-----------------------------------------------------------------------------
void SurfacePathFileConfig::read(const QString& nameFile)
{
	data->clear();
    QFile file(nameFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

	
	SurfacePathFileConfigData * tfcd = dynamic_cast<SurfacePathFileConfigData*>(data);

	QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;

	if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        return;
    }

	QDomElement root = domDocument.documentElement();

	if (root.tagName() != "paths")
    {
        return;
    }

    if (root.hasAttribute("version") &&
            (root.attribute("version") != QString("1.0")))
    {
        return;
    }

	QDomElement path = root.firstChildElement();

	if(path.tagName() == "path")
    {		
		while(!path.isNull())
		{
			int id = path.attribute("id").toInt();
			int type = path.attribute("type").toInt();

			QString value = (path.hasAttribute("value"))?path.attribute("value"):"";
			QString dir = (path.hasAttribute("dir"))?path.attribute("dir"):"";
			QString timeMonitoring = (path.hasAttribute("timeMonitoring"))?path.attribute("timeMonitoring"):"";
			QString port = (path.hasAttribute("port"))?path.attribute("port"):"";
			QString username = (path.hasAttribute("username"))?path.attribute("username"):"";
			QString password = (path.hasAttribute("password"))?path.attribute("password"):"";
			QString lastTime = (path.hasAttribute("lastTime"))?path.attribute("lastTime"):"";
			
			SurfacePath * newData = new SurfacePath();
			newData->setId(id);
			newData->setType(type);
			newData->setAttribute(TSPA_Path,value);
			newData->setAttribute(TSPA_UserName,username);
			newData->setAttribute(TSPA_Password,password);
			newData->setAttribute(TSPA_Port,port);
			newData->setAttribute(TSPA_Dir,dir);
			newData->setAttribute(TSPA_TimeMonitoring,timeMonitoring);
			newData->setAttribute(TSPA_LastTime,lastTime);
			
			tfcd->addObject(newData);

			path = path.nextSiblingElement();
		}
	} 

	file.close();

}

//-----------------------------------------------------------------------------
void SurfacePathFileConfig::exportData(const QString& nameFile)
{
	QFile file(nameFile);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }

	SurfacePathFileConfigData * tfcd = dynamic_cast<SurfacePathFileConfigData*>(data);
	
    QXmlStreamWriter xw;

    xw.setAutoFormatting(true);

    xw.setDevice(&file);
    xw.setCodec(QObject::tr("ISO 8859-1").toStdString().c_str());

	xw.writeStartDocument();
    xw.writeDTD("<!DOCTYPE paths>");
    xw.writeStartElement("paths");
	xw.writeAttribute("version", "1.0");

	QList<ObjectFileSystem*> listObejcts = tfcd->getObjects();
	QList<ObjectFileSystem*>::iterator it;

	for(it=listObejcts.begin();it!=listObejcts.end();it++)
	{
		SurfacePath * sp = dynamic_cast<SurfacePath*>(*it);

		xw.writeStartElement("path");
		switch(sp->getType())
		{
			case TSP_Dir:	
			{
				xw.writeAttribute("id", QString::number(sp->getId()));
				xw.writeAttribute("type", QString::number(sp->getType()));
				xw.writeAttribute("value", sp->getAttribute(TSPA_Path));
				xw.writeAttribute("timeMonitoring", sp->getAttribute(TSPA_TimeMonitoring));
				xw.writeAttribute("lastTime", sp->getAttribute(TSPA_LastTime));
			}
			break;
			case TSP_FTP:
			{
				xw.writeAttribute("id", QString::number(sp->getId()));
				xw.writeAttribute("type", QString::number(sp->getType()));
				xw.writeAttribute("value", sp->getAttribute(TSPA_Path));
				xw.writeAttribute("dir", sp->getAttribute(TSPA_Dir));
				xw.writeAttribute("port", sp->getAttribute(TSPA_Port));
				xw.writeAttribute("username", sp->getAttribute(TSPA_UserName));
				xw.writeAttribute("password", sp->getAttribute(TSPA_Password));
				xw.writeAttribute("timeMonitoring", sp->getAttribute(TSPA_TimeMonitoring));
				xw.writeAttribute("lastTime", sp->getAttribute(TSPA_LastTime));
			}
			break;
		}
		xw.writeEndElement();
	}
	
	xw.writeEndElement();

	xw.writeEndDocument();
	
    file.close();
}