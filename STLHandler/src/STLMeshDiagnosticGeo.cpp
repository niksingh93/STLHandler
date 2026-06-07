#include "STLMeshDiagnosticGeo.h"
#include <queue>
#include <cmath>
#include "pch.h"

//DetectDegenerateTriangles : Finds degenerate triangles
// Triangles with edge < Reso or area < AreaReso
// Returns faceID of degenerate triangles
std::vector<int> STLMeshDiagnosticGeo::DetectDegenerateTriangles()
{
    if (!_mesh || _mesh->GetNumFaces() == 0) return std::vector<int>();

    std::vector<int> DegenerateFaceIdx;
    int numFaces = _mesh->GetNumFaces();

    // Can be made parallel later
    for (int fidx = 0; fidx < numFaces; fidx++)
    {
        TriFace face = _mesh->GetFace(fidx);
        if (face.Area() < AreaReso)
        {
            DegenerateFaceIdx.push_back(fidx);
            continue;
        }

        Point p1 = face.GetVertex(0).GetPoint();
        Point p2 = face.GetVertex(1).GetPoint();
        Point p3 = face.GetVertex(2).GetPoint();

        if ((p1 - p2).norm() < Resolution ||
            (p2 - p3).norm() < Resolution ||
            (p3 - p1).norm() < Resolution)
        {
            DegenerateFaceIdx.push_back(fidx);
        }

    }
    return DegenerateFaceIdx;
}


//DetectInvertedNormals : Detects Triangles which point inwards
// Finds a reference triangle using ray firing
// Finds orientation of triangles w.r.t. reference triangle using concepts of Half-Edge
std::vector<int> STLMeshDiagnosticGeo::DetectInvertedNormals()
{
    #pragma region Identifying Reference Face
    // Check if mesh has vertices
    if (!_mesh || _mesh->GetNumVertices() == 0) return std::vector<int>();

    // Find vertex with max Z.
    Vertex topVertex = _mesh->GetVertex(0);
    double maxZ = topVertex.GetZ();
    for (int vid = 0; vid < _mesh->GetNumVertices(); vid++) 
    {
        if (_mesh->GetVertex(vid).GetZ() > maxZ)
        {
            topVertex = _mesh->GetVertex(vid);
            maxZ = topVertex.GetZ();
        }
    }

    std::vector<int> topFaces = topVertex.GetNeighbouringFaces();

    if (topFaces.empty())
        throw std::runtime_error("STLMeshDiagnosticGeo::DetectInvertedNormals");

    int RefFaceId = -1;

    // Find face with highest centroid of all topFaces.
    Point cent_maxz = _mesh->GetFace(topFaces[0]).GetCentroid();
    double normal_z = 0;
    for (auto fid : topFaces)
    {
        Point cent = _mesh->GetFace(fid).GetCentroid();
        double normal_z = _mesh->GetFace(fid).GetNormal()[2];
        if (cent[2] > cent_maxz[2] && std::abs(normal_z) > Resolution)
        {
            RefFaceId = fid;
            cent_maxz = cent;
        }
    }

    // Point close to the top. (Reduces the chances to algo failing)
    Point RayPoint = 0.1 * cent_maxz + 0.9 * topVertex.GetPoint();

    // Shooting ray in +ve Z-direction and checking for intersection
    std::vector<int> RayIntersectFaces; 
    std::vector<Point> IntersectionPoints;
    FacesIntersectingRay(RayPoint, { 0,0,1 }, topFaces, RayIntersectFaces, IntersectionPoints);

    if (!RayIntersectFaces.empty())
    {
        Point highestPoint = RayPoint;
        for (int i = 0; i < RayIntersectFaces.size(); i++)
        {
            if (IntersectionPoints[i][2] > highestPoint[2])
            {
                RefFaceId = RayIntersectFaces[i];
                highestPoint = IntersectionPoints[i];
            }
        }

    }

    if (RefFaceId == -1)
        throw std::runtime_error("STLMeshDiagnosticGeo::DetectInvertedNormals");
    #pragma endregion


    #pragma region Check Normal Orientation
    std::vector<bool> IsCorrectlyOriented(_mesh->GetNumFaces(),false);
    if (_mesh->GetFace(RefFaceId).GetNormal()[2] > Resolution)
        IsCorrectlyOriented[RefFaceId] = true;
    else
        IsCorrectlyOriented[RefFaceId] = false;

    std::vector<bool> IsOriDetermined(_mesh->GetNumFaces(), false);
    IsOriDetermined[RefFaceId] = true;

    std::queue<int> QueuedFaces;
    QueuedFaces.push(RefFaceId);

    std::vector<int> oFaceWithInvNormal;
    while (!QueuedFaces.empty())
    {
        int priFaceID = QueuedFaces.front();
        QueuedFaces.pop();
        TriFace PrimaryFace = _mesh->GetFace(priFaceID);

        std::vector<int> adjFaces = PrimaryFace.GetAdjacentFaceList();

        // Check Orientation and Queue adjacent faces
        for (int f : adjFaces) 
        {
            // If orientation already determined : continue
            if (IsOriDetermined[f]) continue;
            
            TriFace face = _mesh->GetFace(f);

            if (HaveSameOri(PrimaryFace, face))
                IsCorrectlyOriented[f] = IsCorrectlyOriented[priFaceID];
            else
                IsCorrectlyOriented[f] = !IsCorrectlyOriented[priFaceID];

            // Add face to queue
            QueuedFaces.push(f);
            IsOriDetermined[f] = true;

            // If orientation inverted, add to inverted list
            if (!IsCorrectlyOriented[f])
                oFaceWithInvNormal.push_back(f);
        }
    }

    #pragma endregion

    return oFaceWithInvNormal;
}

