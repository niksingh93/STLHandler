#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
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
};