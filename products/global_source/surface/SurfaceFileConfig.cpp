#include "SurfaceFileConfig.h"
#include "SurfaceFileConfigData.h"
#include "SurfaceData.h"
#include "persistence\filesystem\DataFileSystem.h"

#include <QXmlStreamReader>
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QDir>
#include <QFile> 

//-----------------------------------------------------------------------------
SurfaceFileConfig::SurfaceFileConfig(DataFileSystem * v)
{
	data = v;
}
	
//-----------------------------------------------------------------------------
SurfaceFileConfig::SurfaceFileConfig(const SurfaceFileConfig& v)
{
	data = v.data;
}

//-----------------------------------------------------------------------------
SurfaceFileConfig::~SurfaceFileConfig()
{
	data = 0;
}

//-----------------------------------------------------------------------------
SurfaceFileConfig& SurfaceFileConfig::operator = (const SurfaceFileConfig& v)
{
	if (&v == this)
        return *this; 
	data = v.data;
	return *this; 
}

//-----------------------------------------------------------------------------
void SurfaceFileConfig::read(const QString& nameFile)
{
	data->clear();
    QFile file(nameFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

	SurfaceFileConfigData * tfcd = dynamic_cast<SurfaceFileConfigData*>(data);

	QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;

	if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        return;
    }

	QDomElement root = domDocument.documentElement();

	if (root.tagName() != "surfaces")
    {
        return;
    }

    if (root.hasAttribute("version") &&
            (root.attribute("version") != QString("1.0")))
    {
        return;
    }

	if (root.hasAttribute("current"))
    {
		tfcd->setCurrentSurface(root.attribute("current").toInt());
    }

	QDomElement surface = root.firstChildElement();

	if(surface.tagName() == "surface")
    {		
		while(!surface.isNull())
		{
			SurfaceData * newData = new SurfaceData();
			int id = surface.attribute("id").toInt();
			QString name = surface.attribute("name");
			QString creationDate = surface.attribute("creationDate");
			int registrationDate = surface.attribute("registrationDate").toInt();
			bool edited = surface.attribute("edited").toInt();
			newData->setId(id);
			newData->setName(name);
			newData->setCreationDate(creationDate);
			newData->setRegistrationDate(registrationDate);
			newData->setEdited(edited);			

			QDomElement coordReal = surface.firstChildElement();
			QList<double> dim;
			dim << coordReal.attribute("x1").toDouble();
			dim << coordReal.attribute("x2").toDouble();
			dim << coordReal.attribute("y1").toDouble();
			dim << coordReal.attribute("y2").toDouble();
			dim << coordReal.attribute("z1").toDouble();
			dim << coordReal.attribute("z2").toDouble();			
		
			newData->setDimensionReal(dim);

			/*QDomElement coord3d = coordReal.nextSiblingElement();
			dim.clear();
			dim << coord3d.attribute("x1").toDouble();
			dim << coord3d.attribute("x2").toDouble();
			dim << coord3d.attribute("y1").toDouble();
			dim << coord3d.attribute("y2").toDouble();
			dim << coord3d.attribute("z1").toDouble();
			dim << coord3d.attribute("z2").toDouble();			

			newData->setDimension3D(dim);*/

			QDomElement environment = coordReal.nextSiblingElement();
			double opacity = environment.attribute("opacity").toDouble();
			newData->setOpacity(opacity);

			QDomElement colorAmb = environment.firstChildElement();
			double r = colorAmb.attribute("r").toDouble();
			double g = colorAmb.attribute("g").toDouble();
			double b = colorAmb.attribute("b").toDouble();
			double alpha = colorAmb.attribute("alpha").toDouble();
			newData->setColorAmbient(Ogre::ColourValue(r,g,b,alpha));
			
			QDomElement colorDiff = colorAmb.nextSiblingElement();
			r = colorDiff.attribute("r").toDouble();
			g = colorDiff.attribute("g").toDouble();
			b = colorDiff.attribute("b").toDouble();
			alpha = colorDiff.attribute("alpha").toDouble();
			newData->setColorDiffuse(Ogre::ColourValue(r,g,b,alpha));
			
			QDomElement colorSpec = colorDiff.nextSiblingElement();
			r = colorSpec.attribute("r").toDouble();
			g = colorSpec.attribute("g").toDouble();
			b = colorSpec.attribute("b").toDouble();
			alpha = colorSpec.attribute("alpha").toDouble();			
			newData->setColorSpecular(Ogre::ColourValue(r,g,b,alpha));
			
			QDomElement colorSelf = colorSpec.nextSiblingElement();
			r = colorSelf.attribute("r").toDouble();
			g = colorSelf.attribute("g").toDouble();
			b = colorSelf.attribute("b").toDouble();
			alpha = colorSelf.attribute("alpha").toDouble();
			
			newData->setColorSelf(Ogre::ColourValue(r,g,b,alpha));
			
			QDomElement description = environment.nextSiblingElement();
			QString value = description.attribute("value");
			newData->setDescription(value);

			tfcd->addObject(newData);
			surface = surface.nextSiblingElement();
		}
	} 


	file.close();

}

