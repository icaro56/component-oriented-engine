/*
This source file is part of OgrePhysX.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include "KernelPrerequisites.h"
#include "OgreString.h"
#include "foundation/PxVec3.h"
#include "common/PxCoreUtilityTypes.h"
#include "math/Vector3.h"

namespace physx
{
	class PxTriangleMesh;
	class PxConvexMesh;
	class PxMaterial;
	class PxOutputStream;
}

class KERNEL_VS_EXPORT Cooker
{
private:
	Ogre::String mOgreResourceGroup;

public:

	class KERNEL_VS_EXPORT Params
	{
	public:
		Vector3 mScale;
		std::map<Ogre::String, physx::PxMaterial*> mMaterialBindings;
		bool mAddBackfaces;

		Params() : mScale(Vector3(1,1,1)), mAddBackfaces(false) {}
		~Params() {}

		Params& scale(const Vector3 &scale) { mScale = scale; return *this; }
		Params& materials(std::map<Ogre::String, physx::PxMaterial*> &bindings) { mMaterialBindings = bindings; return *this; }
		Params& backfaces(bool addBackfaces) { mAddBackfaces = addBackfaces; return *this; }
	};

	class AddedMaterials
	{
	public:
		physx::PxMaterial **materials;
		physx::PxU32  materialCount; 

		AddedMaterials() : materials(nullptr) {}
		~AddedMaterials() { if (materials) delete[] materials; }
	};

	struct MeshInfo
	{
		//vertex buffer
		std::vector<Vector3> vertices;

		//index buffer
		std::vector<int> indices;

		//assigns a material to each triangle.
		std::vector<Ogre::String> materials;
	};

	void getMeshInfo(Ogre::MeshPtr mesh, Params &params, MeshInfo &outInfo);
	void mergeVertices(MeshInfo &outInfo, float fMergeDist = 1e-3f);
	void insetMesh(MeshInfo &outInfo, float fAmount);

	Cooker(void);
	~Cooker(void);

	/*
	setOgreResourceGroup
	Tells OgrePhysX in which ogre resource group it is supposed to look for resources.
	By default, this is set to "General".
	*/
	void setOgreResourceGroup(Ogre::String group);

	/*
	hasPxMesh
	Checks whether the Ogre Resource System has a resource PxsFile.
	*/
	bool hasPxMesh(Ogre::String PxsFile);
	/*
	loadPxMeshFromFile
	Loads a PhysX triangle mesh from a Pxs file. Throws an exception if the file is not found in the Ogre resource system.
	*/
	physx::PxTriangleMesh* loadPxTriangleMeshFromFile(Ogre::String PxsFile);

	/*
	cookPxTriangleMesh
	cooks an PhysX triangle mesh from an Ogre mesh.
	out_addedMaterials can be passed to obtain information about the used materials (for per triangle materials).
	@see PxShape::setMaterials
	*/
	bool cookPxTriangleMesh(Ogre::MeshPtr mesh, physx::PxOutputStream& outputStream, Params &params = Params(), AddedMaterials *out_addedMaterials = nullptr);
	void cookPxTriangleMeshToFile(Ogre::MeshPtr mesh, Ogre::String PxsOutputFile, Params &params = Params(), AddedMaterials *out_addedMaterials = nullptr);

	bool cookPxConvexMesh(Ogre::MeshPtr mesh, physx::PxOutputStream& outputStream, Params &params = Params());

	void cookPxCCDSkeleton(Ogre::MeshPtr mesh, physx::PxOutputStream& outputStream, Params &params = Params());

	/*
	createPxTriangleMesh
	Cooks an Px mesh from an ogre mesh and returns it, does not save to file.
	*/
	physx::PxTriangleMesh* createPxTriangleMesh(Ogre::MeshPtr mesh, Params &params = Params(), AddedMaterials *out_addedMaterials = nullptr);

	physx::PxConvexMesh* createPxConvexMesh(Ogre::MeshPtr mesh, Params &params = Params());

	//Singleton
	static Cooker& getSingleton();
};