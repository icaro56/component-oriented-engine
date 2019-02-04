///////////////////////////////////////////////////////////
//  SurfaceManager.h
//  Implementation of the Class SurfaceManager
//  Created on:      08-jul-2013
///////////////////////////////////////////////////////////

#if !defined(SURFACE_MANAGER_H)
#define SURFACE_MANAGER_H

#include <QHash>
#include <QMutex>
#include <QStringList>
#include <engine\RenderView.h>
#include "components/rendering/Camera.h"
#include "PTPrerequisites.h"
#include "engine/PhysicsEngine.h"

class Surface;
class SurfacePath;
class SurfaceData;
class SurfaceListerner;
class SurfacePathManager;
class Camera;

namespace Ogre
{
	class Vector3;
};

class SceneNodeOld;

class SurfaceManager
{

public:	

	/*!
     * \brief 
     */
	static SurfaceManager* getInstance();	

	/*!
     * \brief 
     */
	static void destroy();

	/*!
     * \brief 
     */
	void init();

	/*!
     * \brief 
     */
	void update();

///////////////////////////////////////////////////////////////////////////////
	
	/*!
	 * \brief retorna o nome do terreno corrente
	 * \return QString  
     */
	unsigned int idCurrentSurface() const;

	/*!
	 * \brief retorna o nome do terreno corrente
	 * \return QString  
     */
	const QHash<int ,const SurfaceData*>  getDataInfo() const;

///////////////////////////////////////////////////////////////////////////////

	/*!
     * \brief adiciona um arquivo de terreno na aplicação 
	 * \param nome do terreno na aplicação 
	 * \param file nome do arquivo de terreno
     */
	void addSurface(const QString& name,const QString& file);

	/*!
     * \brief remove o terreno da aplicação
	 * \param idSurface, identificador do terreno caso não tenha 
	 * paramentro se considera o terreno corrente
     */
	void removeSurface(unsigned int idSurface = 0);	

	/*!
     * \brief exportar um terreno para um arquivo
	 * \param name do terreno deve esta na lista de terrenos 
	 * \param path pasta aonde o arquivo vai ser chamado
     */
	void exportSurface(const QString& name,const QString& path);

	/*!
     * \brief importa uma lista de terreno
	 * \param
	 * \param 
     */
	void importSurface(unsigned int path,const QStringList& list);

	/*!
     * \brief carrega um terreno na aplicação
	 * \param const QString idSurface 
     */
	bool loadSurface(unsigned int idSurface);

	/*!
     * \brief descarrega um terreno da aplicação 
	 * \param const QString idSurface  
     */
	void unLoadSurface(unsigned int idSurface);
	
	/*!
	 * \brief modifica o terreno corrente da aplicação 
	 * \param const QString idSurface 
     */
	void swapSurface(unsigned int idSurface);
	
	/*!
	 * \brief 
	 * \return   
     */
	void setAttributeDataSurface(unsigned int idSurface,unsigned int,
		const QString&);

	/*!
	 * \brief 
	 * \return   
     */
	QString getAttributeDataSurface(unsigned int idSurface,unsigned int);
	
	/*!
	 * \brief 
	 * \return   
     */
	QList<const SurfacePath*> getPathsSurface() const;

	/*!
	 * \brief 
	 * \return   
     */
	void addPathSurface(const SurfacePath& path);

	/*!
	 * \brief 
	 * \return   
     */
	void removePathSurface(const QList<unsigned int>& list);
	
	/*!
	 * \brief 
	 * \return   
     */
	void editPathSurface(const SurfacePath& path);

///////////////////////////////////////////////////////////////////////////////
		
	/*!
     * \brief retorna distância (altura) em relação ao terreno
     * \param vX posição x 3D 
     * \param vZ posição z 3D
     * \param rBonus incrementa um valor no resultado
     * \param bUseRoot
     * \param bUseGeoMorphing
     * \param onlyVisible considerar a visibilidade do terreno 
     * \param idSurface identificador do terreno para cálculo
     * \return double distância em relação ao terreno
     */
	double getHeightAt( const double& vX, const double& vZ, const double& rBonus = 0.0, 
		bool bUseRoot = false, bool bUseGeoMorphing = false, 
		bool onlyVisible = true, unsigned int idSurface = 0);

