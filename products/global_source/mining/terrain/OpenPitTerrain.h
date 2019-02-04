///////////////////////////////////////////////////////////
//  OpenPitTerrain.h
//  Implementation of the Class OpenPitTerrain
//  Created on:      08-jul-2013 
///////////////////////////////////////////////////////////

#if !defined(OPEN_PIT_TERRAIN_H)
#define OPEN_PIT_TERRAIN_H

#include "surface/Surface.h"

#include "OgreImage.h"
#include "OgreTexture.h"

class Terrain;

class OpenPitTerrain: public Surface
{

public:

	//! Construtor
	OpenPitTerrain();

	//! Destrutor
	virtual ~OpenPitTerrain();

	/*!
     * \brief Inicia terreno principal
     * \param 
     */
	void init();

	/*!
     * \brief 
     * \param 
     */
	void update();

	/*!
     * \brief Metoo que processa um click no terreno
     * \param 
     */
	bool mousePosition(int mx, int my,double& x, double& y,	double & z);

	/*!
     * \brief Método que testa se a posição da camera passada está com colisão com terreno
     * \param SceneNodeOld
     * \param height altura
     * \param flyFree bool se voa acima do terreno ou não
     */
	bool isSurfaceCollision(Vector3& oldPos, 
							const double& height, 
							Vector3 &newPos,
							int mask = Physics->DefaultRaycastLayers(),
							bool onlyVisible = true);
		
	/*!
     * \brief 
     * \param 
     */
	double getHeightAt( const double& vX, const double& vZ, const double& rBonus = 0.0, 
		bool bUseRoot = false, bool bUseGeoMorphing = false, bool onlyVisible = true );

	/*!
     * \brief 
     * \param 
     */
    bool getHeightAt(Vector3& vPos, const double& rBonus = 0.0, 
		bool bUseRoot = false, bool bUseGeoMorphing = false, bool onlyVisible = true);

	/*!
     * \brief 
     * \param 
     */
	Vector3 mouseWorldPosition(const double& xMouse, const double& yMouse, 
		const double& wWindow, const double& hWindow);

	/*!
     * \brief 
     * \param 
     */
	double getOpacity() const;

	/*!
     * \brief 
     * \param 
     */
	void setOpacity(const double& alpha);		

	/*!
     * \brief 
     * \param 
     */
	void setLod( int l);	
	/*!
     * \brief 
     * \param 
     */
	int getLod();

	/*!
     * \brief 
     * \param 
     */
	double getHeight() const;

	/*!
     * \brief Configura o material do terreno. O segundo parametro não precisa
	 ser informado nessa classe filha
     * \param 
     */
	void setMaterial(const std::string& materialName, unsigned idEnt = 0);

	/*!
     * \brief Retorna o material do terreno. Esse terreno só possui um material
	 e não é necessário preencher esse parâmetro
     * \param 
     */
	Ogre::MaterialPtr getMaterial(unsigned i = 0 );

	/*!
     * \brief 
     * \param 
     */
	unsigned int getNumMaterials() const;

	/*!
     * \brief 
     * \param 
     */
	void createMaterial(const QString& name);

	/*!
		\brief
	*/
	bool thisVisible(const Vector3&,const QString&);

	/*!
		\brief Usado para configurar a cor do terreno
	*/
	void configureSurfaceMaterial(bool isColor, const QColor& color);

	/*!
		\brief Usado para trocar a cor do terreno
	*/
	void configureSurfaceColor(const QColor& color);

	/*!
     * \brief
     * \param 
     */
	Vector3 getCenterPosition() const;

	/*!
		\brief
	*/
	void setWireFrame(bool active);

	/*!
		\brief
	*/
	bool isWireFrame() const;

	/*!
		\brief
	*/
	void setSolidFrame(bool active);

	/*!
		\brief
	*/
	bool isSolidFrame() const;

	void configureEffectiveMining();

	HEIGHTMAPTYPE* getHeightMapData();

	size_t getHeightMapWidth();

private:
		
	//! Construtor de copia
    OpenPitTerrain(const OpenPitTerrain&);

	//! operator de atribuição
    OpenPitTerrain& operator = (const OpenPitTerrain&);

	/*!
     * \brief Método chamado para contruir o mesh do terreno.
     * \param 
     */
    void buildTerrainMesh();

private:
	 int lod;
	 Terrain * terrain;

	 //Usados para o cálculo de efetividade de lavra
	 Ogre::Image mImg;
     Ogre::TexturePtr texTopologic;
};

#endif // !defined(OPEN_PIT_TERRAIN_H)