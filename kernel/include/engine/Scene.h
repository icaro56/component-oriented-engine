#ifndef SCENE_H
#define SCENE_H

#include "KernelPrerequisites.h"
#include "OgreSceneManager.h"
#include "utils/Cooker.h"
#include "foundation/PxTransform.h"
#include "PxQueryReport.h"
#include "callback/SimulationEventCallback.h"
#include "callback/CharacterControllerCallback.h"
#include "callback/SceneQueryEventCallback.h"

#include "math/Vector3.h"
#include "math/KernelMath.h"
#include "math/Ray.h"
#include "math/Geometry.h"

#include "engine/PhysicsEngine.h"

#include "OgreSceneQuery.h"

class GameObject;
class Transform;

namespace Ogre
{
	class Root;
}

namespace MOC
{
	class CollisionTools;
}

namespace physx
{
	class PxScene;
	class PxRigidStatic;
	class PxGeometry; 
	class PxMaterial;
	class PxRigidDynamic;
	class PxRigidActor;
	class PxShape;
	class PxControllerManager;
	class PxController;
	class PxCapsuleController;
	class PxCapsuleControllerDesc;
	class PxBoxControllerDesc;
}

class KERNEL_VS_EXPORT Scene
{
	friend class Engine;
	friend class GameObjectManager;
	friend class GameObject;
public:

	Ogre::SceneManager* getOgreScene();
	physx::PxScene* getPhysxScene();
	
	void destroyGameObject(GameObject* gameObject);
	void destroyAllGameObjects();
	GameObject* findGameObject(const std::string& name);

	std::string getName() const;

	void updateGameObjects();
	void lateUpdateGameObjects();
	void update(float dt);
	void fixedUpdateGameObject();
	bool simulate(float dt);

	// Ativa sombras dinâmicas e define a técnica a ser usada.
    /**
    * @brief Ativa sombras dinâmicas e define a técnica a ser usada.
    * @param type ShadowTechnique pode ser SHADOW_NONE ou SHADOW_STENCIL ou
    * SHADOW_TEXTURE
    */
    void setShadowType(ShadowTechnique type, int textureSize = 512);

    // Informa a partir de qual distancia da camera, as sombras dos
    //meshs serão calculadas
    /**
    * @brief Informa a partir de qual distancia da camera, as sombras
    dos meshs serão calculadas
    * @param distance distância da camera em relação aos meshs.
    */
    void setShadowDistance(float distance = 1000);

    // Define a cor das sombras.
    /**
    * @brief Define a cor das sombras.
    * @param r 0 minimo e 1 maximo
    * @param g 0 minimo e 1 maximo
    * @param b 0 minimo e 1 maximo.
    */
    void setShadowColor(double r, double g, double b);

	/**
    * @brief Desativa a neblina na cena.
    */
    void disableFog();

	// Define um skybox (imagem de fundo).
    /**
     * @brief Define um skybox (imagem de fundo).
     * @remarks A partir de um objeto Ogre::MaterialManager a textura
     passada como parametro
     * é regulada para se tornar a imagem de fundo da cena, isso é feito
     pelo metodo setSkyBox()
     * da classe Ogre::SceneManager
     * @param texture Nome das texturas a serem usadas.
     * @param distance Distância do skybox à camera. Independente desta
     distância,
     * o skybox será desenhado por trás de tudo.
     */
    void setSkybox(const std::string &texture, double distance = 1);

    // Define um skydome (imagem de fundo).
    /**
     * @brief Define um skydome (céu de fundo animado).
     * @remarks A partir de um objeto Ogre::MaterialManager a textura
     passada como parametro
     * é regulada para se tornar a imagem de fundo da cena, isso é feito
     pelo metodo setSkyDome()
     * da classe Ogre::SceneManager
     * @param texture Nome das texturas a serem usadas.
     * @param curvature Curvaruta do skydome
     * @param tilling quantidade de vezes que a textura é repetida
     * @param distance Distância do skybox à camera. Independente desta
     distância,
     * o skydome será desenhado por trás de tudo.
     */
    void setSkyDome(const std::string &texture, bool enable = true,
                    double curvature = 10, double tilling = 8,
                    double distance=4000);

    // Define a neblina (cor, distância onde ela começa e distância onde
    //ela é máxima).
    /**
    * @brief Define a neblina
    * @param colorR 0 minimo e 1 maximo
    * @param colorG 0 minimo e 1 maximo
    * @param colorB 0 minimo e 1 maximo
    * @param distNear Distancia onde ela começa
    * @param distFar Distância onde ela é máxima
    */
    void setFog(double colorR, double colorG, double colorB,
                double distNear, double distFar);

    // Define a luz ambiente.
    /**
    * @brief Define a luz ambiente
    * @param r 0 minimo e 1 maximo
    * @param g 0 minimo e 1 maximo
    * @param b 0 minimo e 1 maximo.
    */
    void setAmbientLight(double r, double g, double b);

	bool isColliding(const Ogre::Vector3 &oldPos, const Ogre::Vector3 &newPos, const float &radius, const unsigned &mask, const std::string &idName, const bool bounding = false);
	MOC::CollisionTools *getCollisionTools(){return collisionTools;}

	void createPlane(const physx::PxVec3& normal = physx::PxVec3(0,1,0), float distance = 0);

	physx::PxShape* createCapsuleShape( physx::PxRigidActor* actor,
		float radius,
		float height,
		float density,
		const physx::PxTransform &shapePose);

