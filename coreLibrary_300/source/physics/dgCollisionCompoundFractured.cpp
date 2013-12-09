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

#include "dgPhysicsStdafx.h"
#include "dgWorld.h"
#include "dgMeshEffect.h"
#include "dgCollisionInstance.h"
#include "dgCollisionConvexHull.h"
#include "dgCollisionCompoundFractured.h"


#define DG_FRACTURE_AABB_GUARD_DISTANCE 1.0f


dgCollisionCompoundFractured::dgConectivityGraph::dgConectivityGraph (dgMemoryAllocator* const allocator)
	:dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>(allocator)
{
}


dgCollisionCompoundFractured::dgConectivityGraph::dgConectivityGraph (dgMemoryAllocator* const allocator, dgDeserialize callback, void* const userData)
	:dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>(allocator)
{
	dgAssert (0);
/*
	dgInt32 count;
	dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>::dgListNode* node;

	callback (userData, &count, dgInt32 (sizeof (dgInt32)));
	dgStack<dgListNode*> nodesMap(count);

	node = dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>::AddNode();
	dgDebriNodeInfo& data = GetFirst()->GetInfo().m_nodeData;
	callback (userData, &data.m_commonData, sizeof (data.m_commonData));

	nodesMap[0] = node;
	for (dgInt32 i = 1; i < count; i ++) {
		dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>::dgListNode* node;

		node = dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>::AddNode();
		dgDebriNodeInfo& data = node->GetInfo().m_nodeData;
		callback (userData, &data.m_commonData, sizeof (data.m_commonData));
		data.m_mesh = new (GetAllocator()) dgMesh (GetAllocator(), callback, userData);
		nodesMap[i] = node;
	}

	for (dgInt32 i = 0; i < count - 1; i ++) {
		dgInt32 edges;
		callback (userData, &edges, dgInt32 (sizeof (dgInt32)));
		dgStack<dgInt32> pool(edges);
		callback (userData, &pool[0], size_t (edges * dgInt32 (sizeof (dgInt32))));
		for (dgInt32 j = 0; j < edges; j ++) {
			nodesMap[i]->GetInfo().AddEdge (nodesMap[pool[j]]);
		}
	}
*/
}



dgCollisionCompoundFractured::dgConectivityGraph::dgConectivityGraph (const dgConectivityGraph& source)
	:dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>(source.GetAllocator())
{
	dgTree<dgListNode*, dgListNode*> filter(GetAllocator());   

	for (dgConectivityGraph::dgListNode* node = source.GetFirst(); node; node = node->GetNext() ) {
		dgListNode* const newNode = dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>::AddNode();

		dgDebriNodeInfo& srcData = node->GetInfo().m_nodeData;
		dgDebriNodeInfo& data = newNode->GetInfo().m_nodeData;
		//data.m_commonData = srcData.m_commonData;
		data.m_mesh = srcData.m_mesh;
		data.m_mesh->AddRef();

		filter.Insert(newNode, node);
	}

	for (dgListNode* node = source.GetFirst(); node != source.GetLast(); node = node->GetNext()) {
		dgListNode* const myNode = filter.Find(node)->GetInfo();
		for (dgGraphNode<dgDebriNodeInfo, dgSharedNodeMesh>::dgListNode* edgeNode = node->GetInfo().GetFirst(); edgeNode; edgeNode = edgeNode->GetNext()) {
			dgListNode* const otherNode = filter.Find(edgeNode->GetInfo().m_node)->GetInfo();
			myNode->GetInfo().AddEdge (otherNode);
		}
	}
}


dgCollisionCompoundFractured::dgConectivityGraph::~dgConectivityGraph ()
{
}

void dgCollisionCompoundFractured::dgConectivityGraph::Serialize(dgSerialize callback, void* const userData) const
{
	dgAssert (0);
/*
	dgInt32 lru;
	dgInt32 index;
	dgInt32 count;
	dgTree<dgInt32,dgListNode*> enumerator(GetAllocator());   

	lru = 0;
	index = 1;
	count = GetCount();
	callback (userData, &count, dgInt32 (sizeof (dgInt32)));
	dgDebriNodeInfo& data = GetFirst()->GetInfo().m_nodeData;

	dgDebriNodeInfo::PackedSaveData packedData (data.m_commonData);
	packedData.m_lru = 0;
	callback (userData, &packedData, sizeof (packedData));
	enumerator.Insert(0, GetFirst());

	for (dgListNode* node = GetFirst()->GetNext(); node; node = node->GetNext()) {
		dgDebriNodeInfo& data = node->GetInfo().m_nodeData;
		dgDebriNodeInfo::PackedSaveData packedData (data.m_commonData);
		packedData.m_lru = 0;
		callback (userData, &packedData, sizeof (packedData));

		data.m_mesh->Serialize(callback, userData);
		enumerator.Insert(index, node);
		index ++;
	}
	
	for (dgListNode* node = GetFirst(); node != GetLast(); node = node->GetNext()) {
		dgInt32 count;

		index = 0;
		count = node->GetInfo().GetCount();
		callback (userData, &count, dgInt32 (sizeof (dgInt32)));
		dgStack<dgInt32> buffer(count);
		dgInt32* const pool = &buffer[0];
		for (dgGraphNode<dgDebriNodeInfo, dgSharedNodeMesh>::dgListNode* edgeNode = node->GetInfo().GetFirst(); edgeNode; edgeNode = edgeNode->GetNext()) {
			pool[index] = enumerator.Find(edgeNode->GetInfo().m_node)->GetInfo();
			index ++;
		}
		callback (userData, &pool[0], size_t (count * sizeof (dgInt32)));
	}
*/
}




dgCollisionCompoundFractured::dgCollisionCompoundFractured (dgWorld* const world, dgDeserialize deserialization, void* const userData)
	:dgCollisionCompound (world, deserialization, userData)
	,m_conectivity (world->GetAllocator(), deserialization, userData)
	,m_conectivityMap (world->GetAllocator())
//	,m_detachedIslands(world->GetAllocator())
{
	dgAssert (0);
/*
	dgInt32 stack;
	m_collisionId = m_compoundBreakable;
	m_rtti |= dgCollisionCompoundBreakable_RTTI;

	deserialization (userData, &m_lru, dgInt32 (sizeof (dgInt32)));
	deserialization (userData, &m_lastIslandColor, dgInt32 (sizeof (dgInt32)));
	deserialization (userData, &m_visibilityMapIndexCount, dgInt32 (sizeof (dgInt32)));

	m_visibilityMap = (dgInt8 *) m_allocator->Malloc (dgInt32 (m_visibilityMapIndexCount * sizeof (dgInt8)));
	deserialization (userData, m_visibilityMap, m_visibilityMapIndexCount * sizeof (dgInt8));

	m_visibilityInderectMap = (dgInt32 *) m_allocator->Malloc (m_visibilityMapIndexCount * dgInt32 (sizeof (dgInt32)));
	deserialization (userData, m_visibilityInderectMap, size_t (m_visibilityMapIndexCount * dgInt32 (sizeof (dgInt32))));

	m_vertexBuffer = new (m_allocator) dgVertexBuffer (m_allocator, deserialization, userData);

	stack = 0;
	for (dgDebriGraph::dgListNode* node = m_conectivity.GetFirst()->GetNext(); node != m_conectivity.GetLast(); node = node->GetNext() ) {
		dgCollisionConvexIntance* instance;
		instance = (dgCollisionConvexIntance*) m_array[stack];
		node->GetInfo().m_nodeData.m_shape = instance;
		instance->AddRef();
		instance->m_graphNode = node;
		stack ++;
	}
*/
}


