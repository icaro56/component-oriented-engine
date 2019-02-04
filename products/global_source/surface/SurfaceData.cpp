#include "SurfaceData.h"
#include <OgreVector3.h>
#include "SurfaceException.h"

const QString SurfaceData::Name = "name";
const QString SurfaceData::Description = "description";
const QString SurfaceData::CreationDate = "creationDate";
const QString SurfaceData::RegistrationDate = "registrationDate";

//-----------------------------------------------------------------------------
SurfaceData::SurfaceData():SurfaceObject()
{
	name = "";
	description = "";
	creationDate = "";

	isInit = false;
	edited = false;

	cReal_x1 = 0;
	cReal_y1 = 0;
	cReal_z1 = 0;
	cReal_x2 = 0;
	cReal_y2 = 0;
	cReal_z2 = 0;

	c3D_x1 = -16000.0;
	c3D_y1 = 16000.0;
	c3D_z1 = 0;
	c3D_x2 = 16000.0;
	c3D_y2 = -16000.0;
	c3D_z2 = 0;

	id = 0;
	lod = 2;//lod baixo
	registrationDate = 0;//lod baixo

	kr = 0;
	k3d = 0;

	width = 0;
	dx = 0;
}

//-----------------------------------------------------------------------------
SurfaceData::SurfaceData(const SurfaceData& v)
{
	name = v.name;
	description = v.description;
	creationDate = v.creationDate;

	isInit = v.isInit;
	edited = v.edited;

	cReal_x1 = v.cReal_x1;
	cReal_y1 = v.cReal_y1;
	cReal_z1 = v.cReal_z1;
	cReal_x2 = v.cReal_x2;
	cReal_y2 = v.cReal_y2;
	cReal_z2 = v.cReal_z2;

	c3D_x1 = v.c3D_x1;
	c3D_y1 = v.c3D_y1;
	c3D_z1 = v.c3D_z1;
	c3D_x2 = v.c3D_x2;
	c3D_y2 = v.c3D_y2;
	c3D_z2 = v.c3D_z2;

	diff = v.diff;
	spec = v.spec;
	amb = v.amb;
	self = v.self;

	id = v.id;
	lod = v.lod;
	registrationDate = v.registrationDate;

	kr = v.kr;
	k3d = v.k3d;

	width = v.width;
	dx = v.dx;
}

//-----------------------------------------------------------------------------
SurfaceData::~SurfaceData()
{
}

//-----------------------------------------------------------------------------
SurfaceData& SurfaceData::operator = (const SurfaceData& v)
{
	if (&v == this)
        return *this; 

	name = v.name;
	description = v.description;
	creationDate = v.creationDate;

	isInit = v.isInit;
	edited = v.edited;

	cReal_x1 = v.cReal_x1;
	cReal_y1 = v.cReal_y1;
	cReal_z1 = v.cReal_z1;
	cReal_x2 = v.cReal_x2;
	cReal_y2 = v.cReal_y2;
	cReal_z2 = v.cReal_z2;

	c3D_x1 = v.c3D_x1;
	c3D_y1 = v.c3D_y1;
	c3D_z1 = v.c3D_z1;
	c3D_x2 = v.c3D_x2;
	c3D_y2 = v.c3D_y2;
	c3D_z2 = v.c3D_z2;

	diff = v.diff;
	spec = v.spec;
	amb = v.amb;
	self = v.self;

	id = v.id;
	lod = v.lod;
	registrationDate = v.registrationDate;

	kr = v.kr;
	k3d = v.k3d;

	width = v.width;
	dx = v.dx;

	return *this;
}

//-----------------------------------------------------------------------------
void SurfaceData::init()
{
	kr = qAbs((c3D_x2 - c3D_x1)/(cReal_x2-cReal_x1));
	k3d = 1./kr;
	width = c3D_x2 - c3D_x1;
	dx = cReal_x2 - cReal_x1;

	//Z2 3d é calculado após sabermos o z2 real. Ele é o exagero vertical
	c3D_z2 =  ((c3D_x2 - c3D_x1) /
                            (cReal_x2 - cReal_x1)) *
                            (cReal_z2 - cReal_z1);//2280.5;


	isInit = true;
}