	physx::PxShape* createCapsuleShape( physx::PxRigidActor* actor,
		float radius,
		float height,
		float density,
		physx::PxMaterial &material,
		const physx::PxTransform &shapePose);

	physx::PxShape* createSphereShape( physx::PxRigidActor* actor,
									   float radius,
									   float density,
									   const physx::PxTransform &shapePose);

	physx::PxShape* createSphereShape( physx::PxRigidActor* actor,
		float radius,
		float density,
		physx::PxMaterial &material,
		const physx::PxTransform &shapePose);

	physx::PxShape* createBoxShape(	physx::PxRigidActor* actor, 
		                            const physx::PxVec3& dim, 
									float density,
									const physx::PxTransform &shapePose);

	physx::PxShape* createBoxShape(	physx::PxRigidActor* actor, 
		                            const physx::PxVec3& dim,
									float density,
									physx::PxMaterial &material,
									const physx::PxTransform &shapePose);

	physx::PxShape* createHeightFieldShape(physx::PxRigidActor* actor, physx::PxHeightField* desc, const Vector3& scale);

	physx::PxRigidStatic* createRigidStatic(const physx::PxTransform &actorPose = physx::PxTransform::createIdentity());

	physx::PxRigidDynamic* createRigidDynamic(const physx::PxTransform &actorPose = physx::PxTransform::createIdentity());

	// scene queries do rendering
	RaycastHitList volumecastRendering(const Ray& topLeft, const Ray& topRight, const Ray& bottomLeft , const Ray& bottomRight, unsigned layerMask = Physics->DefaultRaycastLayers());

	void initRayQueries();

	bool raycastRendering(const Ray& ray, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool raycastRendering(const Ray& ray, RaycastHit& hitInfo, unsigned layerMask = Physics->DefaultRaycastLayers());
	RaycastHitList raycastRenderingAll(const Ray& ray, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());

	// scene queries de física. Leva em consideração os colliders
	bool raycast(const Vector3& origin, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hitInfo,float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool raycast(const Ray& ray, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool raycast(const Ray& ray, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());

	RaycastHitList raycastAll(const Vector3& origin, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	RaycastHitList raycastAll(const Ray& ray, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());

	bool sweep(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	RaycastHitList sweepAll(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());

	bool sphereCast(const Vector3& origin, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool sphereCast(const Ray& ray, float radius, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool sphereCast(const Ray& ray, float radius, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	RaycastHitList sphereCastAll(const Ray& ray, float radius, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	RaycastHitList sphereCastAll(const Vector3& origin, float radius, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());

	bool capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());
	RaycastHitList capsuleCastAll(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = Physics->DefaultRaycastLayers());

	ColliderVector overlapSphere(const Vector3& position, float radius, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool checkSphere(const Vector3& position, float radius, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool checkCapsule(const Vector3& start, const Vector3& end, float radius, unsigned layerMask = Physics->DefaultRaycastLayers());
	ColliderVector overlap(Geometry* geo,const Vector3& pos, const Quaternion& ori, unsigned layerMask = Physics->DefaultRaycastLayers());

	bool lineCast(const Vector3& start, const Vector3& end, RaycastHit& hitInfo, unsigned layerMask = Physics->DefaultRaycastLayers());
	bool lineCast(const Vector3& start, const Vector3& end, unsigned layerMask = Physics->DefaultRaycastLayers());


	physx::PxShape* createConvexMeshShape(physx::PxRigidActor* actor, Ogre::Entity *entity, float density, physx::PxMaterial &material, const Vector3 &scale, const physx::PxTransform &shapePos);

	physx::PxShape* createTriangleMeshShape(physx::PxRigidActor* actor, Ogre::Entity *entity, Cooker::Params &cookerParams);

	Transform* getRootTransform();

	void createRootTransform();
	SimulationEventCallback& getSimulationEventCallback();
	CharacterControllerCallback& getCharacterControllerCallback();
	SceneQueryEventCallback& getSceneQueryEventCallback();

	void initControllerManager();
	void destroyControllerManager();

	physx::PxController* createCharacterController(const physx::PxCapsuleControllerDesc& capsuleDesc);
	physx::PxController* createCharacterController(const physx::PxBoxControllerDesc& boxDesc);

private:
	Scene(const std::string& name, Ogre::SceneType typeMask = Ogre::ST_EXTERIOR_CLOSE);
	~Scene();

	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);
	void clear();

	//física
	physx::PxShape* createShape(physx::PxRigidActor* actor, 
								physx::PxGeometry &geometry, 
								physx::PxMaterial &material, 
								const physx::PxTransform &shapePose = physx::PxTransform::createIdentity(),
								float density = 1.0f);
	
	
	
private:
	Ogre::SceneManager* mSceneMgr;

	///Ferramenta para testar colisão diretamente no mesh
	MOC::CollisionTools *collisionTools;
	
	// RaySceneQuery
	Ogre::RaySceneQuery *mRaySceneQuery;

	//seleção por área
	Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery;

	//***************************** Física
	physx::PxScene* mPxScene;
	float mTimeAccumulator;

	typedef std::map<const std::string, GameObject*> GameObjectMap;

	//gameobjects dessa cena
	GameObjectMap gameObjects;

	GameObject* rootGameObject;
	Transform* rootTransform ;

	SimulationEventCallback mySimulationEventCallback;
	physx::PxControllerManager* controllerManager;
	CharacterControllerCallback myCharacterControllerCallback;
	SceneQueryEventCallback mySceneQueryEventCallback;
};

#endif