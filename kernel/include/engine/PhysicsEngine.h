#ifndef PHYSICS_ENGINE
#define PHYSICS_ENGINE

#define Physics     PhysicsEngine::getInstance()

#include "extensions/PxVisualDebuggerExt.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"

#include "math/KernelMath.h"
#include "math/Vector3.h"
#include "math/Ray.h"
#include "math/Geometry.h"

#include "KernelPrerequisites.h"

#include "buffers/RaycastHit.h"

class Scene;

namespace physx
{
	class PxPhysics;
	class PxFoundation;
	class PxProfileZoneManager;
	class PxCooking;
	class PxMaterial;
	class PxVehicleDrivableSurfaceToTireFrictionPairs;
	class PxBatchQuery;
}

class VehicleSceneQueryData;

class KERNEL_VS_EXPORT PhysicsEngine
{
public:
	static PhysicsEngine* getInstance();
	void init();
	void destroy();

	physx::PxPhysics* getPhysics();

	physx::PxCooking* getCooking();

	bool initVehicleSDK(Scene* scene);
	bool destroyVehicleSDK();
	bool isStartedVehicleSDK(){return initiazeVehicleSDK;}

	static unsigned AllLayers();
	static unsigned DefaultRaycastLayers();
	static unsigned IgnoreRaycastLayer();

	void setBounceThreshold(float b);
	float getBounceThreshold();

	void setGravity(const Vector3& g);
	Vector3 getGravity();

	void setMaxAngularVelocity(float m);
	float getMaxAngularVelocity();

	///Retrieves all material bindings.
	std::map<std::string, physx::PxMaterial*>& getOgreMaterialNames()
	{
		return mMaterialBindings;
	}

	//Sets the material bindings.
	void setOgreMaterialNames(const std::map<std::string, physx::PxMaterial*> &bindings)
	{
		mMaterialBindings = bindings;
	}

	/**
	Binds a material name to a PhysX material.
	By default, all material names are bound to the physX default material.
	*/
	void registerOgreMaterialName(const std::string &matName, physx::PxMaterial *material);

	/**
	Returns the material index assigned to the material name if existing, or nullptr.
	*/
	physx::PxMaterial* getOgreMaterialByName(const std::string &materialName);

	physx::PxMaterial* getDefaultMaterial();

	bool raycast(const Vector3& origin, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool raycast(const Vector3& origin, const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool raycast(const Ray& ray, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool raycast(const Ray& ray, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);

	RaycastHitList raycastAll(const Vector3& origin, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	RaycastHitList raycastAll(const Ray& ray, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);

	bool sweep(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	RaycastHitList sweepAll(Geometry* geo,const Vector3& pos, const Quaternion& ori,const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);

	bool sphereCast(const Vector3& origin, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool sphereCast(const Ray& ray, float radius, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool sphereCast(const Ray& ray, float radius, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	RaycastHitList sphereCastAll(const Ray& ray, float radius, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	RaycastHitList sphereCastAll(const Vector3& origin, float radius, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);

	bool capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, RaycastHit& hitInfo, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool capsuleCast(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	RaycastHitList capsuleCastAll(const Vector3& point1, const Vector3& point2, float radius, const Vector3& direction, float distance = Math::InfinityF, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);

	ColliderVector overlapSphere(const Vector3& position, float radius, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool checkSphere(const Vector3& position, float radius, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	bool checkCapsule(const Vector3& start, const Vector3& end, float radius, unsigned layerMask = DefaultRaycastLayers(), Scene* scene = nullptr);
	ColliderVector overlap(Geometry* geo,const Vector3& pos, const Quaternion& ori, unsigned layerMask = Physics->DefaultRaycastLayers(), Scene* scene = nullptr);

	bool lineCast(const Vector3& start, const Vector3& end, RaycastHit& hitInfo, unsigned layerMask = Physics->DefaultRaycastLayers(), Scene* scene = nullptr);
	bool lineCast(const Vector3& start, const Vector3& end, unsigned layerMask = Physics->DefaultRaycastLayers(), Scene* scene = nullptr);

	physx::PxVehicleDrivableSurfaceToTireFrictionPairs* getFrictionPairs(){return gFrictionPairs;}

	bool updateVehicles(physx::PxVehicleDrive4W* gVehicle4W);

private:
	PhysicsEngine();
	~PhysicsEngine();

private:
	static PhysicsEngine* instance;

	physx::PxPhysics* mPhysicsSDK;
	physx::PxFoundation* mFoundation;
	physx::PxDefaultErrorCallback gDefaultErrorCallback;
	physx::PxDefaultAllocator gDefaultAllocatorCallback;
	physx::PxProfileZoneManager* mProfileZoneManager;
	physx::PxCooking* mCooking;
	physx::PxMaterial* mDefaultMaterial;
	physx::PxVisualDebuggerConnection* mConnection;
	
	bool initiazeVehicleSDK;
	VehicleSceneQueryData* gVehicleSceneQueryData;
	physx::PxBatchQuery* gBatchQuery;
	physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs;

	std::map<std::string, physx::PxMaterial*> mMaterialBindings;

	float defaultBounceThreshold;
	float defaultMaxAngularVelocity;
	Vector3 defaultGravity;
};

#endif