#include "MinimumSpanningTree.h"

Edge MinimumSpanningTree::FindMinWeightEdge(TArray<Edge>& Edges)
{
    float MinDistance = MAX_FLT;
    Edge MinEdge;

    for (Edge& edge : Edges)
    {
        if ((edge.p1->visited ^ edge.p2->visited) && edge.Length() < MinDistance)
        {
            MinDistance = edge.Length();
            MinEdge = edge;
        }
    }
    return MinEdge;
}

TArray<Edge> MinimumSpanningTree::BuildTree(const TArray<Triangle>& Triangles)
{
    TArray<Edge> Result;
    TArray<Edge> AllEdges;

    // Get all edges from all the triangles
    for (const Triangle& Triangle : Triangles)
    {
        AllEdges.Add(Triangle.e1);
        AllEdges.Add(Triangle.e2);
        AllEdges.Add(Triangle.e3);
    }

    // Deduplicates edges
    for (int32 i = AllEdges.Num() - 1; i >= 0; --i)
    {
        for (int32 j = i - 1; j >= 0; --j)
        {
            if (AllEdges[i] == AllEdges[j])
            {
                AllEdges.RemoveAt(i);
                break;
            }
        }
    }

    if (AllEdges.Num() == 0)
    {
        return Result;
    }

    AllEdges[0].p1->visited = true;
    // Prim Algorithm
    while (true)
    {
        Edge MinEdge = FindMinWeightEdge(AllEdges);

        if (MinEdge.p1 == nullptr)
        {
            break;
        }

        Result.Add(MinEdge);

        MinEdge.p2->visited = true;
        MinEdge.p1->visited = true;
    }

    return Result;
}
