#include "STLMeshDiagnosticTopo.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include "pch.h"

// Detects independent shells
// Finds independent set of triangles which maintain edge connectivity
void STLMeshDiagnosticTopo::DetectNoiseShells(std::vector<std::vector<int>> oIndepedentFaceList)
{
	if (!_mesh || _mesh->GetNumFaces() == 0) return;

	std::vector<bool> IsProcessed(_mesh->GetNumFaces(), false);
	std::queue<int> FaceQueue;

	
	for (int fid = 0; fid < _mesh->GetNumFaces(); fid++)
	{
		if (IsProcessed[fid]) continue;

		FaceQueue.push(fid); 
		std::vector<int> faceList;
		// While loop traverses through all edge connected triangles
		while (!FaceQueue.empty())
		{
			int fid = FaceQueue.front();
			std::vector<int> adjFaceList = _mesh->GetFace(fid).GetAdjacentFaceList();
			IsProcessed[fid] = true;
			faceList.push_back(fid);
			FaceQueue.pop();

			for (auto f : adjFaceList)
			{
				if (IsProcessed[f]) continue;
				FaceQueue.push(f);
			}
		}

		oIndepedentFaceList.push_back(faceList);
	}

}


// Detects Edges which are present in only one face
void STLMeshDiagnosticTopo::DetectOpenEdges()
{
	std::unordered_map<int, std::pair<int,int>> OpenEdgeList;
	for (int fid = 0; fid < _mesh->GetNumFaces(); fid++)
	{
		TriFace face = _mesh->GetFace(fid);
		std::vector<int> vertices = face.GetVertexIds();

		// Get list of adjacent faces for each vertex
		std::vector<std::set<int>> AdjFaces;
		for (int v = 0; v < 3; v++)
		{
			auto list = face.GetVertex(v).GetNeighbouringFaces();
			std::set<int> adjFace;
			for (auto f : list)
			{
				adjFace.insert(f);
			}
			AdjFaces.push_back(adjFace);
		}

		// Find intersection of adjacent faces for each pair of vertex
		// This will give faces which share an edge.
		// If edge is present in only one face, it's an open edge.
		for (int v = 0; v < 3; v++)
		{
			std::set<int> intersect;
			std::set_intersection(AdjFaces[v].begin(), AdjFaces[v].end(),
								  AdjFaces[(v + 1) % 3].begin(), AdjFaces[(v + 1) % 3].end(),
								  std::inserter(intersect, intersect.begin()));

			if (intersect.size() < 2)
			{
				// Open edge detected. vertices[v] & vertices[(v+1)%3].
				OpenEdgeList[fid] = { vertices[v] , vertices[(v + 1) % 3] };
			}
		}
	}
}