class dgCollisionCompoundFractured::dgFractureBuilder: public dgTree<dgMeshEffect*, dgInt32>
{
	public:
    class dgPerimenterEdge
    {
        public:
        dgPerimenterEdge* m_next;
        dgPerimenterEdge* m_prev;
        const dgBigVector* m_vertex;
    };

	class dgFractureConectivity: public dgGraph<int, int>
	{
		public:
		dgFractureConectivity(dgMemoryAllocator* const allocator)
			:dgGraph<int, int> (allocator)
		{
		}
	};

	class dgConvexSolidVertices: public dgList<dgInt32>  
	{
		public:
		dgConvexSolidVertices(dgMemoryAllocator* const allocator)
			:dgList<dgInt32> (allocator)
		{
		}
	};

	class dgConvexSolidArray: public dgTree<dgConvexSolidVertices, dgInt32>  
	{
		public:
		dgConvexSolidArray(dgMemoryAllocator* const allocator)
			:dgTree<dgConvexSolidVertices, dgInt32> (allocator) 
		{
		}
	};


	dgFractureBuilder (dgMemoryAllocator* const allocator, dgMeshEffect* const solidMesh, dgInt32 pointcloudCount, const dgFloat32* const vertexCloud, dgInt32 strideInBytes, int materialId, const dgMatrix& textureProjectionMatrix)
		:dgTree<dgMeshEffect*, dgInt32>(allocator)
		,m_conectivity(allocator)
	{
		dgStack<dgBigVector> buffer(pointcloudCount + 16);
		dgBigVector* const pool = &buffer[0];
		dgFloat64 quantizeFactor = dgFloat64 (16.0f);
		dgFloat64 invQuantizeFactor = dgFloat64 (1.0f) / quantizeFactor;
		dgInt32 stride = strideInBytes / sizeof (dgFloat32); 

		dgBigVector minAABB;
		dgBigVector maxAABB;
		solidMesh->CalculateAABB (minAABB, maxAABB);
		for (dgInt32 i = 0; i < pointcloudCount; i ++) {
			dgFloat64 x = vertexCloud[i * stride + 0];
			dgFloat64 y	= vertexCloud[i * stride + 1];
			dgFloat64 z	= vertexCloud[i * stride + 2];
			x = floor (x * quantizeFactor) * invQuantizeFactor;
			y = floor (y * quantizeFactor) * invQuantizeFactor;
			z = floor (z * quantizeFactor) * invQuantizeFactor;
			pool[i] = dgBigVector (x, y, z, dgFloat64 (0.0f));
		}

		// add the bbox as a barrier
		int count = pointcloudCount;
		pool[count + 0] = dgBigVector ( minAABB.m_x, minAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 1] = dgBigVector ( maxAABB.m_x, minAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 2] = dgBigVector ( minAABB.m_x, maxAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 3] = dgBigVector ( maxAABB.m_x, maxAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 4] = dgBigVector ( minAABB.m_x, minAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		pool[count + 5] = dgBigVector ( maxAABB.m_x, minAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		pool[count + 6] = dgBigVector ( minAABB.m_x, maxAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		pool[count + 7] = dgBigVector ( maxAABB.m_x, maxAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		count += 8;

		dgStack<dgInt32> indexList(count);
		count = dgVertexListToIndexList(&pool[0].m_x, sizeof (dgBigVector), 3, count, &indexList[0], dgFloat64 (5.0e-2f));	
		dgAssert (count >= 8);

		dgFloat64 maxSize = dgMax(maxAABB.m_x - minAABB.m_x, maxAABB.m_y - minAABB.m_y, maxAABB.m_z - minAABB.m_z);
		minAABB -= dgBigVector (maxSize, maxSize, maxSize, dgFloat64 (0.0f));
		maxAABB += dgBigVector (maxSize, maxSize, maxSize, dgFloat64 (0.0f));

		// add the a guard zone, so that we do not have to clip
		dgInt32 guadVertexKey = count;
		pool[count + 0] = dgBigVector ( minAABB.m_x, minAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 1] = dgBigVector ( maxAABB.m_x, minAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 2] = dgBigVector ( minAABB.m_x, maxAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 3] = dgBigVector ( maxAABB.m_x, maxAABB.m_y, minAABB.m_z, dgFloat64 (0.0f));
		pool[count + 4] = dgBigVector ( minAABB.m_x, minAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		pool[count + 5] = dgBigVector ( maxAABB.m_x, minAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		pool[count + 6] = dgBigVector ( minAABB.m_x, maxAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		pool[count + 7] = dgBigVector ( maxAABB.m_x, maxAABB.m_y, maxAABB.m_z, dgFloat64 (0.0f));
		count += 8; 

		dgDelaunayTetrahedralization delaunayTetrahedras (allocator, &pool[0].m_x, count, sizeof (dgBigVector), dgFloat32 (0.0f));
		delaunayTetrahedras.RemoveUpperHull ();

		dgInt32 tetraCount = delaunayTetrahedras.GetCount();
		dgStack<dgBigVector> voronoiPoints(tetraCount + 32);
		dgStack<dgDelaunayTetrahedralization::dgListNode*> tetradrumNode(tetraCount);
		dgConvexSolidArray delanayNodes (allocator);	

		dgInt32 index = 0;
		const dgHullVector* const delanayPoints = delaunayTetrahedras.GetHullVertexArray();
		for (dgDelaunayTetrahedralization::dgListNode* node = delaunayTetrahedras.GetFirst(); node; node = node->GetNext()) {
			dgConvexHull4dTetraherum& tetra = node->GetInfo();
			voronoiPoints[index] = tetra.CircumSphereCenter (delanayPoints);
			tetradrumNode[index] = node;

			for (dgInt32 i = 0; i < 4; i ++) {
				dgConvexSolidArray::dgTreeNode* header = delanayNodes.Find(tetra.m_faces[0].m_index[i]);
				if (!header) {
					dgConvexSolidVertices list (allocator);
					header = delanayNodes.Insert(list, tetra.m_faces[0].m_index[i]);
				}
				header->GetInfo().Append (index);
			}
			index ++;
		}

		dgConvexSolidArray::Iterator iter (delanayNodes);
		dgFloat32 normalAngleInRadians = 30.0f * 3.1416f / 180.0f;

		dgTree<dgFractureConectivity::dgListNode*, int> graphMap(allocator);
		for (iter.Begin(); iter; iter ++) {

			dgConvexSolidArray::dgTreeNode* const nodeNode = iter.GetNode();
			const dgList<dgInt32>& list = nodeNode->GetInfo();
			dgInt32 key = nodeNode->GetKey();
			if (key < guadVertexKey) {
				dgBigVector pointArray[512];
				dgInt32 indexArray[512];

				dgInt32 count = 0;
				for (dgList<dgInt32>::dgListNode* ptr = list.GetFirst(); ptr; ptr = ptr->GetNext()) {
					dgInt32 i = ptr->GetInfo();
					pointArray[count] = voronoiPoints[i];
					count ++;
					dgAssert (count < dgInt32 (sizeof (pointArray) / sizeof (pointArray[0])));
				}

				count = dgVertexListToIndexList(&pointArray[0].m_x, sizeof (dgBigVector), 3, count, &indexArray[0], dgFloat64 (1.0e-3f));	
				if (count >= 4) {
					dgMeshEffect* const convexMesh = new (allocator) dgMeshEffect (allocator, &pointArray[0].m_x, count, sizeof (dgBigVector), dgFloat64 (0.0f));
					if (convexMesh->GetCount()) {
						convexMesh->AddRef();
						Insert (convexMesh, key);

						convexMesh->CalculateNormals(normalAngleInRadians);
						convexMesh->UniformBoxMapping (materialId, textureProjectionMatrix);

						dgInt32 vCount = convexMesh->GetVertexCount();
						for (dgInt32 i = 0; i < vCount; i ++) {
							dgBigVector& point = convexMesh->GetVertex (i);
							point.m_w = key;
						}
						dgInt32 aCount = convexMesh->GetPropertiesCount();
						for (dgInt32 i = 0; i < aCount; i ++) {
							dgMeshEffect::dgVertexAtribute& attib = convexMesh->GetAttribute (i);
							attib.m_vertex.m_w = key;
						}

						dgFractureConectivity::dgListNode* const node = m_conectivity.AddNode ();
						node->GetInfo().m_nodeData = key;
						graphMap.Insert(node, key);
					}
					convexMesh->Release();
				}
			}
		}
		delanayNodes.RemoveAll();

		for (dgDelaunayTetrahedralization::dgListNode* node = delaunayTetrahedras.GetFirst(); node; node = node->GetNext()) {
			dgConvexHull4dTetraherum& tetra = node->GetInfo();
			for (dgInt32 i = 0; i < 3; i ++) {
				dgInt32 nodeindex0 = tetra.m_faces[0].m_index[i];
				if (nodeindex0 < guadVertexKey) {
					for (dgInt32 j = i + 1; j < 4; j ++) {
						dgInt32 nodeindex1 = tetra.m_faces[0].m_index[j];
						if (nodeindex1 < guadVertexKey) {
							dgAssert (graphMap.Find(nodeindex0));
							dgAssert (graphMap.Find(nodeindex1));
							dgFractureConectivity::dgListNode* const node0 = graphMap.Find(nodeindex0)->GetInfo();
							dgFractureConectivity::dgListNode* const node1 = graphMap.Find(nodeindex1)->GetInfo();
							if (!IsPairConnected (node0, node1)) {
								if (AreSolidNeigborg (nodeindex0, nodeindex1)) {
									node0->GetInfo().AddEdge(node1);
									node1->GetInfo().AddEdge(node0);
								}
							}
						}
					}
				}
			}
		}

		dgAssert (SanityCheck());
        ClipFractureParts (solidMesh);

#if 0
for (dgFractureConectivity::dgListNode* node = m_conectivity.GetFirst(); node; node = node->GetNext()) {
	dgInt32 index = node->GetInfo().m_nodeData;
	dgTrace (("node %d: ", index));
	for (dgGraphNode<int, int>::dgListNode* edge = node->GetInfo().GetFirst(); edge; edge = edge->GetNext()) {
		dgFractureConectivity::dgListNode* const otherNode = edge->GetInfo().m_node;
		dgInt32 index1 = otherNode->GetInfo().m_nodeData;
		dgTrace (("%d ", index1));
	}
	dgTrace (("\n"));
}
#endif

		dgAssert (SanityCheck());
	}

    ~dgFractureBuilder()
    {
        Iterator iter (*this);
        for (iter.Begin(); iter; iter ++) {
            dgMeshEffect* const mesh = iter.GetNode()->GetInfo();
            mesh->Release();
        }
    }


	bool IsPairConnected (dgFractureConectivity::dgListNode* const nodeA, dgFractureConectivity::dgListNode* const nodeB) const
	{
		for (dgGraphNode<int, int>::dgListNode* edgeNodeAB = nodeA->GetInfo().GetFirst(); edgeNodeAB; edgeNodeAB = edgeNodeAB->GetNext()) {
			dgFractureConectivity::dgListNode* const otherNode = edgeNodeAB->GetInfo().m_node;
			if (otherNode == nodeB)
				return true;
		}
		return false;
	}

	bool ArePlaneCoplanar (dgMeshEffect* const meshA, void* faceA, const dgBigVector& planeA, dgMeshEffect* const meshB, void* faceB, const dgBigVector& planeB) const
	{
		if (((planeA % planeB) < dgFloat64 (-1.0 + 1.0e-6f)) && ((fabs(planeA.m_w + planeB.m_w) < dgFloat64(1.0e-6f)))) {
			const dgBigVector* const pointsA = (dgBigVector*) meshA->GetVertexPool();
			const dgBigVector* const pointsB = (dgBigVector*) meshB->GetVertexPool();

			dgInt32 indexA[128];
			dgInt32 indexB[128];

			dgInt32 indexCountA = meshA->GetFaceIndexCount(faceA);
			dgInt32 indexCountB = meshB->GetFaceIndexCount(faceB);
			dgAssert (indexCountA < sizeof (indexA)/ sizeof(indexA[0]));
			dgAssert (indexCountB < sizeof (indexB)/ sizeof(indexB[0]));

			meshA->GetFaceIndex(faceA, indexA);
			meshA->GetFaceIndex(faceB, indexB);

//dgTrace (("faceA:\n"));
			dgPerimenterEdge subdivision[256];
			dgAssert ((2 * (indexCountA + indexCountB)) < dgInt32 (sizeof (subdivision) / sizeof (subdivision[0])));
			for (dgInt32 i = 0; i < indexCountB; i ++) {
				subdivision[i].m_vertex = &pointsB[indexB[i]];
				subdivision[i].m_prev = &subdivision[i - 1];
				subdivision[i].m_next = &subdivision[i + 1];

//dgTrace (("%f %f %f\n", pointsB[indexB[i]].m_x, pointsB[indexB[i]].m_y, pointsB[indexB[i]].m_z));
			}
			subdivision[0].m_prev = &subdivision[indexCountB - 1];
			subdivision[indexCountB - 1].m_next = &subdivision[0];

            dgInt32 edgeIndex = indexCountB;

			dgBigVector outputPool[128];
			dgPerimenterEdge* edgeClipped[2];
			dgBigVector* output = &outputPool[0];
			edgeClipped[0] = NULL;
			edgeClipped[1] = NULL;

//dgTrace (("faceB:\n"));
			dgPerimenterEdge* poly = &subdivision[0];
			dgInt32 i0 = indexCountA - 1;
			for (dgInt32 i1 = 0; i1 < indexCountA; i1 ++) {
                const dgBigVector& q0 = pointsA[indexA[i0]];
                const dgBigVector& q1 = pointsA[indexA[i1]];
				dgBigVector n (planeA * (q1 - q0));
				dgBigPlane plane (n, - (n % q0));
				i0 = i1;
//dgTrace (("%f %f %f\n", q0.m_x, q0.m_y, q0.m_z));

				dgInt32 count = 0;
				dgPerimenterEdge* tmp = poly;
				dgInt32 isInside = 0;
				dgFloat64 test0 = plane.Evalue (*tmp->m_vertex);
				do {
					dgFloat64 test1 = plane.Evalue (*tmp->m_next->m_vertex);
					if (test0 >= dgFloat32 (0.0f)) {
						isInside |= 1;
						if (test1 < dgFloat32 (0.0f)) {
							const dgBigVector& p0 = *tmp->m_vertex;
							const dgBigVector& p1 = *tmp->m_next->m_vertex;

							dgBigVector dp (p1 - p0); 
							dgFloat64 den = plane % dp;
							if (fabs(den) < dgFloat32 (1.0e-24f)) {
								den = (den >= dgFloat32 (0.0f)) ?  dgFloat32 (1.0e-24f) :  dgFloat32 (-1.0e-24f);
							}

							den = test0 / den;
							if (den >= dgFloat32 (0.0f)) {
								den = dgFloat32 (0.0f);
							} else if (den <= -1.0f) {
								den = dgFloat32 (-1.0f);
							}
							output[0] = p0 - dp.Scale3 (den);
							edgeClipped[0] = tmp;
							count ++;
						}
					} else if (test1 >= dgFloat32 (0.0f)) {
						const dgBigVector& p0 = *tmp->m_vertex;
						const dgBigVector& p1 = *tmp->m_next->m_vertex;
						isInside |= 1;
						dgBigVector dp (p1 - p0); 
						dgFloat64 den = plane % dp;
						if (fabs(den) < dgFloat32 (1.0e-24f)) {
							den = (den >= dgFloat32 (0.0f)) ?  dgFloat32 (1.0e-24f) :  dgFloat32 (-1.0e-24f);
						}
						den = test0 / den;
						if (den >= dgFloat32 (0.0f)) {
							den = dgFloat32 (0.0f);
						} else if (den <= -1.0f) {
							den = dgFloat32 (-1.0f);
						}
						output[1] = p0 - dp.Scale3 (den);
						edgeClipped[1] = tmp;
						count ++;
					}
					test0 = test1;
					tmp = tmp->m_next;
				} while ((tmp != poly) && count < 2);

				if (!isInside) {
					return false;
				}

				if (count == 2) {
					dgPerimenterEdge* const newEdge = &subdivision[edgeIndex];
					newEdge->m_next = edgeClipped[1];
					newEdge->m_prev = edgeClipped[0];
					edgeClipped[0]->m_next = newEdge;
					edgeClipped[1]->m_prev = newEdge;

					newEdge->m_vertex = &output[0];
					edgeClipped[1]->m_vertex = &output[1];
					poly = newEdge;

					output += 2;
					edgeIndex ++;
					dgAssert (edgeIndex < dgInt32 (sizeof (subdivision) / sizeof (subdivision[0])));
				}
			}
//dgTrace (("\n"));
			dgAssert (poly);
            dgBigVector area(dgFloat32 (0.0f));
            dgBigVector r0 (*poly->m_vertex);
            dgBigVector r1 (*poly->m_next->m_vertex);
            dgBigVector r1r0 (r1 - r0);
            dgPerimenterEdge* polyPtr = poly->m_next->m_next;
            do {
                dgBigVector r2 (*polyPtr->m_vertex);
                dgBigVector r2r0 (r2 - r0);
                area += r2r0 * r1r0;
                r1r0 = r2r0;
                polyPtr = polyPtr->m_next;
            } while (polyPtr != poly);
            return fabs (area % planeA) > dgFloat32 (1.0e-5f);
		}

		return false;
	}


    bool AreSolidNeigborg (int indexA, int indexB) const
    {
        dgMeshEffect* const meshA = Find(indexA)->GetInfo();
        dgMeshEffect* const meshB = Find(indexB)->GetInfo();

        const dgBigVector* const pointsA = (dgBigVector*) meshA->GetVertexPool();
        const dgBigVector* const pointsB = (dgBigVector*) meshB->GetVertexPool();

        dgBigVector planeB_array[512];

        dgInt32 planeB_Count = 0;
        for (void* faceB = meshB->GetFirstFace(); faceB; faceB = meshB->GetNextFace(faceB)) {
            if (!meshB->IsFaceOpen (faceB)) {
				dgInt32 vertexIndexB = meshB->GetVertexIndex (faceB);
				dgBigVector planeB (meshB->CalculateFaceNormal (faceB));
				planeB.m_w = -(planeB % pointsB[vertexIndexB]);
				planeB_array[planeB_Count] = planeB;
				planeB_Count ++;
				dgAssert (planeB_Count < sizeof (planeB_array) / sizeof (planeB_array[0]));
			}
        }

        for (void* faceA = meshA->GetFirstFace(); faceA; faceA = meshA->GetNextFace(faceA)) {
            if (!meshA->IsFaceOpen (faceA)) {
				dgInt32 vertexIndexA = meshA->GetVertexIndex (faceA);
				dgBigVector planeA (meshA->CalculateFaceNormal (faceA));
				planeA.m_w = -(planeA % pointsA[vertexIndexA]);

				dgInt32 indexB = 0;
				for (void* faceB = meshB->GetFirstFace(); faceB; faceB = meshB->GetNextFace(faceB)) {
					if (!meshB->IsFaceOpen (faceB)) {
						if (ArePlaneCoplanar (meshA, faceA, planeA, meshB, faceB, planeB_array[indexB])) {
							return true;
						}
						indexB ++;
					}
				}
			}
        }
        return false;
    }

    void ClipFractureParts (dgMeshEffect* const solidMesh)
    {
		dgFractureBuilder::dgFractureConectivity::dgListNode* nextNode;
        for (dgFractureConectivity::dgListNode* node = m_conectivity.GetFirst(); node; node = nextNode) {
			nextNode = node->GetNext();
            dgInt32 index = node->GetInfo().m_nodeData;
            dgTreeNode* const fractureNode = Find(index);
            dgAssert (fractureNode);
            dgMeshEffect* const voronoiConvex = fractureNode->GetInfo();
            dgMeshEffect* const fracturePiece = solidMesh->ConvexMeshIntersection (voronoiConvex);
            if (fracturePiece) {
                voronoiConvex->Release();    
                fractureNode->GetInfo() = fracturePiece;
            } else {
				m_conectivity.DeleteNode(node);
			}
        }
    }

	bool SanityCheck() const
	{
		for (dgFractureConectivity::dgListNode* rootNode = m_conectivity.GetFirst(); rootNode; rootNode = rootNode->GetNext() ) {
			dgTree<dgFractureConectivity::dgListNode*, dgFractureConectivity::dgListNode*> filter(GetAllocator());
			for (dgGraphNode<int, int>::dgListNode* edgeNode = rootNode->GetInfo().GetFirst(); edgeNode; edgeNode = edgeNode->GetNext()) {
				dgFractureConectivity::dgListNode* node = edgeNode->GetInfo().m_node;
				dgAssert (!filter.Find(node));
				filter.Insert(node, node);
			}
		}
		return true;
	}

	dgFractureConectivity m_conectivity;
};



dgCollisionCompoundFractured::dgDebriNodeInfo::dgDebriNodeInfo ()
	:m_mesh(NULL)
	,m_shapeNode(NULL)
	,m_lru(0)
{
//	memset (&m_commonData, 0, sizeof (m_commonData));
}

dgCollisionCompoundFractured::dgDebriNodeInfo::~dgDebriNodeInfo ()
{

	if (m_mesh) {
		m_mesh->Release();
	}
}


dgCollisionCompoundFractured::dgVertexBuffer::dgVertexBuffer(dgInt32 vertsCount, dgMemoryAllocator* allocator)
	:m_allocator(allocator)
	,m_vertexCount(vertsCount)
{
	m_uv = (dgFloat32 *) m_allocator->Malloc (2 * vertsCount * dgInt32 (sizeof (dgFloat32))); 
	m_vertex = (dgFloat32 *) m_allocator->Malloc (3 * vertsCount * dgInt32 (sizeof (dgFloat32))); 
	m_normal = (dgFloat32 *) m_allocator->Malloc (3 * vertsCount * dgInt32 (sizeof (dgFloat32))); 
}

dgCollisionCompoundFractured::dgVertexBuffer::~dgVertexBuffer ()
{
	m_allocator->Free (m_normal);
	m_allocator->Free (m_vertex);
	m_allocator->Free (m_uv);
}

dgCollisionCompoundFractured::dgVertexBuffer::dgVertexBuffer (dgMemoryAllocator* const allocator, dgDeserialize callback, void* const userData)
{
	dgAssert(0);
	m_allocator = allocator;
	callback (userData, &m_vertexCount, dgInt32 (sizeof (dgInt32)));

	m_uv = (dgFloat32 *) m_allocator->Malloc (2 * m_vertexCount * dgInt32 (sizeof (dgFloat32))); 
	m_vertex = (dgFloat32 *) m_allocator->Malloc (3 * m_vertexCount * dgInt32 (sizeof (dgFloat32))); 
	m_normal = (dgFloat32 *) m_allocator->Malloc (3 * m_vertexCount * dgInt32 (sizeof (dgFloat32))); 

	callback (userData, m_vertex, size_t (3 * m_vertexCount * dgInt32 (sizeof (dgFloat32))));
	callback (userData, m_normal, size_t (3 * m_vertexCount * dgInt32 (sizeof (dgFloat32))));
	callback (userData, m_uv, size_t (2 * m_vertexCount * dgInt32 (sizeof (dgFloat32))));
}

void dgCollisionCompoundFractured::dgVertexBuffer::Serialize(dgSerialize callback, void* const userData) const
{
	callback (userData, &m_vertexCount, dgInt32 (sizeof (dgInt32)));
	callback (userData, m_vertex, size_t (3 * m_vertexCount * dgInt32 (sizeof (dgFloat32))));
	callback (userData, m_normal, size_t (3 * m_vertexCount * dgInt32 (sizeof (dgFloat32))));
	callback (userData, m_uv, size_t (2 * m_vertexCount * dgInt32 (sizeof (dgFloat32))));
}

/*
void dgCollisionCompoundBreakable::dgVertexBuffer::GetVertexStreams (dgInt32 vertexStrideInByte, dgFloat32* const vertex, dgInt32 normalStrideInByte, dgFloat32* const normal, dgInt32 uvStrideInByte, dgFloat32* const uv) const
{
	uvStrideInByte /= dgInt32 (sizeof (dgFloat32));
	vertexStrideInByte /= dgInt32 (sizeof (dgFloat32));
	normalStrideInByte /= dgInt32 (sizeof (dgFloat32));
	for (dgInt32 i = 0; i < m_vertexCount; i ++)	{
		dgInt32 j = i * vertexStrideInByte;
		vertex[j + 0] = m_vertex[i * 3 + 0];
		vertex[j + 1] = m_vertex[i * 3 + 1];
		vertex[j + 2] = m_vertex[i * 3 + 2];

		j = i * normalStrideInByte;
		normal[j + 0] = m_normal[i * 3 + 0];
		normal[j + 1] = m_normal[i * 3 + 1];
		normal[j + 2] = m_normal[i * 3 + 2];

		j = i * uvStrideInByte;
		uv[j + 0] = m_uv[i * 2 + 0];
		uv[j + 1] = m_uv[i * 2 + 1];
	}
}
*/

dgCollisionCompoundFractured::dgSubMesh::dgSubMesh (dgMemoryAllocator* const allocator)
	:m_indexes(NULL)
	,m_allocator(allocator)
	,m_faceOffset(0)
	,m_material(0)
	,m_faceCount(0)
	,m_visibleFaces(true)
{
}

dgCollisionCompoundFractured::dgSubMesh::~dgSubMesh ()
{
	if (m_indexes) {
		m_allocator->Free (m_indexes);
	}
}

void dgCollisionCompoundFractured::dgSubMesh::Serialize(dgSerialize callback, void* const userData) const
{
	dgAssert(0);
/*
	callback (userData, &m_material, dgInt32 (sizeof (dgInt32)));
	callback (userData, &m_faceCount, dgInt32 (sizeof (dgInt32)));
	callback (userData, &m_faceOffset, dgInt32 (sizeof (dgInt32)));
	callback (userData, &m_visibleFaces, dgInt32 (sizeof (dgInt32)));
	callback (userData, m_indexes, size_t (3 * m_faceCount * dgInt32 (sizeof (dgInt32))));
*/
}

dgCollisionCompoundFractured::dgSharedNodeMesh::dgSharedNodeMesh ()
{
}

dgCollisionCompoundFractured::dgSharedNodeMesh::~dgSharedNodeMesh ()
{
}


dgCollisionCompoundFractured::dgMesh::dgMesh(dgMemoryAllocator* const allocator)
	:dgList<dgSubMesh>(allocator)
	,m_IsVisible(false)
{
}

dgCollisionCompoundFractured::dgMesh::~dgMesh()
{
}

dgCollisionCompoundFractured::dgMesh::dgMesh (dgMemoryAllocator* const allocator, dgDeserialize callback, void* const userData)
	:dgList<dgSubMesh>(allocator), dgRefCounter ()
{
	dgAssert(0);
	/*

	dgInt32 count;

	callback (userData, &m_IsVisible, dgInt32 (sizeof (dgInt32)));
	callback (userData, &count, dgInt32 (sizeof (dgInt32)));
	for (dgInt32 i = 0; i < count; i ++) {
		dgInt32 material;
		dgInt32 faceCount;
		dgInt32 faceOffset;
		dgInt32 exteriorFaces;

		callback (userData, &material, dgInt32 (sizeof (dgInt32)));
		callback (userData, &faceCount, dgInt32 (sizeof (dgInt32)));
		callback (userData, &faceOffset, dgInt32 (sizeof (dgInt32)));
		callback (userData, &exteriorFaces, dgInt32 (sizeof (dgInt32)));
		dgSubMesh* const subMesh = AddgSubMesh (faceCount * 3, material);

		subMesh->m_faceOffset = faceOffset;
		subMesh->m_visibleFaces = exteriorFaces;

		//		callback (userData, subMesh->m_visibilityMap, faceCount * dgInt32 (sizeof (dgInt32)));
		callback (userData, subMesh->m_indexes, size_t (3 * faceCount * sizeof (dgInt32)));
	}
*/
}

void dgCollisionCompoundFractured::dgMesh::Serialize(dgSerialize callback, void* const userData) const
{
	dgAssert(0);
/*
	dgInt32 count;
	count = GetCount();

	callback (userData, &m_IsVisible, dgInt32 (sizeof (dgInt32)));
	callback (userData, &count, dgInt32 (sizeof (dgInt32)));
	for (dgListNode* node = GetFirst(); node; node = node->GetNext()) {
		dgSubMesh& subMesh = node->GetInfo();
		subMesh.Serialize(callback, userData);
	}
*/
}

dgCollisionCompoundFractured::dgSubMesh* dgCollisionCompoundFractured::dgMesh::AddgSubMesh(dgInt32 indexCount, dgInt32 material)
{
	dgSubMesh tmp (GetAllocator());
	dgSubMesh& subMesh = Append(tmp)->GetInfo();

	subMesh.m_faceOffset = 0;
	subMesh.m_visibleFaces = true;

	subMesh.m_material = material;
	subMesh.m_faceCount = indexCount / 3;

	subMesh.m_indexes = (dgInt32 *) subMesh.m_allocator->Malloc (indexCount * dgInt32 (sizeof (dgInt32))); 
	return &subMesh;
}


 dgCollisionCompoundFractured::dgConectivityGraph::dgListNode* dgCollisionCompoundFractured::dgConectivityGraph::AddNode (dgFlatVertexArray& vertexArray, dgMeshEffect* const factureVisualMesh, dgTreeArray::dgTreeNode* const collisionNode, dgInt32 interiorMaterialBase)
{
	dgListNode* const node = dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>::AddNode ();
	dgDebriNodeInfo& data = node->GetInfo().m_nodeData;

	data.m_mesh = new (GetAllocator()) dgMesh(GetAllocator());
	data.m_shapeNode = collisionNode;

	dgInt32 vertexCount = factureVisualMesh->GetPropertiesCount();

	dgStack<dgVector>vertex (vertexCount);
	dgStack<dgVector>normal (vertexCount);
	dgStack<dgVector>uv0 (vertexCount);
	dgStack<dgVector>uv1 (vertexCount);
	factureVisualMesh->GetVertexStreams (sizeof (dgVector), &vertex[0].m_x, sizeof (dgVector), &normal[0].m_x, sizeof (dgVector), &uv0[0].m_x, sizeof (dgVector), &uv1[0].m_x);
		
	// extract the materials index array for mesh
	dgMeshEffect::dgIndexArray* const geometryHandle = factureVisualMesh->MaterialGeometryBegin();

	vertexCount = vertexArray.m_count;
	dgInt32 baseVertexCount = vertexArray.m_count;
	for (dgInt32 handle = factureVisualMesh->GetFirstMaterial (geometryHandle); handle != -1; handle = factureVisualMesh->GetNextMaterial (geometryHandle, handle)) {
		dgInt32 material = factureVisualMesh->GetMaterialID (geometryHandle, handle);

		bool isVisible = (material < interiorMaterialBase) ? true : false;
		data.m_mesh->m_IsVisible |= isVisible;

		dgInt32 indexCount = factureVisualMesh->GetMaterialIndexCount (geometryHandle, handle);
		dgSubMesh* const segment = data.m_mesh->AddgSubMesh(indexCount, isVisible ? material : material - interiorMaterialBase);
		segment->m_visibleFaces = isVisible;

		factureVisualMesh->GetMaterialGetIndexStream (geometryHandle, handle, segment->m_indexes);
		for (dgInt32 i = 0; i < indexCount; i ++) {
			dgInt32 j = segment->m_indexes[i];
			vertexArray[vertexCount].m_point[0] = vertex[j].m_x;
			vertexArray[vertexCount].m_point[1] = vertex[j].m_y;
			vertexArray[vertexCount].m_point[2] = vertex[j].m_z;
			vertexArray[vertexCount].m_point[3] = normal[j].m_x;
			vertexArray[vertexCount].m_point[4] = normal[j].m_y;
			vertexArray[vertexCount].m_point[5] = normal[j].m_z;
			vertexArray[vertexCount].m_point[6] = uv0[j].m_x;
			vertexArray[vertexCount].m_point[7] = uv0[j].m_y;
			vertexArray[vertexCount].m_point[8] = uv1[j].m_x;
			vertexArray[vertexCount].m_point[9] = uv1[j].m_y;
			segment->m_indexes[i] = vertexCount - baseVertexCount; 
			vertexCount ++;
		}
	}
	factureVisualMesh->MaterialGeomteryEnd(geometryHandle);

	dgStack<dgInt32> indexBuffer (vertexCount - baseVertexCount);
	vertexArray.m_count += dgVertexListToIndexList (&vertexArray[baseVertexCount].m_point[0], sizeof (dgFlatVertex), sizeof (dgFlatVertex), 0, vertexCount - baseVertexCount, &indexBuffer[0], dgFloat32 (1.0e-6f));

	for (dgMesh::dgListNode* meshSgement = data.m_mesh->GetFirst(); meshSgement; meshSgement = meshSgement->GetNext()) {
		dgSubMesh* const subMesh = &meshSgement->GetInfo();
		for (dgInt32 i = 0; i < subMesh->m_faceCount * 3; i ++) {
			dgInt32 j = subMesh->m_indexes[i];  
			subMesh->m_indexes[i] = baseVertexCount + indexBuffer[j];
		}
	}

	return node;
}
 

dgCollisionCompoundFractured::dgCollisionCompoundFractured (const dgCollisionCompoundFractured& source)
    :dgCollisionCompound(source)
	,m_conectivity(source.m_conectivity)
	,m_conectivityMap (source.m_conectivityMap)
	,m_vertexBuffer(source.m_vertexBuffer)
	,m_lru(0)
	,m_visibilityMapIndexCount(source.m_visibilityMapIndexCount)
	,m_impulseStrengthPerUnitMass(source.m_impulseStrengthPerUnitMass)
	,m_impulseAbsortionFactor(source.m_impulseAbsortionFactor)
{
	m_rtti |= dgCollisionCompoundBreakable_RTTI;

	m_vertexBuffer->AddRef();


	m_visibilityMapIndexCount = source.m_visibilityMapIndexCount;
	m_visibilityMap = (dgInt8 *) m_allocator->Malloc (dgInt32 (source.m_visibilityMapIndexCount * sizeof (dgInt8)));
	memcpy (m_visibilityMap, source.m_visibilityMap, size_t (source.m_visibilityMapIndexCount * sizeof (dgInt8)));

	m_visibilityIndirectMap = (dgInt32 *) m_allocator->Malloc (source.m_visibilityMapIndexCount * dgInt32 (sizeof (dgInt32)));
	memcpy (m_visibilityIndirectMap, source.m_visibilityIndirectMap, size_t (source.m_visibilityMapIndexCount * dgInt32 (sizeof (dgInt32))));

	dgTree<dgInt32, dgTreeArray::dgTreeNode*> nodeMap(m_allocator);
	dgTreeArray::Iterator iter (source.m_array);
	for (iter.Begin(); iter; iter ++) {
		nodeMap.Insert(iter.GetKey(), iter.GetNode());
	}

	dgConectivityGraph::dgListNode* myNode = m_conectivity.GetFirst();
	for (dgConectivityGraph::dgListNode* node = source.m_conectivity.GetFirst(); node != source.m_conectivity.GetLast(); myNode = myNode->GetNext(), node = node->GetNext() ) {
		dgDebriNodeInfo& nodeInfo = node->GetInfo().m_nodeData;
		dgTreeArray::dgTreeNode* const nodeBase = nodeInfo.m_shapeNode;

		dgAssert (nodeMap.Find(nodeBase));
		dgInt32 index = nodeMap.Find(nodeBase)->GetInfo();

		dgAssert (m_array.Find(index));
		dgDebriNodeInfo& myNodeInfo = myNode->GetInfo().m_nodeData;
		myNodeInfo.m_shapeNode = m_array.Find(index);
	}
	m_conectivityMap.Pupolate(m_conectivity);
	dgAssert (SanityCheck());
}


dgCollisionCompoundFractured::dgCollisionCompoundFractured (dgWorld* const world, dgMeshEffect* const solidMesh, int fracturePhysicsMaterialID, int pointcloudCount, const dgFloat32* const vertexCloud, int strideInBytes, int materialID, const dgMatrix& offsetMatrix)
	:dgCollisionCompound (world)
	,m_conectivity(world->GetAllocator())
	,m_conectivityMap (world->GetAllocator())
	,m_vertexBuffer(NULL)
	,m_lru(0)
	,m_impulseStrengthPerUnitMass(10.0f)
	,m_impulseAbsortionFactor(0.5f)
{
//pointcloudCount = 0;

	m_collisionId = m_compoundBreakable;
	m_rtti |= dgCollisionCompoundBreakable_RTTI;

	dgInt32 interiorMaterialBase = 1000;
	dgFractureBuilder fractureBuilder (GetAllocator(), solidMesh, pointcloudCount, vertexCloud, strideInBytes, materialID + interiorMaterialBase, offsetMatrix);

	dgFlatVertexArray vertexArray(m_world->GetAllocator());
	dgTree<dgConectivityGraph::dgListNode*, dgInt32> conectinyMap(GetAllocator());
	
    BeginAddRemove ();
    for (dgFractureBuilder::dgFractureConectivity::dgListNode* node = fractureBuilder.m_conectivity.GetFirst(); node; node = node->GetNext()) {
        dgInt32 index = node->GetInfo().m_nodeData;
        dgFractureBuilder::dgTreeNode* const fractureNode = fractureBuilder.Find(index);
        dgAssert (fractureNode);
        dgMeshEffect* const fractureSubMesh = fractureNode->GetInfo();
        dgCollisionInstance* const collisionInstance = fractureSubMesh->CreateConvexCollision(world, dgFloat32 (1.0e-5f), fracturePhysicsMaterialID);
		dgTreeArray::dgTreeNode* const shapeNode = AddCollision (collisionInstance);
		dgConectivityGraph::dgListNode* const conectivityNode = m_conectivity.AddNode(vertexArray, fractureSubMesh, shapeNode, interiorMaterialBase);
		conectinyMap.Insert(conectivityNode, index);
		collisionInstance->Release();
	}
    EndAddRemove ();

	for (dgFractureBuilder::dgFractureConectivity::dgListNode* node = fractureBuilder.m_conectivity.GetFirst(); node; node = node->GetNext()) {
		dgInt32 index0 = node->GetInfo().m_nodeData;
		dgConectivityGraph::dgListNode* const conectivityNode0 = conectinyMap.Find(index0)->GetInfo();
		for (dgGraphNode<int, int>::dgListNode* edge = node->GetInfo().GetFirst(); edge; edge = edge->GetNext()) {
			dgFractureBuilder::dgFractureConectivity::dgListNode* const otherNode = edge->GetInfo().m_node;
			dgInt32 index1 = otherNode->GetInfo().m_nodeData;
			dgConectivityGraph::dgListNode* const conectivityNode1 = conectinyMap.Find(index1)->GetInfo();
			conectivityNode0->GetInfo().AddEdge(conectivityNode1);
		}
	}

	dgStack<dgInt32> indexBuffer (vertexArray.m_count);
	dgInt32 vertsCount = dgVertexListToIndexList (&vertexArray[0].m_point[0], sizeof (dgFlatVertex), sizeof (dgFlatVertex), 0, vertexArray.m_count, &indexBuffer[0], dgFloat32 (1.0e-4f));

	m_vertexBuffer = new (m_world->GetAllocator()) dgVertexBuffer(vertsCount, m_world->GetAllocator());
	for (dgInt32 i = 0; i < vertsCount; i ++) {
		m_vertexBuffer->m_vertex[i * 3 + 0] = vertexArray[i].m_point[0];
		m_vertexBuffer->m_vertex[i * 3 + 1] = vertexArray[i].m_point[1];
		m_vertexBuffer->m_vertex[i * 3 + 2] = vertexArray[i].m_point[2];
		m_vertexBuffer->m_normal[i * 3 + 0] = vertexArray[i].m_point[3];
		m_vertexBuffer->m_normal[i * 3 + 1] = vertexArray[i].m_point[4];
		m_vertexBuffer->m_normal[i * 3 + 2] = vertexArray[i].m_point[5];
		m_vertexBuffer->m_uv[i * 2 + 0] = vertexArray[i].m_point[6];
		m_vertexBuffer->m_uv[i * 2 + 1] = vertexArray[i].m_point[7];
	}

	dgSubMesh* mainSegmenst[256];
	dgInt32 materialHitogram[256];
	dgInt32 faceOffsetHitogram[256];

	memset (materialHitogram, 0, sizeof (materialHitogram));
	memset (faceOffsetHitogram, 0, sizeof (faceOffsetHitogram));
	memset (mainSegmenst, 0, sizeof (mainSegmenst));

	for (dgConectivityGraph::dgListNode* node = m_conectivity.GetFirst(); node; node = node->GetNext()) {
		dgDebriNodeInfo& data = node->GetInfo().m_nodeData;
		for (dgMesh::dgListNode* meshSgement = data.m_mesh->GetFirst(); meshSgement; meshSgement = meshSgement->GetNext()) {
			dgSubMesh* const subMesh = &meshSgement->GetInfo();
			dgInt32 material = subMesh->m_material;
			materialHitogram[material] += subMesh->m_faceCount;
		}
	}

	dgInt32 acc = 0;
	dgMesh* const mainMesh = new (m_world->GetAllocator()) dgMesh(m_world->GetAllocator());

	for (dgInt32 i = 0; i < 256; i ++) {
		if (materialHitogram[i]) {
			dgSubMesh* const segment = mainMesh->AddgSubMesh(materialHitogram[i] * 3, i);
			segment->m_faceOffset = acc;
			segment->m_faceCount = 0;
			mainSegmenst[i] = segment;
		}
		faceOffsetHitogram[i] = acc;
		acc += materialHitogram[i];
	}

	m_visibilityMapIndexCount = acc;

	m_visibilityMap = (dgInt8*) m_allocator->Malloc (dgInt32 (acc * sizeof (dgInt8)));
	m_visibilityIndirectMap = (dgInt32*) m_allocator->Malloc (acc * dgInt32 (sizeof (dgInt32)));
	acc = 0;
	for (dgConectivityGraph::dgListNode* node = m_conectivity.GetFirst(); node; node = node->GetNext() ) {
		dgDebriNodeInfo& data = node->GetInfo().m_nodeData;
		for (dgMesh::dgListNode* node = data.m_mesh->GetFirst(); node; node = node->GetNext()) {
			dgSubMesh& segment = node->GetInfo();
			dgSubMesh& rootSegment = *mainSegmenst[segment.m_material];
			bool visbility = segment.m_visibleFaces;

			memset (&m_visibilityMap[acc], visbility, size_t (segment.m_faceCount));
			for (dgInt32 i = 0; i < segment.m_faceCount; i ++) {
				m_visibilityIndirectMap[faceOffsetHitogram[segment.m_material] + i] = acc + i;
			}
			faceOffsetHitogram[segment.m_material] += segment.m_faceCount;

			dgInt32 rootIndexCount = rootSegment.m_faceCount * 3;
			for (dgInt32 i = 0; i < segment.m_faceCount * 3; i ++) {
				dgInt32 j = segment.m_indexes[i];
				segment.m_indexes[i] = indexBuffer[j];
				rootSegment.m_indexes[rootIndexCount] = indexBuffer[j];
				rootIndexCount ++;
			}
			rootSegment.m_faceCount = rootIndexCount / 3;

			segment.m_faceOffset = acc;
//			dgAssert (acc == segment.m_faceOffset);
			acc += segment.m_faceCount;
		}
//break;
	}

	dgConectivityGraph::dgListNode* const mainNode = m_conectivity.dgGraph<dgDebriNodeInfo, dgSharedNodeMesh>::AddNode ();
	dgDebriNodeInfo& mainNodeData = mainNode->GetInfo().m_nodeData;
	mainNodeData.m_mesh = mainMesh;

	m_conectivityMap.Pupolate(m_conectivity);

	dgAssert (SanityCheck());
//	LinkNodes ();	
//	ResetAnchor ();
//  m_conectivity.Trace();
}

dgCollisionCompoundFractured::~dgCollisionCompoundFractured(void)
{
    if (m_visibilityMap) {
        m_allocator->Free (m_visibilityMap);
        m_allocator->Free (m_visibilityIndirectMap);
    }

    if (m_vertexBuffer) {
        m_vertexBuffer->Release();
    }
}


dgInt32 dgCollisionCompoundFractured::GetSegmentIndexStreamShort (dgConectivityGraph::dgListNode* const node, dgMesh::dgListNode* subMeshNode, dgInt16* const index) const
{
	dgSubMesh* const subMesh = &subMeshNode->GetInfo();
	const dgInt32* const indexes = subMesh->m_indexes;	

	dgInt32 currentIndex = 0;
	if (node == m_conectivity.GetLast()) {
		dgInt32 faceCount;

		const dgInt8* const visbilityMap = m_visibilityMap;
		const dgInt32* const visibilityInderectMap = &m_visibilityIndirectMap[subMesh->m_faceOffset];

		faceCount = subMesh->m_faceCount;
		for (dgInt32 i = 0; i < faceCount; i ++) {
			if (visbilityMap[visibilityInderectMap[i]]) {
				index[currentIndex + 0] = dgInt16 (indexes[i * 3 + 0]);
				index[currentIndex + 1] = dgInt16 (indexes[i * 3 + 1]);
				index[currentIndex + 2] = dgInt16 (indexes[i * 3 + 2]);
				currentIndex += 3;
			}
		}

	} else {
		dgInt32 indexCount = subMesh->m_faceCount * 3;
		for (dgInt32 i = 0; i < indexCount; i ++) {
			index[i] = dgInt16 (indexes[i]);
		}
		currentIndex = indexCount;
	}

	return currentIndex;
}


dgInt32 dgCollisionCompoundFractured::GetSegmentIndexStream (dgConectivityGraph::dgListNode* const node, dgMesh::dgListNode* const subMeshNode, dgInt32* const index) const
{
	dgSubMesh* const subMesh = &subMeshNode->GetInfo();
	const dgInt32* const indexes = subMesh->m_indexes;

	dgInt32 currentIndex = 0;
	if (node == m_conectivity.GetLast()) {
		dgInt32 faceCount;

		const dgInt8* const visbilityMap = m_visibilityMap;
		const dgInt32* const visibilityInderectMap = &m_visibilityIndirectMap[subMesh->m_faceOffset];

		faceCount = subMesh->m_faceCount;
		for (dgInt32 i = 0; i < faceCount; i ++) {
			if (visbilityMap[visibilityInderectMap[i]]) {
				index[currentIndex + 0] = indexes[i * 3 + 0];
				index[currentIndex + 1] = indexes[i * 3 + 1];
				index[currentIndex + 2] = indexes[i * 3 + 2];
				currentIndex += 3;
			}
		}

	} else {
		dgInt32 indexCount = subMesh->m_faceCount * 3;
		for (dgInt32 i = 0; i < indexCount; i ++) {
			index[i] = indexes[i];
		}
		currentIndex = indexCount;
	}
	return currentIndex;
}


void dgCollisionCompoundFractured::SetImpulseStrength(dgFloat32 impulseStrength)
{
	m_impulseStrengthPerUnitMass = dgMax (impulseStrength, dgFloat32 (0.0f), impulseStrength) ;
}

dgFloat32 dgCollisionCompoundFractured::GetImpulseStrength() const
{
	return m_impulseStrengthPerUnitMass;
}

void dgCollisionCompoundFractured::SetImpulsePropgationFactor(dgFloat32 factor)
{
	m_impulseAbsortionFactor = dgClamp(factor, dgFloat32 (0.0f), dgFloat32 (1.0f));
}

dgFloat32 dgCollisionCompoundFractured::GetSetImpulsePropgationFactor() const
{
	return m_impulseAbsortionFactor;
}



dgVector dgCollisionCompoundFractured::GetObbSize() const
{
	return dgCollisionCompound::GetObbSize() + dgVector (DG_FRACTURE_AABB_GUARD_DISTANCE) & dgVector::m_triplexMask;;
}


void dgCollisionCompoundFractured::CalcAABB (const dgMatrix& matrix, dgVector& p0, dgVector& p1) const
{
	dgCollisionCompound::CalcAABB (matrix, p0, p1);
	p0 -= dgVector (DG_FRACTURE_AABB_GUARD_DISTANCE) & dgVector::m_triplexMask;
	p1 += dgVector (DG_FRACTURE_AABB_GUARD_DISTANCE) & dgVector::m_triplexMask;
}


dgInt32 dgCollisionCompoundFractured::CalculateContacts (dgCollidingPairCollector::dgPair* const pair, dgCollisionParamProxy& proxy) const
{
	dgInt32 count = dgCollisionCompound::CalculateContacts (pair, proxy);
	if (!count) {
		dgContact* const constraint = pair->m_contact;
		dgBody* const myBody = constraint->GetBody0();
		dgBody* const otherBody = constraint->GetBody1();
		dgVector relVeloc (otherBody->GetVelocity() - myBody->GetVelocity());
		dgAssert (relVeloc.m_w == dgFloat32 (0.0f));
		dgVector impulseStimate (relVeloc.Scale4 (dgFloat32 (1.0f) / (myBody->GetInvMass().m_w + otherBody->GetInvMass().m_w))); 
		dgFloat32 impulseStimate2 = impulseStimate.DotProduct4(impulseStimate).m_w;
		dgFloat32 impulseStrength = m_impulseStrengthPerUnitMass * myBody->GetMass().m_w;
		if (impulseStimate2 > (impulseStrength * impulseStrength)) {

			dgCollisionInstance* const myInstance = myBody->GetCollision();
			dgCollisionInstance* const otherInstance = otherBody->GetCollision();
			dgAssert (myInstance->GetChildShape() == this);

			dgContactPoint contactOut;
		
			dgFloat32 dist = ConvexRayCast (otherInstance, otherInstance->GetGlobalMatrix(), relVeloc, proxy.m_timestep, contactOut, myBody, myInstance, NULL, proxy.m_threadIndex);
			if (dist < proxy.m_timestep) {
				dgAssert (m_conectivityMap.Find(contactOut.m_collision0));
				dgConectivityGraph::dgListNode* const rootNode = m_conectivityMap.Find(contactOut.m_collision0)->GetInfo();
				dgDebriNodeInfo& nodeInfo = rootNode->GetInfo().m_nodeData;
				nodeInfo.m_lru = 1;
				SpawnSingleDrebree (myBody, rootNode, impulseStimate2, impulseStrength * impulseStrength);

//				Insert(node, nodeInfo.m_shapeNode->GetInfo()->GetShape());
//				dgAssert (0);
			}
		}
	}

	return count;
}

bool dgCollisionCompoundFractured::SanityCheck() const
{
	for (dgConectivityGraph::dgListNode* rootNode = m_conectivity.GetFirst(); rootNode; rootNode = rootNode->GetNext() ) {
		dgTree<dgConectivityGraph::dgListNode*, dgConectivityGraph::dgListNode*> filter(GetAllocator());
		for (dgGraphNode<dgDebriNodeInfo, dgSharedNodeMesh>::dgListNode* edgeNode = rootNode->GetInfo().GetFirst(); edgeNode; edgeNode = edgeNode->GetNext()) {
			dgConectivityGraph::dgListNode* const node = edgeNode->GetInfo().m_node;
			dgAssert (!filter.Find(node));
			filter.Insert(node, node);
		}
	}

	return true;
}

void dgCollisionCompoundFractured::SpawnSingleDrebree (dgBody* const myBody, dgConectivityGraph::dgListNode* const rootNode, dgFloat32 impulseStimate2, dgFloat32 impulseStimateCut2) const
{
	dgFloat32 strengthPool[256];
	dgConectivityGraph::dgListNode* pool[256];

	dgFloat32 attenuation = m_impulseAbsortionFactor;
	m_lru ++;
	dgInt32 stack = 1;
	pool[0] = rootNode;
	strengthPool[0] = impulseStimate2;
	while (stack) {
		stack --;
		dgFloat32 strenght = strengthPool[stack] * attenuation;
		dgConectivityGraph::dgListNode* const rootNode = pool[stack];

		if (strenght > impulseStimateCut2) {
			for (dgGraphNode<dgDebriNodeInfo, dgSharedNodeMesh>::dgListNode* edgeNode = rootNode->GetInfo().GetFirst(); edgeNode; edgeNode = edgeNode->GetNext()) {
				dgConectivityGraph::dgListNode* const node = edgeNode->GetInfo().m_node;
				dgDebriNodeInfo& childNodeInfo = node->GetInfo().m_nodeData;
				if (childNodeInfo.m_lru != m_lru) {
					childNodeInfo.m_lru = m_lru;
					strengthPool[stack] = strenght;
					pool[stack] = node;
					stack ++;
					dgAssert (stack < sizeof (pool)/sizeof (pool[0]));
				}
			}
		}
		dgDebriNodeInfo& nodeInfo = rootNode->GetInfo().m_nodeData;
	}
}
