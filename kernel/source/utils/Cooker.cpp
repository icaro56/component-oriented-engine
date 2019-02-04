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

#include "utils/Cooker.h"

#include "utils/Streams.h"
#include "engine/Engine.h"
#include "engine/PhysicsEngine.h"
#include "utils/KernelConverter.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
#include "PxMaterial.h"


Cooker::Cooker(void)
{
	mOgreResourceGroup = "General";
}

Cooker::~Cooker(void)
{
}

void Cooker::setOgreResourceGroup(Ogre::String group)
{
	mOgreResourceGroup = group;
}

bool Cooker::hasPxMesh(Ogre::String PxsFile)
{
	return Ogre::ResourceGroupManager::getSingleton().resourceExists(mOgreResourceGroup, PxsFile);
}

physx::PxTriangleMesh* Cooker::loadPxTriangleMeshFromFile(Ogre::String pxsFile)
{
	if (!hasPxMesh(pxsFile))
	{
		//throw exception
		return 0;
	}
	Ogre::DataStreamPtr ds = Ogre::ResourceGroupManager::getSingleton().openResource(pxsFile);
	
	return Physics->getPhysics()->createTriangleMesh(OgrePxInputStream(ds));
}

void Cooker::getMeshInfo(Ogre::MeshPtr mesh, Params &params, MeshInfo &outInfo)
{
	//First, we compute the total number of vertices and indices and init the buffers.
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	if (mesh->sharedVertexData) numVertices += mesh->sharedVertexData->vertexCount;
	Ogre::Mesh::SubMeshIterator subMeshIterator = mesh->getSubMeshIterator();
	bool indices32 = true;
	while (subMeshIterator.hasMoreElements())
	{
		Ogre::SubMesh *subMesh = subMeshIterator.getNext();
		if (subMesh->vertexData) numVertices += subMesh->vertexData->vertexCount;
		if (params.mAddBackfaces)
			numIndices += subMesh->indexData->indexCount*2;
		else
			numIndices += subMesh->indexData->indexCount;
				
		//We assume that every submesh uses the same index format
		indices32 = (subMesh->indexData->indexBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
	}

	outInfo.vertices.resize(numVertices);
	outInfo.indices.resize(numIndices);
	outInfo.materials.resize(numIndices / 3);

	unsigned int addedVertices = 0;
	unsigned int addedIndices = 0;
	unsigned int addedMaterialIndices = 0;

	/*
	Read shared vertices
	*/
	unsigned int shared_index_offset = 0;
	Ogre::VertexData *shared_vertex_data = mesh->sharedVertexData;
	if (shared_vertex_data)
	{
		const Ogre::VertexElement* posElem =
			shared_vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		Ogre::HardwareVertexBufferSharedPtr vbuf =
			shared_vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		shared_index_offset = shared_vertex_data->vertexCount;

		unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		float* pReal;
		for (size_t i = addedVertices; i < shared_vertex_data->vertexCount; i++)
		{
			posElem->baseVertexPointerToElement(pVertices, &pReal);
			Vector3 vec;
			vec.x = (*pReal++) * params.mScale.x;
			vec.y = (*pReal++) * params.mScale.y;
			vec.z = (*pReal++) * params.mScale.z;
			outInfo.vertices[i] = vec;
			pVertices += vbuf->getVertexSize();
		}
		vbuf->unlock();
		addedVertices += shared_vertex_data->vertexCount;

	}

	unsigned int index_offset = 0;

	/*
	Read submeshes
	*/
	subMeshIterator = mesh->getSubMeshIterator();
	while (subMeshIterator.hasMoreElements())
	{
		Ogre::SubMesh *subMesh = subMeshIterator.getNext();

		//Read vertex data
		Ogre::VertexData *vertex_data = subMesh->vertexData;
		if (vertex_data)
		{
			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* pVertices = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			float* pReal;
			for (size_t i = addedVertices; i < addedVertices+vertex_data->vertexCount; i++)
			{
				posElem->baseVertexPointerToElement(pVertices, &pReal);
				Vector3 vec;
				vec.x = (*pReal++) * params.mScale.x;
				vec.y = (*pReal++) * params.mScale.y;
				vec.z = (*pReal++) * params.mScale.z;
				outInfo.vertices[i] = vec;
				pVertices += vbuf->getVertexSize();
			}
			addedVertices += vertex_data->vertexCount;

			vbuf->unlock();
		}

		//Read index data
		Ogre::IndexData *index_data = subMesh->indexData;
		if (index_data)
		{
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

			physx::PxU32 *pIndices = 0;
			if (indices32)
			{
				pIndices = static_cast<physx::PxU32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			}
			else
			{
				physx::PxU16 *pShortIndices = static_cast<physx::PxU16*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				pIndices = new physx::PxU32[index_data->indexCount];
				for (size_t k = 0; k < index_data->indexCount; k++) pIndices[k] = static_cast<physx::PxU32>(pShortIndices[k]);
			}
			unsigned int bufferIndex = 0;
			if (params.mAddBackfaces)
			{
				size_t numTris = index_data->indexCount / 3;
				size_t i = addedIndices;
				for (unsigned int x = 0; x < numTris; x++)
				{
					if (subMesh->useSharedVertices)
					{
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i] = pIndices[bufferIndex];
						bufferIndex++;
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i+1] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i+1] = pIndices[bufferIndex];
						bufferIndex++;
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i+2] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i+2] = pIndices[bufferIndex];
						bufferIndex++;
					}
					else
					{
						outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
						bufferIndex++;
						outInfo.indices[i+1] = pIndices[bufferIndex] + index_offset;
						bufferIndex++;
						outInfo.indices[i+2] = pIndices[bufferIndex] + index_offset;
						bufferIndex++;
					}
					outInfo.indices[i+3] = outInfo.indices[i+2];
					outInfo.indices[i+4] = outInfo.indices[i+1];
					outInfo.indices[i+5] = outInfo.indices[i];
					i += 6;
				}
				addedIndices += index_data->indexCount*2;
			}
			else
			{
				for (size_t i = addedIndices; i < addedIndices+index_data->indexCount; i++)
				{
					if (subMesh->useSharedVertices)
					{
						if (pIndices[bufferIndex] > shared_index_offset) outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
						else outInfo.indices[i] = pIndices[bufferIndex];
					}
					else outInfo.indices[i] = pIndices[bufferIndex] + index_offset;
					bufferIndex++;
				}
				addedIndices += index_data->indexCount;
			}
			if (!indices32) delete[] pIndices;

			ibuf->unlock();

			//All triangles of a submesh have the same material
			unsigned int numTris = index_data->indexCount / 3;
			if (params.mAddBackfaces) numTris *= 2;
			for (size_t i = addedMaterialIndices; i < addedMaterialIndices+numTris; i++)
				outInfo.materials[i] = subMesh->getMaterialName();
			addedMaterialIndices += numTris;
		}

		if (vertex_data) index_offset += vertex_data->vertexCount;

	}
}

