#include <algorithm>
#include <map>
#include "MeshBuilder.h"
#include "pch.h"

STLMesh* MeshBuilder::GenerateMeshFromStlData(std::vector<Point>& iStlData, Point* iBB_min, Point* iBB_max)
{
    STLMesh* oMesh = NULL;
    if (iStlData.empty()) return oMesh;

    // Get/Evaluate Bounding Box data
    Point BoundingBox_Minpt;
    Point BoundingBox_Maxpt;
    if (iBB_min && iBB_max)
    {
        BoundingBox_Minpt = *iBB_min;
        BoundingBox_Maxpt = *iBB_max;
    }
    else
    {
        Point Minpt = iStlData[0];
        Point Maxpt = iStlData[0];

        for (Point p : iStlData) {
            if (p[0] < Minpt[0])
                Minpt[0] = p[0];
            else if (p[0] > Maxpt[0])
                Maxpt[0] = p[0];

            if (p[1] < Minpt[1])
                Minpt[1] = p[1];
            else if (p[1] > Maxpt[1])
                Maxpt[1] = p[1];

            if (p[2] < Minpt[2])
                Minpt[2] = p[2];
            else if (p[2] > Maxpt[2])
                Maxpt[2] = p[2];
        }

        BoundingBox_Minpt = Minpt;
        BoundingBox_Maxpt = Maxpt;
    }

    // Cell size
    double dx = 5 * Resolution;
    double dy = 5 * Resolution;
    double dz = 5 * Resolution;

    // Place all points in a Point Tree
    std::map<int, std::map<int, Leaf>> PointTree;

    #pragma region Collect_Points
    for (int idx = 0; idx < iStlData.size(); idx++)
    {
        Point pt = iStlData[idx];
        Point Local_pt = pt - BoundingBox_Minpt;

        int I = (int)(Local_pt[0] / dx);
        int J = (int)(Local_pt[1] / dy);
        int K = (int)(Local_pt[2] / dz);

        PointTree[I][J][K].push_back(idx);
    }
    #pragma endregion

    std::vector<std::vector<int>> neighbour;
    neighbour.reserve(26);

    #pragma region Neighbour_List
    //neighbour.push_back({ 0,0,0 });
    neighbour.push_back({ 0,0,1 });
    neighbour.push_back({ 0,0,-1 });
    neighbour.push_back({ 0,1,0 });
    neighbour.push_back({ 0,1,1 });
    neighbour.push_back({ 0,1,-1 });
    neighbour.push_back({ 0,-1,0 });
    neighbour.push_back({ 0,-1,1 });
    neighbour.push_back({ 0,-1,-1 });

    neighbour.push_back({ 1,0,0 });
    neighbour.push_back({ 1,0,1 });
    neighbour.push_back({ 1,0,-1 });
    neighbour.push_back({ 1,1,0 });
    neighbour.push_back({ 1,1,1 });
    neighbour.push_back({ 1,1,-1 });
    neighbour.push_back({ 1,-1,0 });
    neighbour.push_back({ 1,-1,1 });
    neighbour.push_back({ 1,-1,-1 });

    neighbour.push_back({ -1,0,0 });
    neighbour.push_back({ -1,0,1 });
    neighbour.push_back({ -1,0,-1 });
    neighbour.push_back({ -1,1,0 });
    neighbour.push_back({ -1,1,1 });
    neighbour.push_back({ -1,1,-1 });
    neighbour.push_back({ -1,-1,0 });
    neighbour.push_back({ -1,-1,1 });
    neighbour.push_back({ -1,-1,-1 });

    #pragma endregion

    std::vector<int> P2V(iStlData.size(), -1); // -1 means unassigned
    std::vector<bool> PointAlreadyProcessed(iStlData.size(), false);

    for (const auto& X : PointTree)
    {
        int I = X.first;
        for (const auto& Y : X.second)
        {
            int J = Y.first;
            for (const auto& Z : Y.second)
            {
                int K = Z.first;
                std::vector<int> Pt_Lst = Z.second;

                if (Pt_Lst.empty()) continue;

                // Mark all points as processed
                for (auto p : Pt_Lst)
                    PointAlreadyProcessed[p] = true;

                // Get all points from Neighbouring Cells
                std::vector<int> Neigh_pt_lst = Pt_Lst;
                for (const auto& neigh : neighbour)
                {
                    auto nptlst = PointTree[I + neigh[0]][J + neigh[1]][K + neigh[2]];

                    // If points belong to already processed cell, ignore points
                    if (nptlst.empty() || !PointAlreadyProcessed[nptlst[0]])
                        continue;

                    // Add additional constraint [Point should lie within resolution distance of cell]


                    Neigh_pt_lst.insert(Neigh_pt_lst.end(), nptlst.begin(), nptlst.end());
                }

                // Group points based on proximity
                GroupPointsByDistance(Pt_Lst, Neigh_pt_lst, P2V, iStlData);
            }
        }
    }

    oMesh = new STLMesh(iStlData, P2V, &BoundingBox_Minpt, &BoundingBox_Maxpt);
    return oMesh;
}

void MeshBuilder::GroupPointsByDistance(const std::vector<int>& iPt_lst, const std::vector<int>& iNeighP_tlst, std::vector<int>& ioP2V, const std::vector<Point>& iStlData)
{
    int group_id = *std::max_element(ioP2V.begin(), ioP2V.end()) + 1;
    if (group_id < 0) group_id = 0;

    for (int pt_idx : iPt_lst)
    {
        // Point is already assigned vertex
        if (ioP2V[pt_idx] != -1) continue;

        ioP2V[pt_idx] = group_id;
        // Proximity list:
        // Contains all points within resolution distance
        std::vector<int> Prox_lst = { pt_idx };

        for (int npt_idx : iNeighP_tlst)
        {
            // If same point or point already assigned vertex, ignore point
            if (pt_idx == npt_idx || ioP2V[npt_idx] != -1) continue;

            auto npt = iStlData[npt_idx];
            for (int idx : Prox_lst)
            {
                auto pt = iStlData[idx];
                double dist = (pt - npt).norm();
                if (dist <= Resolution)
                {
                    Prox_lst.push_back(npt_idx);
                    ioP2V[npt_idx] = group_id;
                    break;
                }
            }
        }

        ++group_id;
    }
}