//-----------------------------------------------------------------------------
void SurfaceData::isInitiated() const
{
	if(!isInit)
	{
		QString str = QObject::tr("SurfaceData não foi iniciado.");
		std::cout << str.toStdString() << std::endl;
		SurfaceException e;
		e.setMessage(str);
		throw e;
	}
}

//-----------------------------------------------------------------------------
QString SurfaceData::getName() const
{
	return name;
}

//-----------------------------------------------------------------------------
QList<double> SurfaceData::getCoordinateMinReal() const
{   
	isInitiated();
	QList<double> r;
	r.push_back(cReal_x1);
	r.push_back(cReal_y1);
	r.push_back(cReal_z1);
	return r;
}

//-----------------------------------------------------------------------------
QList<double> SurfaceData::getCoordinateMin3D() const
{   
	isInitiated();
	QList<double> r;
	r.push_back(c3D_x1);
	r.push_back(c3D_y1);
	r.push_back(c3D_z1);
	return r;
}


//-----------------------------------------------------------------------------
QList<double> SurfaceData::getCoordinateMaxReal() const
{
	isInitiated();
	QList<double> r;
	r.push_back(cReal_x2);
	r.push_back(cReal_y2);
	r.push_back(cReal_z2);
	return r;
}

//-----------------------------------------------------------------------------
QList<double> SurfaceData::getCoordinateMax3D() const
{
	isInitiated();
	QList<double> r;
	r.push_back(c3D_x2);
	r.push_back(c3D_y2);
	r.push_back(c3D_z2);
	return r;
}

//-----------------------------------------------------------------------------
QList<double> SurfaceData::getDimensionReal() const
{
	isInitiated();
	QList<double> r;
	r.push_back(cReal_x1);
	r.push_back(cReal_x2);
	r.push_back(cReal_y1);
	r.push_back(cReal_y2);
	r.push_back(cReal_z1);	
	r.push_back(cReal_z2);
	return r;
}

//-----------------------------------------------------------------------------
QList<double> SurfaceData::getDimension3D() const
{
	isInitiated();
	QList<double> r;
	r.push_back(c3D_x1);
	r.push_back(c3D_x2);
	r.push_back(c3D_y1);
	r.push_back(c3D_y2);
	r.push_back(c3D_z1);	
	r.push_back(c3D_z2);
	return r;
}

//-----------------------------------------------------------------------------
Ogre::ColourValue SurfaceData::getColorDiffuse() const
{
	return diff;
}

//-----------------------------------------------------------------------------
Ogre::ColourValue SurfaceData::getColorAmbient() const
{
	return amb;
}

//-----------------------------------------------------------------------------
Ogre::ColourValue SurfaceData::getColorSpecular() const
{
	return spec;
}

//-----------------------------------------------------------------------------
Ogre::ColourValue SurfaceData::getColorSelf() const
{
	return self;
}

//-----------------------------------------------------------------------------
void SurfaceData::setName(const QString& v)
{
	name = v;
}

//-----------------------------------------------------------------------------
void SurfaceData::setCoordinateMinReal(const double& x, const double& y, const double& z)
{
	isInit = false;
	cReal_x1 = x;
	cReal_y1 = y;
	cReal_z1 = z;
}

//-----------------------------------------------------------------------------
void SurfaceData::setCoordinateMin3D(const double& x, const double& y, const double& z)
{
	isInit = false;
	c3D_x1 = x;
	c3D_y1 = y;
	c3D_z1 = z;
}

//-----------------------------------------------------------------------------
void SurfaceData::setCoordinateMaxReal(const double& x, const double& y, const double& z)
{
	isInit = false;
	cReal_x2 = x;
	cReal_y2 = y;
	cReal_z2 = z;
}

//-----------------------------------------------------------------------------
void SurfaceData::setCoordinateMax3D(const double& x, const double& y, const double& z)
{
	isInit = false;
	c3D_x2 = x;
	c3D_y2 = y;
	c3D_z2 = z;
}