struct KERNEL_VS_EXPORT OctreeNode
{
	OctreeNode(){vPos.x=0.0f;vPos.y=0.0f;vPos.z=0.0f;
		aSubNodes[0]=0;aSubNodes[1]=0;aSubNodes[2]=0;aSubNodes[3]=0;aSubNodes[4]=0;aSubNodes[5]=0;aSubNodes[6]=0;aSubNodes[7]=0;}
	Vector3 vPos;
	OctreeNode* aSubNodes[8];
	std::list<int> liIndices;
};
	
struct KERNEL_VS_EXPORT STri
{
	STri(){i1=-1;i2=-1;i3=-1;}
	STri(int iIndex1, int iIndex2, int iIndex3, Ogre::String material, bool bSort=true)
	{
		if(!bSort)
		{
			i1=iIndex1;
			i2=iIndex2;
			i3=iIndex3;
			return;
		}
		//rotate indices
		if(iIndex2<iIndex1)
		{
			if(iIndex3<iIndex2)
			{//index 3 is the smallest
				i1=iIndex3;
				i2=iIndex1;
				i3=iIndex2;
			}
			else
			{
				i1=iIndex2;
				i2=iIndex3;
				i3=iIndex1;
			}
		}
		else
		{
			i1=iIndex1;
			i2=iIndex2;
			i3=iIndex3;
		}
		mat = material;
	}
	bool operator !=(STri& op){if(op.i1!=i1 || op.i2!=i2 || op.i3!=i3) return true; return false;}
	bool operator <(STri& op)
	{
		if(op.i1!=i1)
			return i1<op.i1;
		if(op.i2!=i2)
			return i2<op.i2;
		return i3<op.i3;
	}
	int i1,i2,i3;
	Ogre::String mat;
};