//-----------------------------------------------------------------------------
void SurfaceFileConfig::exportData(const QString& nameFile)
{
	QFile file(nameFile);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }

	SurfaceFileConfigData * tfcd = dynamic_cast<SurfaceFileConfigData*>(data);
	
    QXmlStreamWriter xw;

    xw.setAutoFormatting(true);

    xw.setDevice(&file);
    xw.setCodec(QObject::tr("ISO 8859-1").toStdString().c_str());

    xw.writeStartDocument();
    xw.writeDTD("<!DOCTYPE surfaces>");
    xw.writeStartElement("surfaces");
    xw.writeAttribute("version", "1.0");
	xw.writeAttribute("current",QString::number(tfcd->getCurrentSurface()));

	QList<ObjectFileSystem*> listObejcts = tfcd->getObjects();
	QList<ObjectFileSystem*>::iterator it;

	for(it=listObejcts.begin();it!=listObejcts.end();it++)
	{		
		SurfaceData * td = dynamic_cast<SurfaceData*>(*it);

		xw.writeStartElement("surface");
			xw.writeAttribute("id", QString::number(td->getId()));
			xw.writeAttribute("name", td->getName());
			xw.writeAttribute("creationDate", td->getCreationDate());
			xw.writeAttribute("registrationDate", QString::number(td->getRegistrationDate()));
			xw.writeAttribute("edited", QString::number(td->getEdited()));

				QList<double> d = td->getDimensionReal();

				xw.writeStartElement("coordReal");
					xw.writeAttribute("x1", QString::number(d[0],'f',7));
					xw.writeAttribute("x2", QString::number(d[1],'f',7));
					xw.writeAttribute("y1", QString::number(d[2],'f',7));
					xw.writeAttribute("y2", QString::number(d[3],'f',7));
					xw.writeAttribute("z1", QString::number(d[4],'f',7));
					xw.writeAttribute("z2", QString::number(d[5],'f',7));
				xw.writeEndElement();

				d = td->getDimension3D();

				xw.writeStartElement("coord3d");
					xw.writeAttribute("x1", QString::number(d[0],'f',7));
					xw.writeAttribute("x2", QString::number(d[1],'f',7));
					xw.writeAttribute("y1", QString::number(d[2],'f',7));
					xw.writeAttribute("y2", QString::number(d[3],'f',7));
					xw.writeAttribute("z1", QString::number(d[4],'f',7));
					xw.writeAttribute("z2", QString::number(d[5],'f',7));
				xw.writeEndElement();

				xw.writeStartElement("environment");

					xw.writeAttribute("opacity", QString::number(td->getOpacity(),'f',7));

					Ogre::ColourValue c = td->getColorAmbient();
					xw.writeStartElement("colorAmb");		
						xw.writeAttribute("r", QString::number(c.r,'f',7));
						xw.writeAttribute("g", QString::number(c.g,'f',7));
						xw.writeAttribute("b", QString::number(c.b,'f',7));
						xw.writeAttribute("alpha", QString::number(c.a,'f',7));
					xw.writeEndElement();

					c = td->getColorDiffuse();
					xw.writeStartElement("colorDiff");
						xw.writeAttribute("r", QString::number(c.r,'f',7));
						xw.writeAttribute("g", QString::number(c.g,'f',7));
						xw.writeAttribute("b", QString::number(c.b,'f',7));
						xw.writeAttribute("alpha", QString::number(c.a,'f',7));
					xw.writeEndElement();

					c = td->getColorSpecular();
					xw.writeStartElement("colorSpec");
						xw.writeAttribute("r", QString::number(c.r,'f',7));
						xw.writeAttribute("g", QString::number(c.g,'f',7));
						xw.writeAttribute("b", QString::number(c.b,'f',7));
						xw.writeAttribute("alpha", QString::number(c.a,'f',7));
					xw.writeEndElement();

					c = td->getColorSelf();
					xw.writeStartElement("colorSelf");
						xw.writeAttribute("r", QString::number(c.r,'f',7));
						xw.writeAttribute("g", QString::number(c.g,'f',7));
						xw.writeAttribute("b", QString::number(c.b,'f',7));
						xw.writeAttribute("alpha", QString::number(c.a,'f',7));
					xw.writeEndElement();	

				xw.writeEndElement();

				xw.writeStartElement("description");
					xw.writeAttribute("value", td->getDescription());
				xw.writeEndElement();

		xw.writeEndElement();
	}

	xw.writeEndElement();

    xw.writeEndDocument();
    file.close();
}