///////////////////////////////////////////////////////////
//  SurfaceData.h
//  Implementation of the Class SurfaceData
//  Created on:      08-jul-2013
///////////////////////////////////////////////////////////

#if !defined(SURFACE_DATA_H)
#define SURFACE_DATA_H

#include "SurfaceObject.h"
#include "persistence\filesystem\ObjectFileSystem.h"

#include <engine\RenderView.h>

#include <QString>
#include <QList>
#include <QMap>
#include <OgreColourValue.h>

enum TypeSurfaceDataEdit
{
	TSDE_Name,
	TSDE_Descripition
};

class SurfaceData: public SurfaceObject, public ObjectFileSystem
{

public:		

	//! Construtor
	SurfaceData();

	//! Construtor de copia
    SurfaceData(const SurfaceData&);

	//! Destrutor
	virtual ~SurfaceData();	

	//! operator de atribuição
    SurfaceData& operator = (const SurfaceData&);

	/*!
     * \brief retorna o nome do terreno
	 * \return QString nome
     */
	void init();	

	/*!
     * \brief retorna o nome do terreno
	 * \return QString nome
     */
	QString getName() const;	

	/*!
     * \brief retorna o ponto mínimo
	 * \return QList<double>
	 *  aonde 0 -> x1
	 *  aonde 1 -> y1
	 *  aonde 2 -> z1
     */
	QList<double> getCoordinateMinReal() const;

	/*!
     * \brief retorna o ponto mínimo
	 * \return QList<double>
	 *  aonde 0 -> x1
	 *  aonde 1 -> y1
	 *  aonde 2 -> z1
     */
	QList<double> getCoordinateMin3D() const;

	/*!
     * \brief retorna o ponto máximo
	 * \return QList<double>
	 *  aonde 0 -> x2
	 *  aonde 1 -> y2
	 *  aonde 2 -> z2
     */
	QList<double> getCoordinateMaxReal() const;

	/*!
     * \brief retorna o ponto máximo
	 * \return QList<double>
	 *  aonde 0 -> x2
	 *  aonde 1 -> y2
	 *  aonde 2 -> z2
     */
	QList<double> getCoordinateMax3D() const;

	/*!
     * \brief retorna o ponto máximo e o ponto mínimo
	 * \return QList<double>
	 *  aonde 0 -> x1
	 *  aonde 1 -> x2
	 *  aonde 2 -> y1
	 *  aonde 3 -> y2
	 *  aonde 4 -> z1
	 *  aonde 5 -> z2
     */
	QList<double> getDimensionReal() const;

	/*!
     * \brief retorna o ponto máximo e o ponto mínimo
	 * \return QList<double>
	 *  aonde 0 -> x1
	 *  aonde 1 -> x2
	 *  aonde 2 -> y1
	 *  aonde 3 -> y2
	 *  aonde 4 -> z1
	 *  aonde 5 -> z2
     */
	QList<double> getDimension3D() const;

	/*!
     * \brief modifica a cor difusa
	 * \param Ogre::ColourValue
     */
	Ogre::ColourValue getColorDiffuse() const;

	/*!
     * \brief modifica a cor ambiente
	 * \param Ogre::ColourValue
     */
	Ogre::ColourValue getColorAmbient() const;

	/*!
     * \brief modifica a cor especular
	 * \param Ogre::ColourValue
     */
	Ogre::ColourValue getColorSpecular() const;

	/*!
     * \brief modifica a cor self
	 * \param Ogre::ColourValue
     */
	Ogre::ColourValue getColorSelf() const;

	/*!
     * \brief modifica o nome do terreno
	 * \param QString name
     */
	void setName(const QString& name);	

	/*!
     * \brief modifica o ponto mínimo
	 * \param double x1 -> x mínimo
	 * \param double y1 -> y mínimo
	 * \param double z1 -> z mínimo
     */
	void setCoordinateMinReal(const double& x1, const double& y1, 
		const double& z1);

	/*!
     * \brief modifica o ponto mínimo
	 * \param double x1 -> x mínimo
	 * \param double y1 -> y mínimo
	 * \param double z1 -> z mínimo
     */
	void setCoordinateMin3D(const double& x1, const double& y1, 
		const double& z1);
	
	/*!
     * \brief modifica o ponto máximo
	 * \param double x2 -> x máximo
	 * \param double y2 -> y máximo
	 * \param double z2 -> z máximo
     */
	void setCoordinateMaxReal(const double& x2, const double& y2, 
		const double& zmaz);

	/*!
     * \brief modifica o ponto máximo
	 * \param double x2 -> x máximo
	 * \param double y2 -> y máximo
	 * \param double z2 -> z máximo
     */
	void setCoordinateMax3D(const double& x2, const double& y2,
		const double& zmaz);

	/*!
     * \brief modifica a cor difusa
	 * \param Ogre::ColourValue
     */
	void setDimensionReal(const QList<double>&);
	/*!
     * \brief modifica a cor difusa
	 * \param Ogre::ColourValue
	 * \warning Função defasada, agora dimensões 3d são geradas baseadas nas dimensões reais.
     */
	//void setDimension3D(const QList<double>&) ;

	/*!
     * \brief modifica a cor difusa
	 * \param Ogre::ColourValue
     */
	void setColorDiffuse(const Ogre::ColourValue& color);