//-----------------------------------------------------------------------------
void SurfaceData::setDimensionReal(const QList<double>& dim)
{
	isInit = false;
	cReal_x1 = dim[0];
	cReal_x2 = dim[1];
	cReal_y1 = dim[2];
	cReal_y2 = dim[3];
	cReal_z1 = dim[4];	
	cReal_z2 = dim[5];

	//Calculando dimensões 3D baseadas nas dimensões reais fazendo uma razão de 1 para 1
	double diffXReal = abs(cReal_x2 - cReal_x1);
	double halfDiffXReal = diffXReal * 0.5;
	c3D_x1 = -halfDiffXReal;
	c3D_x2 = halfDiffXReal;

	double diffYReal = abs(cReal_y2 - cReal_y1);
	double halfDiffYReal = diffYReal * 0.5;
	c3D_y1 = halfDiffXReal;
	c3D_y2 = -halfDiffXReal;

	c3D_z1 = 0;

	//será calculado mais na frente. Exagero vertical
	c3D_z2 = 100;
}

//-----------------------------------------------------------------------------
/*void SurfaceData::setDimension3D(const QList<double>& dim)
{
	//função comentada, pois agora as dimensões 3d possuem razão de 1 para 1 com as coordenadas reais.
	//Ou seja, 1 unidade real será uma unidade 3D. 1 Metro.
	c3D_x1 = dim[0];
	c3D_x2 = dim[1];
	c3D_y1 = dim[2];
	c3D_y2 = dim[3];
	c3D_z1 = dim[4];	
	c3D_z2 = dim[5];
}*/

//-----------------------------------------------------------------------------
void SurfaceData::setColorDiffuse(const Ogre::ColourValue& c) 
{
	diff = c;
}

//-----------------------------------------------------------------------------
void SurfaceData::setColorAmbient(const Ogre::ColourValue& c) 
{
	amb = c;
}

//-----------------------------------------------------------------------------
void SurfaceData::setColorSpecular(const Ogre::ColourValue& c) 
{
	spec = c;
}

//-----------------------------------------------------------------------------
void SurfaceData::setColorSelf(const Ogre::ColourValue& c) 
{
	self = c;
}

//-----------------------------------------------------------------------------
void SurfaceData::convertRealTo3D(Vector3& point) const
{
	isInitiated();

	/*std::cerr << "\n Debugando ...";
	std::cerr << "\n c3D_x1: " << c3D_x1;
	std::cerr << "\n c3D_x2: " << c3D_x2;
	std::cerr << "\n cReal_x1: " << cReal_x1;
	std::cerr << "\n cReal_x2: " << cReal_x2;

	std::cerr << "\n c3D_y1: " << c3D_y1;
	std::cerr << "\n c3D_y2: " << c3D_y2;
	std::cerr << "\n cReal_y1: " << cReal_y1;
	std::cerr << "\n cReal_y2: " << cReal_y2;
	
	std::cerr << "\n c3D_z1: " << c3D_z1;
	std::cerr << "\n c3D_z2: " << c3D_z2;
	std::cerr << "\n cReal_z1: " << cReal_z1;
	std::cerr << "\n cReal_z2: " << cReal_z2;*/

    double auxY = point.y;
    point.x = c3D_x1 + (point.x - cReal_x1)*
                (c3D_x2 - c3D_x1)/
                (cReal_x2 - cReal_x1);
    point.y = c3D_z1 + (point.z - cReal_z1)*
                (c3D_z2 - c3D_z1)/
                (cReal_z2 - cReal_z1);
    point.z = c3D_y1 + (auxY - cReal_y1)*
                (c3D_y2 - c3D_y1)/
                (cReal_y2 - cReal_y1);
}

//-----------------------------------------------------------------------------
void SurfaceData::convertRealTo3D(double& x,double& y,double& z) const
{
	isInitiated();
    double auxY = y;
    x = c3D_x1 + (x - cReal_x1)*
                (c3D_x2 - c3D_x1)/
                (cReal_x2 - cReal_x1);
    y = c3D_z1 + (z - cReal_z1)*
                (c3D_z2 - c3D_z1)/
                (cReal_z2 - cReal_z1);
    z = c3D_y1 + (auxY - cReal_y1)*
                (c3D_y2 - c3D_y1)/
                (cReal_y2 - cReal_y1);
}

//-----------------------------------------------------------------------------
void SurfaceData::convert3DToReal(Vector3& point) const
{
	isInitiated();
    double auxY = point.y;
    point.x = cReal_x1 + (point.x - c3D_x1)*
                (cReal_x2 - cReal_x1)/
                (c3D_x2 - c3D_x1);
    point.y = cReal_y1 + (point.z - c3D_y1)*
                (cReal_y2 - cReal_y1)/
                (c3D_y2 - c3D_y1);
    point.z = cReal_z1 + (auxY - c3D_z1)*
                (cReal_z2 - cReal_z1)/
                (c3D_z2 - c3D_z1);
}

