///////////////////////////////////////////////////////////
//  Surface.h
//  Implementation of the Class Surface
//  Created on:      08-jul-2013
///////////////////////////////////////////////////////////

#if !defined(SURFACE__H)
#define SURFACE__H

#include "SurfaceData.h"
#include "components/rendering/Camera.h"
#include "PTPrerequisites.h"
#include "engine/PhysicsEngine.h"

enum SurfaceType
{
	OpenPit,
	UnderGround
};


class Surface: public SurfaceData
{

public:

	//! Destrutor
	virtual ~Surface();

	/*!
     * \brief 
     * \param 
     */
	virtual void init() = 0;

	/*!
     * \brief Método testa se posição 3D está dentro do terreno e caso não tiver
        modifica a referência da posição para uma dentro do terreno
     * \param Vector3& worldPosition
		\return bool se está dentro do terreno
     */
    bool isInside(Vector3& worldPosition);

	/*!
     * \brief Metoo que processa um click no terreno
     * \param 
     */
	virtual bool mousePosition(int mx, int my,double& x, double& y, 
														double & z) = 0;
		
	/*!
     * \brief Método que testa se a posição da camera passada está com colisão com terreno
     * \param SceneNodeOld
     * \param height altura
     * \param flyFree bool se voa acima do terreno ou não
     */
	virtual bool isSurfaceCollision(Vector3& oldPos, 
									const double& height, 
									Vector3 &newPos,
									int mask = Physics->DefaultRaycastLayers(),
									bool onlyVisible = true) = 0;
	
	/*!
     * \brief 
     * \param 
     */
	virtual double getHeightAt(const double& vX, const double& vZ, const double& rBonus = 0.0, 
		bool bUseRoot = false, bool bUseGeoMorphing = false, bool onlyVisible = true ) = 0;

	/*!
     * \brief 
     * \param 
     */
    virtual bool getHeightAt(Vector3& vPos, const double& rBonus = 0.0, 
		bool bUseRoot = false, bool bUseGeoMorphing = false, bool onlyVisible = true) = 0;
	
	/*!
     * \brief 
     * \param 
     */
	virtual Vector3 mouseWorldPosition(const double& xMouse, const double& yMouse, 
		const double& wWindow, const double& hWindow) = 0;

	/*!
     * \brief 
     * \param 
     */
	virtual double getOpacity() const = 0;
	
	/*!
     * \brief 
     * \param 
     */
	virtual int getLod();
	
	/*!
     * \brief 
     * \param 
     */
	virtual void setOpacity(const double& alpha) = 0;
	
	/*!
     * \brief 
     * \param 
     */
	virtual void setLod(int lod);
	
	/*!
     * \brief 
     * \param 
     */
	virtual void update() = 0;

	/*!
     * \brief 
     * \param 
     */
	virtual double getHeight() const = 0;

	/*!
     * \brief 
     * \param 
     */
	virtual void createMaterial(const QString& name) = 0;

	/*!
     * \brief 
     * \param 
     */
	virtual void setMaterial(const std::string& materialName, unsigned idEnt = 0) = 0;

	/*!
     * \brief 
     * \param 
     */
	virtual Ogre::MaterialPtr getMaterial(unsigned i = 0 ) = 0;

	/*!
     * \brief Retorna o número de materiais
     * \param 
     */
	virtual unsigned int getNumMaterials() const = 0;	

	/*!
		\brief
	*/
	virtual bool thisVisible(const Vector3&,const QString&);

	/*!
		\brief Usado para configurar a cor do terreno ou o material padrão
	*/
	virtual void configureSurfaceMaterial(bool isColor, const QColor& color) = 0;

	/*!
		\brief Usado para trocar a cor do terreno
	*/
	virtual void configureSurfaceColor(const QColor& color) = 0;
		
	/*!
     * \brief
     * \param 
     */
	virtual Vector3 getCenterPosition() const = 0;	

	virtual void setWireFrame(bool active) = 0;
	virtual bool isWireFrame() const = 0;

	virtual void setSolidFrame(bool active) = 0;
	virtual bool isSolidFrame() const = 0;

	virtual void configureEffectiveMining() = 0;

	virtual HEIGHTMAPTYPE* getHeightMapData() = 0;

	virtual size_t getHeightMapWidth() = 0;

	//! Construtor de copia
   Surface& operator = (const SurfaceData& );

protected:

	//! Construtor
	Surface();
	
	//! Construtor de copia
    Surface(const Surface&);

	//! operator de atribuição
    Surface& operator = (const Surface&);

	/*!
     * \brief
     * \param 
     */
	void setSurfaceData(const SurfaceData& v);
};

#endif // !defined(SURFACE__H)