//returns current vertex count
int KERNEL_VS_EXPORT ExtractOctree(OctreeNode* pNode, int iVertexCount, int* aiIndexTable, Vector3* aNewVertices)
{
	for(std::list<int>::const_iterator it=pNode->liIndices.begin();
		it!=pNode->liIndices.end(); it++)
		aiIndexTable[*it]=iVertexCount;
	aNewVertices[iVertexCount++]=pNode->vPos;
	for(int iSubNode=0; iSubNode<8; iSubNode++)
		if(pNode->aSubNodes[iSubNode])
		{
			iVertexCount=ExtractOctree(pNode->aSubNodes[iSubNode], iVertexCount, aiIndexTable, aNewVertices);
			delete pNode->aSubNodes[iSubNode];
			pNode->aSubNodes[iSubNode] = nullptr;
		}
	return iVertexCount;
}


#define IS_IN_BOX(v1,v2,d) ((v1.x<=v2.x+d) && (v1.x>=v2.x-d) && (v1.y<=v2.y+d) && (v1.y>=v2.y-d) && (v1.z<=v2.z+d) && (v1.z>=v2.z-d))

#define EIGHTH_SPACE_INDEX(v1,v2) (((v1.x>v2.x)?4:0)+((v1.y>v2.y)?2:0)+((v1.z>v2.z)?1:0))

void Cooker::mergeVertices(MeshInfo &meshInfo, float fMergeDist)
{
	//const float fMergeDist=1e-3f;

	OctreeNode root;
	root.vPos=meshInfo.vertices[0];
	int iVertex=0;
	int numAdded = 0;
		
	for(;iVertex<(int)meshInfo.vertices.size(); iVertex++)
	{
		OctreeNode* pCurrNode=&root;
		while(true)
		{
			if(IS_IN_BOX(meshInfo.vertices[iVertex], pCurrNode->vPos, fMergeDist))
			{
				pCurrNode->liIndices.push_back(iVertex);
				break;
			}
			else
			{//vertex is not in merge distance to this node
				int iSubNode=EIGHTH_SPACE_INDEX(pCurrNode->vPos, meshInfo.vertices[iVertex]);
				if(pCurrNode->aSubNodes[iSubNode])
					//proceed deeper into the tree
					pCurrNode=pCurrNode->aSubNodes[iSubNode];
				else
				{//there is no branch so make one
					pCurrNode->aSubNodes[iSubNode]=new OctreeNode;
					pCurrNode=pCurrNode->aSubNodes[iSubNode];
					pCurrNode->vPos=meshInfo.vertices[iVertex];
					numAdded++;
				}
			}//pCurrNode is now one level lower in the tree
		}
	}
	int* aiIndexTable=new int[meshInfo.vertices.size()];		//maps old indices to new 		
	Vector3* aNewVertices=new Vector3[meshInfo.vertices.size()];
	//extract indextable and vertex list
	int nNewVertices=ExtractOctree(&root, 0, aiIndexTable, aNewVertices);
	for (unsigned int iIndex=0; iIndex< meshInfo.indices.size(); ++iIndex)
	{
		assert(meshInfo.indices[iIndex] < (int)meshInfo.indices.size());
		assert(meshInfo.indices[iIndex] >= 0);
		meshInfo.indices[iIndex] = aiIndexTable[meshInfo.indices[iIndex]];
	}
		
	meshInfo.vertices.resize(nNewVertices);
	for(iVertex=0; iVertex<nNewVertices; iVertex++)
		meshInfo.vertices[iVertex]=aNewVertices[iVertex];
		
	delete[] aiIndexTable;
	delete[] aNewVertices;
		
	//search for duplicated and degenerate tris
	std::vector<STri> vTris;
	vTris.resize(meshInfo.indices.size() / 3);
	int nTrisCopied=0;
	int iTri=0;
	for(; iTri<(int)meshInfo.indices.size() / 3; iTri++)
	{//check if this tri is degenerate
		int index1=meshInfo.indices[iTri*3+0],
			index2=meshInfo.indices[iTri*3+1],
			index3=meshInfo.indices[iTri*3+2];
		if(index1==index2 || index3==index2 || index1==index3)
			//degenerate tri: two or more vertices are the same
			continue;
		vTris[nTrisCopied++]=STri(index1,index2,index3, meshInfo.materials[iTri]);
	}
	vTris.resize(nTrisCopied);
	std::sort(vTris.begin(), vTris.end());//sort tris to find duplicates easily
	nTrisCopied=0;
	STri lastTri;
	for(iTri=0; iTri<(int)vTris.size(); iTri++)
	{
		if(lastTri!=vTris[iTri])
		{
			meshInfo.indices[nTrisCopied*3+0]=vTris[iTri].i1;
			meshInfo.indices[nTrisCopied*3+1]=vTris[iTri].i2;
			meshInfo.indices[nTrisCopied*3+2]=vTris[iTri].i3;
			meshInfo.materials[nTrisCopied]=vTris[iTri].mat;
			lastTri=vTris[iTri];
			nTrisCopied++;
		}
	}
	meshInfo.materials.resize(nTrisCopied);
	meshInfo.indices.resize(nTrisCopied*3);
}