//-----------------------------------------------------------------------------
void SurfaceData::convert3DToReal(double& x,double& y,double& z) const
{
	isInitiated();
    double auxY = y;
    x = cReal_x1 + (x - c3D_x1)*
                (cReal_x2 - cReal_x1)/
                (c3D_x2 - c3D_x1);
    y = cReal_y1 + (z - c3D_y1)*
                (cReal_y2 - cReal_y1)/
                (c3D_y2 - c3D_y1);
    z = cReal_z1 + (auxY - c3D_z1)*
                (cReal_z2 - cReal_z1)/
                (c3D_z2 - c3D_z1);
}

//-----------------------------------------------------------------------------
double SurfaceData::getWidth() const
{
	isInitiated();
	return width;
}

//-----------------------------------------------------------------------------
double SurfaceData::getDiffXReal() const
{
	isInitiated();
	return dx;
}

//-----------------------------------------------------------------------------
bool SurfaceData::inside(const Vector3& point)  const
{
	isInitiated();
    if((point.x <= cReal_x1) || (point.x >= cReal_x2)
            || (point.y <= cReal_y1) || (point.y >= cReal_y2))
    {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool SurfaceData::isMine(int p,const double& v) const
{
	isInitiated();
	QList<double> &dim = getDimensionReal();
	if(p == 1)
		return ((dim[0] <= v && v <= dim[1]) || (dim[1] <= v && v <= dim[0]) )?
			true:false;			
	return ((dim[2] <= v && v <= dim[3]) || (dim[3] <= v && v <= dim[2]) )?
		true:false;
	return false;
}

//-----------------------------------------------------------------------------
double SurfaceData::kRealTo3D() const
{    
	isInitiated();
	return kr;
}

//-----------------------------------------------------------------------------
double SurfaceData::k3DToReal() const
{    
	isInitiated();
	return k3d;
}

//-----------------------------------------------------------------------------
bool SurfaceData::getEdited() const
{
	return edited;
}

//-----------------------------------------------------------------------------
void SurfaceData::setEdited(bool v)
{
	edited = v;
}

//-----------------------------------------------------------------------------
double SurfaceData::getOpacity() const
{
	return opacity;
}

//-----------------------------------------------------------------------------
void SurfaceData::setOpacity(const double& v)
{
	opacity = v;
}

//-----------------------------------------------------------------------------
int SurfaceData::getLod() const
{
	return lod;
}

//-----------------------------------------------------------------------------
void SurfaceData::setLod(int l) 
{
	lod = l;
}

//-----------------------------------------------------------------------------
Vector3 SurfaceData::getCenterPosition() const
{
	isInitiated();
	Vector3 pos = Vector3::ZERO;
	
	pos.x = abs(c3D_x2) - abs(c3D_x1);
	pos.z = abs(c3D_y2) - abs(c3D_y1);
	pos.y = abs(c3D_z2) - abs(c3D_z1);

	return pos;
}

//-----------------------------------------------------------------------------
void SurfaceData::setDescription(const QString& v)
{
	description = v;
}

//-----------------------------------------------------------------------------
QString SurfaceData::getDescription() const
{
	return description;
}

//-----------------------------------------------------------------------------
int SurfaceData::getRegistrationDate() const
{
	return registrationDate;
}

//-----------------------------------------------------------------------------
void SurfaceData::setRegistrationDate(int v)
{
	registrationDate = v;
}

//-----------------------------------------------------------------------------
int SurfaceData::getId() const
{
	return id;
}

//-----------------------------------------------------------------------------
void SurfaceData::setId(int v) 
{
	id = v;
}

//-----------------------------------------------------------------------------
void SurfaceData::setCreationDate(const QString& v)
{
	creationDate = v;
}

//-----------------------------------------------------------------------------
const QString SurfaceData::getCreationDate() const
{
	return creationDate;
}

//-----------------------------------------------------------------------------
QMap<QString,QString> SurfaceData::getAtributes() const
{
	QMap<QString,QString> ret;	

	ret.insert("name",name);
	ret.insert("description",description);
	ret.insert("creationDate",creationDate);

	ret.insert("cReal_x1",QString::number(cReal_x1,'f',7));
	ret.insert("cReal_y1",QString::number(cReal_y1,'f',7));
	ret.insert("cReal_z1",QString::number(cReal_z1,'f',7));
	ret.insert("cReal_x2",QString::number(cReal_x2,'f',7));
	ret.insert("cReal_y2",QString::number(cReal_y2,'f',7));
	ret.insert("cReal_z2",QString::number(cReal_z2,'f',7));

	ret.insert("c3D_x1",QString::number(c3D_x1,'f',7));
	ret.insert("c3D_y1",QString::number(c3D_y1,'f',7));
	ret.insert("c3D_z1",QString::number(c3D_z1,'f',7));
	ret.insert("c3D_x2",QString::number(c3D_x2,'f',7));
	ret.insert("c3D_y2",QString::number(c3D_y2,'f',7));
	ret.insert("c3D_z2",QString::number(c3D_z2,'f',7));

	ret.insert("diff_r",QString::number(diff.r,'f',7));	
	ret.insert("diff_g",QString::number(diff.g,'f',7));	
	ret.insert("diff_b",QString::number(diff.b,'f',7));	
	ret.insert("diff_a",QString::number(diff.a,'f',7));	

	ret.insert("spec_r",QString::number(spec.r,'f',7));	
	ret.insert("spec_g",QString::number(spec.g,'f',7));	
	ret.insert("spec_b",QString::number(spec.b,'f',7));	
	ret.insert("spec_a",QString::number(spec.a,'f',7));	

	ret.insert("amb_r",QString::number(amb.r,'f',7));	
	ret.insert("amb_g",QString::number(amb.g,'f',7));	
	ret.insert("amb_b",QString::number(amb.b,'f',7));	
	ret.insert("amb_a",QString::number(amb.a,'f',7));	

	ret.insert("self_r",QString::number(self.r,'f',7));	
	ret.insert("self_g",QString::number(self.g,'f',7));	
	ret.insert("self_b",QString::number(self.b,'f',7));	
	ret.insert("self_a",QString::number(self.a,'f',7));	
	
	ret.insert("opacity",QString::number(opacity,'f',7));

	ret.insert("lod",QString::number(lod));

	ret.insert("registrationDate",QString::number(registrationDate));

	return ret;
}

//-----------------------------------------------------------------------------
void SurfaceData::setAtributes(const QMap<QString,QString>& v)
{
	double r=0;
	double g=0;
	double b=0;
	double a=1;

	name = v["name"];
	description = v["description"];
	creationDate = v["creationDate"];
	cReal_x1 = v["cReal_x1"].toDouble();
	cReal_y1 = v["cReal_y1"].toDouble();
	cReal_z1 = v["cReal_z1"].toDouble();
	cReal_x2 = v["cReal_x2"].toDouble();
	cReal_y2 = v["cReal_y2"].toDouble();
	cReal_z2 = v["cReal_z2"].toDouble();

	c3D_x1 = v["c3D_x1"].toDouble();
	c3D_y1 = v["c3D_y1"].toDouble();
	c3D_z1 = v["c3D_z1"].toDouble();
	c3D_x2 = v["c3D_x2"].toDouble();
	c3D_y2 = v["c3D_y2"].toDouble();
	c3D_z2 = v["c3D_z2"].toDouble();

	r = v["diff_r"].toDouble();
	g = v["diff_g"].toDouble();
	b = v["diff_b"].toDouble();
	a = v["diff_a"].toDouble();
	diff = Ogre::ColourValue(r,g,b,a);

	r = v["spec_r"].toDouble();
	g = v["spec_g"].toDouble();
	b = v["spec_b"].toDouble();
	a = v["spec_a"].toDouble();
	spec = Ogre::ColourValue(r,g,b,a);

	r = v["amb_r"].toDouble();
	g = v["amb_g"].toDouble();
	b = v["amb_b"].toDouble();
	a = v["amb_a"].toDouble();
	amb = Ogre::ColourValue(r,g,b,a);

	r = v["self_r"].toDouble();
	g = v["self_g"].toDouble();
	b = v["self_b"].toDouble();
	a = v["self_a"].toDouble();
	self = Ogre::ColourValue(r,g,b,a);

	opacity = v["opacity"].toDouble();

	lod = v["lod"].toInt();

	registrationDate = v["registrationDate"].toInt();
}
