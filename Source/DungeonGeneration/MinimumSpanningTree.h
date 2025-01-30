#pragma once
#include "DataStruct.h"

class MinimumSpanningTree
{
public:
    /**
     * Get the edge with the lowest weight
     * @param Edges Edges
     * @return 
     */
    Edge FindMinWeightEdge(TArray<Edge>& Edges);

    /**
     * Build a minimum spanning tree using Prim's algorithm from an array of triangles
     * @param Triangles Triangles to use for the minimum spanning tree algorithm
     * @return Edges corresponding to the minimum spanning tree
     */
    TArray<Edge> BuildTree(const TArray<Triangle>& Triangles);
};