//function to generate ccd mesh
void Cooker::insetMesh(MeshInfo &meshInfo, float fAmount)
{
	//STri* tris= new STri[meshInfo.numTriangles];
	std::vector<STri> vTris;
	vTris.resize(meshInfo.indices.size()/3);//create rotated tris
	int iTri=0;
	for(; iTri<(int)meshInfo.indices.size()/3; iTri++)
	{
		vTris[iTri*3]=STri(meshInfo.indices[iTri*3], meshInfo.indices[iTri*3+1], meshInfo.indices[iTri*3+2], 0, false);//no mat indices this time!
		vTris[iTri*3+1]=STri(meshInfo.indices[iTri*3+2], meshInfo.indices[iTri*3], meshInfo.indices[iTri*3+1], 0, false);
		vTris[iTri*3+2]=STri(meshInfo.indices[iTri*3+1], meshInfo.indices[iTri*3+2], meshInfo.indices[iTri*3], 0, false);
	}
	std::sort(vTris.begin(), vTris.end());
	int iLastIndex=-1;
	int nVertices = 0;
	Vector3 vAccNomals;
	Vector3 vPos;
	std::vector<Vector3> vNewVertices;
	vNewVertices.resize(meshInfo.vertices.size());
	for(iTri=0; iTri<(int)vTris.size(); iTri++)
	{
		if(vTris[iTri].i1!=iLastIndex)
		{
			if(iLastIndex!=-1)
			{
				vAccNomals.normalise();
				vNewVertices[iLastIndex]=meshInfo.vertices[iLastIndex]-fAmount*vAccNomals;
			}
			nVertices=0;
			vAccNomals=Vector3(0,0,0);
			iLastIndex=vTris[iTri].i1;
			vPos=meshInfo.vertices[iLastIndex];
		}
		Vector3 v=(meshInfo.vertices[vTris[iTri].i2]-vPos).crossProduct(meshInfo.vertices[vTris[iTri].i3]-vPos);
		v.normalise();
		vAccNomals+=v;
		nVertices++;
	}
	vAccNomals.normalise();
	vNewVertices[iLastIndex]=meshInfo.vertices[iLastIndex]+fAmount*vAccNomals;
	for(unsigned int iVertex=0; iVertex<meshInfo.vertices.size(); iVertex++)
		meshInfo.vertices[iVertex]=vNewVertices[iVertex];
}

bool Cooker::cookPxTriangleMesh(Ogre::MeshPtr mesh, physx::PxOutputStream& outputStream, Params &params, AddedMaterials *out_addedMaterials)
{
	MeshInfo meshInfo;
	getMeshInfo(mesh, params, meshInfo);
	mergeVertices(meshInfo);

	physx::PxTriangleMeshDesc desc;
	desc.setToDefault();

	desc.points.count = meshInfo.vertices.size();
	desc.points.stride = 12;
	float *fVertices = new float[meshInfo.vertices.size()*3];
	for (unsigned int i = 0; i < meshInfo.vertices.size(); ++i)
	{
		fVertices[i*3] = meshInfo.vertices[i].x;
		fVertices[i*3+1] = meshInfo.vertices[i].y;
		fVertices[i*3+2] = meshInfo.vertices[i].z;
	}
	desc.points.data = fVertices;
		
	desc.triangles.count = meshInfo.indices.size() / 3;
	desc.triangles.stride = 12;
	physx::PxU32 *iIndices = new physx::PxU32[meshInfo.indices.size()];
	for (unsigned int i = 0; i < meshInfo.indices.size(); ++i)
		iIndices[i] = meshInfo.indices[i];
	desc.triangles.data = iIndices;

	std::unordered_map<physx::PxMaterial*, physx::PxMaterialTableIndex> materialIndicesMap;
	std::vector<physx::PxMaterial*> orderedMaterials;
	std::pair<physx::PxMaterial*, physx::PxMaterialTableIndex> pair;
	pair = std::make_pair(Physics->getDefaultMaterial(), 0);
	materialIndicesMap.insert(pair);	//add default material at index 0
	orderedMaterials.push_back(Physics->getDefaultMaterial());

	physx::PxMaterialTableIndex *materialIndices = nullptr;
	if (!params.mMaterialBindings.empty())
	{
		desc.materialIndices.stride = sizeof(physx::PxMaterialTableIndex);
		materialIndices = new physx::PxMaterialTableIndex[meshInfo.indices.size() / 3];
		for (unsigned int i = 0; i < meshInfo.indices.size() / 3; ++i)
		{
			auto mat = params.mMaterialBindings.find(meshInfo.materials[i]);
			if (mat == params.mMaterialBindings.end())
			{
				materialIndices[i] = 0;		//default material
			}
			else
			{
				auto matIndex = materialIndicesMap.find(mat->second);
				physx::PxMaterialTableIndex index;
				if (matIndex == materialIndicesMap.end())
				{	//create material index if not existing
					index = materialIndicesMap.size();

					std::pair<physx::PxMaterial*, physx::PxMaterialTableIndex> par;
					par = std::make_pair(mat->second, index);
					materialIndicesMap.insert(par);
					orderedMaterials.push_back(mat->second);
				}
				else index = matIndex->second;
				materialIndices[i] = index;
			}
		}	
	}
	desc.materialIndices.data = materialIndices;

	if (out_addedMaterials)
	{
		out_addedMaterials->materialCount = materialIndicesMap.size();
		out_addedMaterials->materials = new physx::PxMaterial*[out_addedMaterials->materialCount];
		for (unsigned int i = 0; i < orderedMaterials.size(); ++i)
			out_addedMaterials->materials[i] = orderedMaterials[i];
	}

	bool result = Physics->getCooking()->cookTriangleMesh(desc, outputStream);

	delete[] fVertices;
	delete[] iIndices;
	if (materialIndices) delete[] materialIndices;

	return result;
}

