#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Definitions.h"
#include "STLHandlerExports.h"



class STLHANDLER_API  STLFileHandler
{
public:
	static std::vector<Point> ReadFile(std::string iFile, Point* oMin = NULL, Point* oMax = NULL) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(iFile,aiProcess_Triangulate | aiProcess_GenBoundingBoxes);

        if (!scene || !scene->HasMeshes()) {
            std::cerr << "Error loading STL: "<< importer.GetErrorString()<< std::endl;
            return std::vector<Point>();
        }

        aiMesh* mesh = scene->mMeshes[0];

        // Copy data to points
        std::vector<Point> oStlPointData;
        oStlPointData.resize(mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; i++) {
            auto v = mesh->mVertices[i];
            oStlPointData[i] = { v.x , v.y , v.z };
        }

        if (oMax)
        {
            aiVector3D BB_Maxpt = mesh->mAABB.mMax;
            *oMax = Point{ BB_Maxpt.x , BB_Maxpt.y , BB_Maxpt.z };
        }
        

        if (oMin)
        {
            aiVector3D BB_Minpt = mesh->mAABB.mMin;
            *oMin = Point{ BB_Minpt.x , BB_Minpt.y , BB_Minpt.z };
        }
        
        
        return oStlPointData; // scene->mMeshes[0];
	}

    //static void WriteFile(std::string iFile);
    static void WriteFile(std::string iFile, STLMesh _mesh)
    {
        std::ofstream file(iFile);

        file << "solid mesh\n";

        for (int fid = 0; fid < _mesh.GetNumFaces(); fid++)
        {
            auto vertexIds = _mesh.GetFace(fid).GetVertexIds();

            Point p0 = _mesh.GetVertex(vertexIds[0]).GetPoint();
            Point p1 = _mesh.GetVertex(vertexIds[1]).GetPoint();
            Point p2 = _mesh.GetVertex(vertexIds[2]).GetPoint();

            Point normal = (p1 - p0).cross(p2 - p0).normalized();

            file << "facet normal "
                << normal.x() << " "
                << normal.y() << " "
                << normal.z() << "\n";

            file << "outer loop\n";

            file << "vertex " << p0.x() << " " << p0.y() << " " << p0.z() << "\n";
            file << "vertex " << p1.x() << " " << p1.y() << " " << p1.z() << "\n";
            file << "vertex " << p2.x() << " " << p2.y() << " " << p2.z() << "\n";

            file << "endloop\n";
            file << "endfacet\n";
        }

        file << "endsolid mesh\n";
    }
};