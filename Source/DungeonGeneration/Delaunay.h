#pragma once
#include "Containers/Array.h"
#include "DataStruct.h"

class Delaunay
{
    /**
     * Create a triangle that contains all the points passed within it's area
     * @param points List of points used to create the super triangle
     * @return Triangle which contains all the points
     */
    Triangle GetSuperTriangle(TArray<Point*>& points);

public:
    /**
     * Triangulate using the points specified using Delaunay triangulation
     * @param points 
     * @return 
     */
    TArray<Triangle> Triangulate(TArray<Point*>& points);
};