// HaveSameOri : Only to be used with edge adjacent triangles
// Returns true if Orientation is consistent
bool STLMeshDiagnosticGeo::HaveSameOri(TriFace& iFirstFace, 
                                       TriFace& iSecondFace)
{
    const auto& firstVerts = iFirstFace.GetVertexIds();
    const auto& secondVerts = iSecondFace.GetVertexIds();

    // Find common edge vertices
    std::vector<int> commonVerts;

    for (int v1 : firstVerts)
    {
        for (int v2 : secondVerts)
        {
            if (v1 == v2)
            {
                commonVerts.push_back(v1);
            }
        }
    }

    // Triangles must share exactly one edge (2 common vertices)
    if (commonVerts.size() != 2)
    {
        throw std::runtime_error("STLMeshDiagnosticGeo::HaveSameOri");
    }

    int edgeV0 = commonVerts[0];
    int edgeV1 = commonVerts[1];

    // Find edge direction in first face
    int firstDir = 0;
    for (int i = 0; i < 3; ++i)
    {
        int curr = firstVerts[i];
        int next = firstVerts[(i + 1) % 3];

        if (curr == edgeV0 && next == edgeV1)
        {
            firstDir = 1;
            break;
        }

        if (curr == edgeV1 && next == edgeV0)
        {
            firstDir = -1;
            break;
        }
    }

    // Find edge direction in second face
    int secondDir = 0;
    for (int i = 0; i < 3; ++i)
    {
        int curr = secondVerts[i];
        int next = secondVerts[(i + 1) % 3];

        if (curr == edgeV0 && next == edgeV1)
        {
            secondDir = 1;
            break;
        }

        if (curr == edgeV1 && next == edgeV0)
        {
            secondDir = -1;
            break;
        }
    }

    // Same orientation means shared edge direction is opposite
    return (firstDir != secondDir);
}

// Ray firing (Möller–Trumbore algorithm)
// GPT generated code
// Solution to the below equation
//rayOrigin + t * rayDir = v0 + u * edge1 + v * edge2
void STLMeshDiagnosticGeo::FacesIntersectingRay(
    const Point& ipoint, 
    const Eigen::Vector3d idir,
    const std::vector<int>& itopFaces,
    std::vector<int>& oIntersectingFaces,
    std::vector<Point>& oIntersectionPoints) const
{
    std::vector<Point> intersectingPoint;

    const Point rayOrigin = ipoint;
    const Eigen::Vector3d rayDir = idir;

    constexpr double EPS = 1e-8;

    for (int fid : itopFaces)
    {
        TriFace face = _mesh->GetFace(fid);

        Point v0 = face.GetVertex(0).GetPoint();
        Point v1 = face.GetVertex(1).GetPoint();
        Point v2 = face.GetVertex(2).GetPoint();

        Eigen::Vector3d edge1 = v1 - v0;
        Eigen::Vector3d edge2 = v2 - v0;

        Eigen::Vector3d h = rayDir.cross(edge2);

        double a = edge1.dot(h);

        // Ray parallel to triangle
        if (std::abs(a) < EPS) continue;

        double f = 1.0 / a;

        Eigen::Vector3d s = rayOrigin - v0;

        // First Barycentric co-ordinate
        double u = f * s.dot(h);

        // Ray does not inversect with Face
        if (u < 0.0 || u > 1.0)
            continue;

        Eigen::Vector3d q = s.cross(edge1);

        // Second Barycentric co-ordinate
        double v = f * rayDir.dot(q);

        // Ray does not inversect with Face
        if (v < 0.0 || (u + v) > 1.0)
            continue;

        double t = f * edge2.dot(q);

        // Intersection exists in +Z direction
        if (t > EPS)
        {
            intersectingPoint.push_back(rayOrigin + t * rayDir);
            oIntersectingFaces.push_back(fid);
        }
    }
}