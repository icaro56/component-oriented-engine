#ifndef LAYER_MASK_H
#define LAYER_MASK_H

#include "KernelPrerequisites.h"
#include "foundation/PxSimpletypes.h"

//32 layers no total
class KERNEL_VS_EXPORT LayerMask
{
public:
	~LayerMask();

	void setValue(int v);
	int value();
	static std::string layerToName(int i);
	static int nameToLayer(const std::string& name);
	static int nameToLayerInt(const std::string& name);
	static void createLayer(const std::string& name);
	static void initDefault();

	//LayerMask operator =(int arg);

	static const physx::PxU32* groupCollisionFlags();
	static void updateGroupCollisionFlags();
	static void updateGroupCollisionFlag(int id);

	//métodos só funcionam quando chamados antes de criar uma cena.
	static void setLayerCollision(const std::string& layer, unsigned collisionFlags);
	static void setLayerCollision(const std::string& layer, const std::string& otherLayer, bool collide);

private:
	LayerMask();
	static void updateEnvolvingGroups(unsigned i, unsigned collisionFlags);
	unsigned layer;
	unsigned layerInt;
	unsigned myCollisions;

	typedef std::vector<std::pair<std::string, LayerMask>> ListLayer;
	
	static ListLayer allLayers;
	static physx::PxU32 flags[32];
};

#endif