	/*!
     * \brief modifica a cor ambiente
	 * \param Ogre::ColourValue
     */
	void setColorAmbient(const Ogre::ColourValue& color);

	/*!
     * \brief modifica a cor especular
	 * \param Ogre::ColourValue
     */
	void setColorSpecular(const Ogre::ColourValue& color);

	/*!
     * \brief modifica a cor self
	 * \param Ogre::ColourValue
     */
	void setColorSelf(const Ogre::ColourValue& color);

	/*!
     * \brief Método retorna a constante de conversão de coordenada de real para 3D
     * \return double
     */
	double kRealTo3D() const;

	/*!
     * \brief Método retorna a constante de conversão de coordenada de real para 3D
     * \return double
     */
	double k3DToReal() const;

    /*!
     * \brief Método utilizado para converter pontos 3D a partir de um ponto no mundo real
     * \param point const Vector3& ponto 3D em coordenadas do mundo 3D
     * \return Vector3 contendo ponto 3D correspondente no mundo real.
     */
    void convertRealTo3D(Vector3& point) const;

	/*!
     * \brief Método utilizado para converter pontos 3D a partir de
        um ponto no mundo real 
     * \param point const Vector3& ponto 3D em coordenadas do mundo 3D
	 * \return Vector3 contendo ponto 3D correspondente no mundo real.
     */
	void convertRealTo3D(double& x,double& y,double& z) const;

	/*!
     * \brief Método utilizado para converter pontos 3D a partir de
        um ponto no mundo 3D
     * \param point const Vector3& ponto 3D em coordenadas do mundo
        real
	 * \return Vector3 contendo ponto 3D correspondente no mundo 3D.
     */
    void convert3DToReal(Vector3& point) const;
	
	/*!
     * \brief Método utilizado para converter pontos 3D a partir de
        um ponto no mundo 3D
     * \param point const Vector3& ponto 3D em coordenadas do mundo
        real
	 * \return Vector3 contendo ponto 3D correspondente no mundo 3D.
     */
	void convert3DToReal(double& x,double& y,double& z) const;

	/*!
     * \brief Função que retorna a diferença de X2 e X1 do terreno
     */
	double getWidth() const;	

	/*!
     * \brief Função que retorna a diferença de X2 e X1 do terreno
     */
	double getDiffXReal() const;

	/*!
     * \brief retorna estado de edição
     */
	bool getEdited() const;

	/*!
     * \brief retorna estado de edição
     * \param 
     */
	void setEdited(bool);

	/*!
        \brief Método retorna se determinado ponto está em cima ou
        fora do terreno.
        \param point const Vector3& ponto real a qual se quer fazer o teste.
        \return bool se está ou não em cima do terreno.
		\warning Rever necessidade desta função
    */
    bool inside(const Vector3& point) const;

	 /*!
     * \brief Função que retorna se um valor esta nas coordenadas da mina
     * \param int 1 - x y
     * \param double valor
     * \return QVector<double> contendo as dimensões do terreno
		\warning Rever necessidade desta função
     */
	bool isMine(int,const double&) const;	

	/*!
     * \brief retorna a opacidade
     * \return  double
     */
	double getOpacity() const;
	
	/*!
     * \brief modifica a opacidade
     * \param alpha opacidade
     */
	void setOpacity(const double& alpha);

	/*!
     * \brief
     * \param 
     */
	void setLod(int l);

	/*!
     * \brief
     * \param 
     */
	int getLod() const;

	/*!
     * \brief
     * \param 
     */
	Vector3 getCenterPosition() const;

	/*!
     * \brief
     * \param 
     */
	void setDescription(const QString&);

	/*!
     * \brief
     * \param 
     */
	QString getDescription() const;

	/*!
     * \brief
     * \param 
     */
	void setRegistrationDate(int l);

	/*!
     * \brief
     * \param 
     */
	int getRegistrationDate() const;

	/*!
     * \brief
     * \param 
     */
	void setCreationDate(const QString& l);

	/*!
     * \brief
     * \param 
     */
	const QString getCreationDate() const;

	/*!
     * \brief
     * \param 
     */
	void setId(int l);

	/*!
     * \brief
     * \param 
     */
	int getId() const;

	/*!
     * \brief
     * \param 
     */
	QMap<QString,QString> getAtributes() const;


	/*!
     * \brief
     * \param 
     */
	 void setAtributes(const QMap<QString,QString> &);

private:

	/*!
     * \brief
     * \param 
     */
	void isInitiated() const;

public:

	static const QString Name;
	static const QString Description;
	static const QString CreationDate;
	static const QString RegistrationDate;

private:

	QString name;
	QString description;
	QString creationDate;

	bool isInit;
	bool edited;

	double cReal_x1; 
	double cReal_y1;
	double cReal_z1;
	double cReal_x2;
	double cReal_y2;
	double cReal_z2;

	double c3D_x1; 
	double c3D_y1;
	double c3D_z1;
	double c3D_x2;
	double c3D_y2;
	double c3D_z2;

	double opacity;
	int id;
	int lod;
	int registrationDate;

	Ogre::ColourValue diff, spec, amb, self;

	double kr;
	double k3d;

	double width;
	double dx;

};

#endif // !defined(SURFACE_DATA_H)