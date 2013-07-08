/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "dStdAfxNewton.h"
#include "dNewton.h"
#include "dNewtonMesh.h"
#include "dNewtonCollision.h"


dNewtonMesh::dNewtonMesh(dNewton* const world)
	:m_mesh (NewtonMeshCreate (world->GetNewton()))
{
}

dNewtonMesh::dNewtonMesh(const dNewtonMesh& clone)
	:m_mesh (NewtonMeshCreateFromMesh (clone.m_mesh))
{
}

dNewtonMesh::dNewtonMesh(const dNewtonCollision& collision)
	:m_mesh (NewtonMeshCreateFromCollision(collision.GetShape()))
{
}

dNewtonMesh::dNewtonMesh(dNewton* const world, int pointCount, const dFloat* const vertexCloud, int strideInBytes, dFloat tolerance)
	:m_mesh (NewtonMeshCreateConvexHull (world->GetNewton(), pointCount, vertexCloud, strideInBytes, tolerance))
{
}

dNewtonMesh::~dNewtonMesh()
{
	NewtonMeshDestroy (m_mesh);
}



void dNewtonMesh::CreateVoronoiConvexDecomposition (const dNewtonMesh& contexMesh)
{
	NewtonMeshDestroy (m_mesh);
	dAssert (0);
//	m_mesh = NewtonMeshCreateVoronoiConvexDecomposition (const NewtonWorld* const newtonWorld, int pointCount, const dFloat* const vertexCloud, int strideInBytes, int materialID, const dFloat* const textureMatrix);
}

void dNewtonMesh::CreateApproximateConvexDecomposition (const dNewtonMesh& mesh, dFloat maxConcavity, dFloat backFaceDistanceFactor, int maxCount, int maxVertexPerHull)
{
	NewtonMeshDestroy (m_mesh);
	m_mesh = NewtonMeshApproximateConvexDecomposition (mesh.m_mesh, maxConcavity, backFaceDistanceFactor, maxCount, maxVertexPerHull, NULL);
}



void dNewtonMesh::Polygonize ()
{
	NewtonMeshPolygonize (m_mesh);
}

void dNewtonMesh::Triangulate ()
{
	NewtonMeshTriangulate (m_mesh);
}

int dNewtonMesh::GetPointCount() const
{
	return NewtonMeshGetPointCount (m_mesh);
}

void dNewtonMesh::GetVectexStreams(dPoint* const posit, dPoint* const normal, dUV* const uv0, dUV* const uv1) const
{
	NewtonMeshGetVertexStreams (m_mesh, sizeof (dPoint), &posit[0].m_x, sizeof (dPoint), &normal[0].m_x, sizeof (dUV), &uv0[0].m_u, sizeof (dUV), &uv1[0].m_u);
}


int dNewtonMesh::GetTotalIndexCount() const
{
	return NewtonMeshGetTotalIndexCount (m_mesh); 
}

int dNewtonMesh::GetTotalFaceCount() const
{
	return NewtonMeshGetTotalFaceCount (m_mesh); 
}

void* dNewtonMesh::BeginMaterialHandle () const
{
	return NewtonMeshBeginHandle (m_mesh); 
}

void dNewtonMesh::EndMaterialHandle (void* const materialHandle) const
{
	NewtonMeshEndHandle (m_mesh, materialHandle); 
}

int dNewtonMesh::GetMaterialIndex (void* const materialHandle) const
{
	return NewtonMeshFirstMaterial (m_mesh, materialHandle);
}

int dNewtonMesh::GetNextMaterialIndex (void* const materialHandle, int materialIndex) const
{
	return NewtonMeshNextMaterial (m_mesh, materialHandle, materialIndex);
}

int dNewtonMesh::MaterialGetMaterial (void* const materialHandle, int materialIndex) const
{
	return NewtonMeshMaterialGetMaterial (m_mesh, materialHandle, materialIndex); 
}

int dNewtonMesh::MaterialGetIndexCount (void* const materialHandle, int materialIndex) const
{
	return NewtonMeshMaterialGetIndexCount (m_mesh, materialHandle, materialIndex); 
}

void dNewtonMesh::MaterialGetIndexStream (void* const materialHandle, int materialIndex, int* const indexes) const
{
	NewtonMeshMaterialGetIndexStream (m_mesh, materialHandle, materialIndex, indexes); 
}

/*
AllocVertexData(NewtonMeshGetPointCount (mesh));
NewtonMeshGetVertexStreams (mesh, 3 * sizeof (dFloat), (dFloat*) m_vertex, 3 * sizeof (dFloat), (dFloat*) m_normal,	2 * sizeof (dFloat), (dFloat*) m_uv, 2 * sizeof (dFloat), (dFloat*) m_uv);

//	dTree<dScene::dTreeNode*, int> materialMap;
//	for (void* ptr = scene->GetFirstChild(meshNode); ptr; ptr = scene->GetNextChild (meshNode, ptr)) {
//		dScene::dTreeNode* node = scene->GetNodeFromLink(ptr);
//		dNodeInfo* info = scene->GetInfoFromNode(node);
//		if (info->GetTypeId() == dMaterialNodeInfo::GetRttiType()) {
//			dMaterialNodeInfo* const material = (dMaterialNodeInfo*)info;
//			materialMap.Insert(node, material->GetId());
//		}
//	}

// extract the materials index array for mesh
void* const meshCookie = NewtonMeshBeginHandle (mesh); 
for (int handle = NewtonMeshFirstMaterial (mesh, meshCookie); handle != -1; handle = NewtonMeshNextMaterial (mesh, meshCookie, handle)) {
	int textureId = NewtonMeshMaterialGetMaterial (mesh, meshCookie, handle); 
	int indexCount = NewtonMeshMaterialGetIndexCount (mesh, meshCookie, handle); 
	DemoSubMesh* const segment = AddSubMesh();

	//		dTree<dScene::dTreeNode*, int>::dTreeNode* matNodeCache = materialMap.Find(materialIndex);
	//		if (matNodeCache) {
	//			dScene::dTreeNode* const matNode = matNodeCache->GetInfo();
	//			dMaterialNodeInfo* const material = (dMaterialNodeInfo*) scene->GetInfoFromNode(matNode);
	//			if (material->GetDiffuseTextId() != -1) {
	//				dScene::dTreeNode* const node = scene->FindTextureByTextId(matNode, material->GetDiffuseTextId());
	//				dAssert (node);
	//				dTextureNodeInfo* const texture = (dTextureNodeInfo*)scene->GetInfoFromNode(node);
	//
	//				segment->m_textureHandle = LoadTexture(texture->GetPathName());
	//				strcpy (segment->m_textureName, texture->GetPathName());
	//			}


	segment->m_shiness = 1.0f;
	segment->m_ambient = dVector (0.8f, 0.8f, 0.8f, 1.0f);
	segment->m_diffuse = dVector (0.8f, 0.8f, 0.8f, 1.0f);
	segment->m_specular = dVector (0.0f, 0.0f, 0.0f, 1.0f);
	//		}

	segment->m_textureHandle = textureId;

	segment->AllocIndexData (indexCount);
	// for 16 bit indices meshes
	//NewtonMeshMaterialGetIndexStreamShort (mesh, meshCookie, handle, (short int*)segment->m_indexes); 

	// for 32 bit indices mesh
	NewtonMeshMaterialGetIndexStream (mesh, meshCookie, handle, (int*)segment->m_indexes); 
}
NewtonMeshEndHandle (mesh, meshCookie); 
*/