bool Cooker::cookPxConvexMesh(Ogre::MeshPtr mesh, physx::PxOutputStream& outputStream, Params &params)
{
	MeshInfo meshInfo;
	getMeshInfo(mesh, params, meshInfo);
	mergeVertices(meshInfo, 0.1f);

	physx::PxConvexMeshDesc desc;
	desc.points.count = meshInfo.vertices.size();
	desc.points.stride = 12;
	desc.flags |= physx::PxConvexFlag::eCOMPUTE_CONVEX;

	float *fVertices = new float[meshInfo.vertices.size()*3];
	for (unsigned int i = 0; i < meshInfo.vertices.size(); ++i)
	{
		fVertices[i*3] = meshInfo.vertices[i].x;
		fVertices[i*3+1] = meshInfo.vertices[i].y;
		fVertices[i*3+2] = meshInfo.vertices[i].z;
	}
	desc.points.data = fVertices;
		
	desc.triangles.count = meshInfo.indices.size() / 3;
	desc.triangles.stride = 12;
	int *iIndices = new int[meshInfo.indices.size()];
	for (unsigned int i = 0; i < meshInfo.indices.size(); ++i)
		iIndices[i] = meshInfo.indices[i];
	desc.triangles.data = iIndices;

	bool result = Physics->getCooking()->cookConvexMesh(desc, outputStream);

	delete[] fVertices;
	delete[] iIndices;

	return result;
}

void Cooker::cookPxTriangleMeshToFile(Ogre::MeshPtr mesh, Ogre::String pxsOutputFile, Params &params, AddedMaterials *out_addedMaterials)
{
	std::fstream f;
	f.open(pxsOutputFile.c_str());
	cookPxTriangleMesh(mesh, OgrePxOutputStream(Ogre::DataStreamPtr(&Ogre::FileStreamDataStream(&f, false))), params);
	f.close();
}

physx::PxTriangleMesh* Cooker::createPxTriangleMesh(Ogre::MeshPtr mesh, Params &params, AddedMaterials *out_addedMaterial)	
{
	physx::PxDefaultMemoryOutputStream writeBuffer;
	
	if (cookPxTriangleMesh(mesh, writeBuffer, params, out_addedMaterial))
	{
		physx::PxDefaultMemoryInputData inputBuffer(writeBuffer.getData(), writeBuffer.getSize());

		return Physics->getPhysics()->createTriangleMesh(inputBuffer);
	}

	return NULL;
}

physx::PxConvexMesh* Cooker::createPxConvexMesh(Ogre::MeshPtr mesh, Params &params)	
{
	//MemoryOutputStream writeBuffer;
	physx::PxDefaultMemoryOutputStream writeBuffer;

	if (cookPxConvexMesh(mesh, writeBuffer, params))
	{
		physx::PxDefaultMemoryInputData inputBuffer(writeBuffer.getData(), writeBuffer.getSize());

		return Physics->getPhysics()->createConvexMesh(inputBuffer);
	}

	return NULL;

	
}

Cooker& Cooker::getSingleton()
{
	static Cooker instance;
	return instance;
}