	/*!
     * \brief determina uma posição relativa ao terreno
     * \param vPos posição no mundo 3D
     * \param rBonus incrementa um valor no resultado
     * \param bUseRoot
     * \param bUseGeoMorphing
     * \param onlyVisible considerar a visibilidade do terreno 
     * \param idSurface identificador do terreno para cálculo
     * \return bool true se está posição é válida
     */
    bool getHeightAt(Vector3& vPos, const double& rBonus = 0.0, 
		bool bUseRoot = false, bool bUseGeoMorphing = false, 
		bool onlyVisible = true, unsigned int idSurface = 0);
/*!
     * \brief Metoo que processa um click no terreno
     * \param 
     */
	bool mousePosition(int mx, int my,double& x, double& y, 
									double & z,unsigned int idSurface = 0);


	/*!
	 * \brief determina se houve uma colisão com o terreno
	 * \param  oldPos 
	 * \param  height 
	 * \param  newPos 
	 * \param  mask 
	 * \param  onlyVisible 
	 * \param  idSurface 
	 * \return true se bouve uma colisão
     */
	bool isSurfaceCollision(Vector3& oldPos, const double& height, 
							Vector3 &newPos, 
							int mask = Physics->DefaultRaycastLayers(),
							bool onlyVisible = true, unsigned int idSurface = 0);
		
	/*!
	 * \brief determina um ponto de colisão atraves no mouse
	 * \param  xMouse 
	 * \param  yMouse 
	 * \param  wWindow 
	 * \param  hWindow 
	 * \param  activeCam 
	 * \param  idSurface 
	 * \return Vector3 ponto de colisão
     */
	Vector3 mouseWorldPosition(const double& xMouse, const double& yMouse, 
		const double& wWindow, const double& hWindow, unsigned int idSurface = 0);

	/*!
		* \brief 
		* \param  v 
		* \param  name 		
		* \param  idSurface 		 
		* \return
		*/
	bool thisVisible(const Vector3& v,const QString& name,
		unsigned int idSurface = 0);

	/*!
     * \brief Função que retorna se um valor esta nas coordenadas da mina
     * \param int 1 - x y
     * \param double valor
     * \return QVector<double> contendo as dimensões do terreno
	 \warning Rever necessidade desta função
     */
	bool isMine(int,const double&,unsigned int idSurface = 0) const;

	/*!
        \brief Método retorna se determinado ponto está em cima ou
        fora do terreno.
        \param point const Vector3& ponto real a qual se quer fazer o teste.
        \return bool se está ou não em cima do terreno.
		\warning Rever necessidade desta função
    */
    bool inside(const Vector3& point,unsigned int idSurface = 0);

	/*!
     * \brief Método configura a posição passada para ficar dentro
        do terreno
     * \param node SceneNodeOld*
     */
    //void surfaceBounds(SceneNode* node,unsigned int idSurface = 0);

	/*!
     * \brief Método testa se posição 3D está dentro do terreno e caso não tiver
        modifica a referência da posição para uma dentro do terreno
     * \param Vector3& worldPosition
		\return bool se está dentro do terreno
     */
    bool isInside(Vector3& worldPosition,unsigned int idSurface = 0);

///////////////////////////////////////////////////////////////////////////////
	
	/*!
     * \brief  retorna o lod do terreno
     * \param idSurface identificador do terreno
     */
	int getLod(unsigned int idSurface = 0);

	/*!
     * \brief  retorna a opacidade do terreno
     * \param idSurface identificador do terreno
     */
	double getOpacity(unsigned int idSurface = 0);
	
	/*!
     * \brief modifca a opacidade do terreno
     * \param idSurface identificador do terreno
     */
	void setOpacity(const double& alpha, unsigned int idSurface = 0);
	
	/*!
     * \brief modifca a resolucao do terreno
     * \param idSurface identificador do terreno
     */

	void setLod( int l,unsigned int idSurface = 0);

	/*!
		\brief Usado para configurar a cor do terreno ou o material padrão
	*/
	void configureSurfaceMaterial(bool isColor, const QColor& color,
		unsigned int idSurface = 0);

	/*!
		\brief Usado para trocar a cor do terreno
	*/
	void configureSurfaceColor(const QColor& color, unsigned int idSurface = 0);

	/*!
	 * \brief 
	 * \return   
     */
	void setWireFrame(bool actived, unsigned int idSurface = 0);
	
	/*!
	 * \brief 
	 * \return   
     */
	bool isWireFrame(unsigned int idSurface = 0);
	
	/*!
	 * \brief 
	 * \return   
     */
	void setSolidFrame(bool actived, unsigned int idSurface = 0);
	
	/*!
	 * \brief 
	 * \return   
     */
	bool isSolidFrame(unsigned int idSurface = 0);

	/*!
	 * \brief 
	 * \return   
     */
	void configureEffectiveMining(unsigned int idSurface = 0);

///////////////////////////////////////////////////////////////////////////////
		
