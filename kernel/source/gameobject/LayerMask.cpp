#include "gameobject/LayerMask.h"

LayerMask::ListLayer LayerMask::allLayers;
physx::PxU32 LayerMask::flags[32];

LayerMask::LayerMask()
	: layerInt(0),
	  layer(1 << 0),
	  myCollisions(31)
{

}

LayerMask::~LayerMask()
{

}

void LayerMask::setValue(int v)
{
	this->layer = 1 << v;
	this->layerInt = v;
}

std::string LayerMask::layerToName(int i)
{
	auto it = allLayers.begin();
	for(; it != allLayers.end(); ++it)
	{
		if (it->second.layerInt == i)
		{
			return it->first;
		}
	}

	return "";
}

int LayerMask::nameToLayer(const std::string& name)
{
	auto it = allLayers.begin();
	for (; it != allLayers.end(); it++)
	{
		if (it->first == name)
		{
			return it->second.layer;
		}
	}

	return -1;
}

int LayerMask::nameToLayerInt(const std::string& name)
{
	auto it = allLayers.begin();
	for (; it != allLayers.end(); it++)
	{
		if (it->first == name)
		{
			return it->second.layerInt;
		}
	}

	return -1;
}

void LayerMask::createLayer(const std::string& name)
{
	if (allLayers.size() < 32)
	{
		for(auto it = allLayers.begin(); it != allLayers.end(); ++it)
		{
			if (it->first == name)
			{
				return;
			}
		}

		LayerMask mask;
		mask.setValue(allLayers.size());

		allLayers.push_back(std::make_pair(name, mask));
	}
}

int LayerMask::value()
{
	return layerInt;
}

const physx::PxU32* LayerMask::groupCollisionFlags()
{
	return flags;
}

void LayerMask::updateGroupCollisionFlags()
{
	int i = 0;
	for(auto it = allLayers.begin(); it != allLayers.end(); ++it, i++)
	{
		flags[i] = it->second.myCollisions;
	}
}

void LayerMask::updateGroupCollisionFlag(int id)
{
	if (id < 32)
	{
		flags[id] = allLayers.at(id).second.myCollisions;
	}
}

void LayerMask::initDefault()
{
	for (int i = 0; i < 32; i++)
	{
		//colide com todos os outros layers por padrão
		flags[i] = 0xffffffff;
	}

	createLayer("Default");
	createLayer("IgnoreRaycast");
	createLayer("Water");
	createLayer("Layer1");
	createLayer("Layer2");
	createLayer("Chassis");
	createLayer("Wheels");

	setLayerCollision("Default", "Wheels", false);
}

void LayerMask::setLayerCollision(const std::string& layer, unsigned collisionFlags)
{
	unsigned i = nameToLayerInt(layer);

	if (i >= 0)
	{
		allLayers.at(i).second.myCollisions = collisionFlags;
		updateGroupCollisionFlag(i);

		updateEnvolvingGroups( i, collisionFlags);
	}
}

void LayerMask::setLayerCollision(const std::string& layer, const std::string& otherLayer, bool collide)
{
	unsigned i = nameToLayerInt(layer);
	unsigned i2 = nameToLayerInt(otherLayer);

	if (i >= 0 && i2 >= 0)
	{
		if (collide)
		{
			allLayers.at(i).second.myCollisions = allLayers.at(i).second.myCollisions | allLayers.at(i2).second.layer;
			allLayers.at(i2).second.myCollisions = allLayers.at(i2).second.myCollisions | allLayers.at(i).second.layer;
		}
		else
		{
			allLayers.at(i).second.myCollisions = allLayers.at(i).second.myCollisions & ~allLayers.at(i2).second.layer;
			allLayers.at(i2).second.myCollisions = allLayers.at(i2).second.myCollisions & ~allLayers.at(i).second.layer;
		}

		updateGroupCollisionFlag(i);
		updateGroupCollisionFlag(i2);
	}
}

void LayerMask::updateEnvolvingGroups(unsigned i, unsigned collisionFlags)
{
	unsigned otherLayer = 1 << i;

	int inc = 0;
	for(auto it = allLayers.begin(); it != allLayers.end(); ++it, inc++)
	{
		if (inc != i)
		{
			unsigned colidiu = collisionFlags & it->second.layer;

			if (colidiu)
				it->second.myCollisions = it->second.myCollisions | otherLayer;
			else
				it->second.myCollisions = it->second.myCollisions & ~otherLayer;
		}
		
	}

	updateGroupCollisionFlags();
}

