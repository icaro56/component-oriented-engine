#include "SurfacePathFile.h"
#include "persistence\filesystem\DataFileSystem.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QDir>
#include <QFile> 

//-----------------------------------------------------------------------------
SurfacePathFile::SurfacePathFile(DataFileSystem * v)
{
	data = v;
}
	
//-----------------------------------------------------------------------------
SurfacePathFile::SurfacePathFile(const SurfacePathFile& v)
{
	data = v.data;
}

//-----------------------------------------------------------------------------
SurfacePathFile::~SurfacePathFile()
{
	data = 0;
}

//-----------------------------------------------------------------------------
SurfacePathFile& SurfacePathFile::operator = (const SurfacePathFile& v)
{
	if (&v == this)
        return *this; 
	data = v.data;
	return *this; 
}

//-----------------------------------------------------------------------------
void SurfacePathFile::read(const QString& nameFile)
{
	data->clear();
    QFile file(nameFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

	QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;

	if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        return;
    }

	QDomElement root = domDocument.documentElement();

	if (root.tagName() != "view")
    {
        return;
    }

    if (root.hasAttribute("version") &&
            (root.attribute("version") != QString("1.0")))
    {
        return;
    }

	QDomElement child = root.firstChildElement();

    if (child.isNull())
    {
        return;
    }


	file.close();

	/*
	if(child.tagName() == "text")
    {
        QString value = child.attribute("visible");
		DataFileSystemConfig->importVisibleText(value.toInt());
		value = child.attribute("distance");
		DataFileSystemConfig->importDistanceText(value.toDouble());
		value = child.attribute("color");
		QStringList list = value.split(";");
		if(list.size() == 3)
		{			
			int r = list[0].toInt();
			int g = list[1].toInt();
			int b = list[2].toInt();
			QColor c (r,g,b);
			DataFileSystemConfig->importColorText(c);
		}		
    }  
	*/
}

//-----------------------------------------------------------------------------
void SurfacePathFile::exportData(const QString& nameFile)
{
	QFile file(nameFile);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }

    QXmlStreamWriter xw;

    xw.setAutoFormatting(true);

    xw.setDevice(&file);
    xw.setCodec(QObject::tr("ISO 8859-1").toStdString().c_str());

    xw.writeStartDocument();
    xw.writeDTD("<!DOCTYPE surfaces>");
    xw.writeStartElement("surfaces");
    xw.writeAttribute("version", "1.0");

	QList<ObjectFileSystem*> &listObejcts = data->getObjects();
	QList<ObjectFileSystem*>::iterator it;
	for(it=listObejcts.begin();it!=listObejcts.end();it++)
	{
		xw.writeStartElement("surface");
		xw.writeEndElement();
	}

	xw.writeEndElement();

    xw.writeEndDocument();
    file.close();

	/*
	xw.writeStartElement("text");
	QString value = QString::number(DataFileSystemConfig->exportVisibleText());
    xw.writeAttribute("visible", value);
	value = QString::number(DataFileSystemConfig->exportDistanceText());
	xw.writeAttribute("distance", value);
	QColor c = DataFileSystemConfig->exportColorText();
	value = QString::number(c.red()) + ";" + QString::number(c.green()) 
		+ ";" + QString::number(c.blue());
	xw.writeAttribute("color", value);
    xw.writeEndElement();
	*/
}