	/*!
     * \brief constante de transformação de real para 3D
	 * \param idSurface, identificador do terreno caso não tenha 
	 * paramentro se considera o terreno corrente
     */
	double kRealTo3D(unsigned int idSurface = 0) const;

	/*!
     * \brief constante de transformação de real para 3D
	 * \param idSurface, identificador do terreno caso não tenha 
	 * paramentro se considera o terreno corrente
     */
	double k3DToReal(unsigned int idSurface = 0) const;

	/*!
     * \brief Método utilizado para converter pontos 3D a partir de um ponto no mundo real
     * \param point const Vector3& ponto 3D em coordenadas do mundo 3D
     * \return Vector3 contendo ponto 3D correspondente no mundo real.
     */
    void convertRealTo3D(Vector3& point,
		unsigned int idSurface = 0) const;

	/*!
     * \brief Método utilizado para converter pontos 3D a partir de
        um ponto no mundo real 
     * \param point const Vector3& ponto 3D em coordenadas do mundo 3D
	 * \return Vector3 contendo ponto 3D correspondente no mundo real.
     */
	void convertRealTo3D(double& x,double& y,double& z,
		unsigned int idSurface = 0) const;

	/*!
     * \brief Método utilizado para converter pontos 3D a partir de
        um ponto no mundo 3D
     * \param point const Vector3& ponto 3D em coordenadas do mundo
        real
	 * \return Vector3 contendo ponto 3D correspondente no mundo 3D.
     */
    void convert3DToReal(Vector3& point,
		unsigned int idSurface = 0) const;
	
	/*!
     * \brief Método utilizado para converter pontos 3D a partir de
        um ponto no mundo 3D
     * \param point const Vector3& ponto 3D em coordenadas do mundo
        real
	 * \return Vector3 contendo ponto 3D correspondente no mundo 3D.
     */
	void convert3DToReal(double& x,double& y,double& z,
		unsigned int idSurface = 0) const;

	/*!
     * \brief Função que retorna as dimensões reais do terreno
     * \return QVector<double> contendo as dimensões do terreno
     */
    QList<double> getDimensionReal(unsigned int idSurface = 0) const;

    /*!
     * \brief Função que retorna as dimensões virtuais do terreno
     * \return QVector<double> contendo as dimensões do terreno
     */
    QList<double> getDimension3D(unsigned int idSurface = 0) const;
    
	/*!
     * \brief Função que retorna a diferença de X2 e X1 do terreno
     */
	double getWidth(unsigned int idSurface = 0) const;	

	/*!
     * \brief 
     * \param 
     */
	double getSurfaceHeight(unsigned int idSurface = 0) const;	

	/*!
		\brief Retorna a posição 3D central do terreno
	*/
	Vector3 getCenterPosition(unsigned int idSurface = 0) const;  
		
	/*!
     * \brief Função que retorna a diferença de X2 e X1 do terreno
     */
	double getDiffXReal(unsigned int idSurface = 0) const;

	HEIGHTMAPTYPE* getHeightMapData(unsigned int idSurface = 0);

	size_t getHeightMapWidth(unsigned int idSurface = 0);

///////////////////////////////////////////////////////////////////////////////

private:

	//! Construtor 
    SurfaceManager();
	
	//! Construtor de copia
    SurfaceManager(const SurfaceManager&);

	//! Destrutor 
    ~SurfaceManager();	

	//! operator de atribuição
    SurfaceManager& operator = (const SurfaceManager&);	

	/*!
     * \brief verifica se tem um terreno current carregado
	 * \return bool true se tem carregado 
     */
	bool hasSurfaceCurrent() const;

	/*!
     * \brief verifica se o nome do terreno é válido
	 * \param QString name do terreno 
	 * \return bool true se tem carregado 
     */
	Surface * getSurface(unsigned int idSurface = 0) const;

	/*!
     * \brief verifica se o SurfaceManger foi iniciado
     */
	void isInitiated() const;

	/*!
     * \brief verifica se o arquivo de configuração
     */
	QString fileConfiguration() const;

	/*!
     * \brief verifica se o arquivo de configuração
     */
	void fileConfigurationSave() const;

	/*!
     * \brief verifica se o arquivo de configuração
     */
	void fileConfigurationDelete(unsigned int idSurface = 0) const;

private:

	bool isInit;
	bool isInitListerner;

	static SurfaceManager* instance;
	
	Surface * current;

	QHash<int ,SurfaceData*> listSurfaceData;

	QHash<int ,Surface*> listLoad;

	SurfaceListerner * listerner;

	SurfacePathManager * pathManager;

	QMutex mutex;

};

#endif // !defined(SURFACE_MANAGER_H)