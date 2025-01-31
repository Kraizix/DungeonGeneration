#include "Delaunay.h"

Triangle Delaunay::GetSuperTriangle(TArray<Point*>& points)
{
    float min_x = FLT_MAX, min_y = FLT_MAX;
    float max_x = -FLT_MAX, max_y = -FLT_MAX;

    for (auto& point : points)
    {
        min_x = FMath::Min(min_x, point->x);
        min_y = FMath::Min(min_y, point->y);
        max_x = FMath::Max(max_x, point->x);
        max_y = FMath::Max(max_y, point->y);
    }

    float dx = (max_x - min_x) * 10;
    float dy = (max_y - min_y) * 10;

    Point* p0 = new Point(min_x - dx, min_y - dy * 3);
    Point* p1 = new Point(min_x - dx, max_y + dy);
    Point* p2 = new Point(max_x + dx * 3, max_y + dy);

    return Triangle(p0, p1, p2);
}

TArray<Triangle> Delaunay::Triangulate(TArray<Point*>& points)
{
    Triangle SuperTriangle = GetSuperTriangle(points);

    TArray<Triangle> Triangles;
    Triangles.Add(SuperTriangle);

    for (auto& Point : points)
    {
        TArray<Edge> Edges;

        Triangles.RemoveAll([&](Triangle& triangle) {
            if (triangle.IsInCircle(Point))
            {
                Edges.Add(triangle.e1);
                Edges.Add(triangle.e2);
                Edges.Add(triangle.e3);
                return true;
            }
            return false;
        });
        TArray<Edge> NewEdges;
        for (int32 i = 0; i < Edges.Num(); ++i)
        {
            bool IsUnique = true;
            for (int32 j = 0; j < Edges.Num(); ++j)
            {
                if (i != j && Edges[i] == Edges[j])
                {
                    IsUnique = false;
                    break;
                }
            }
            if (IsUnique)
            {
                NewEdges.Add(Edges[i]);
            }
        }

        for (const Edge& edge : NewEdges)
        {
            Triangles.Add(Triangle(edge.p1, edge.p2, Point));
        }
    }

    Triangles.RemoveAll([&](const Triangle& Triangle) {
        return (Triangle.p1 == SuperTriangle.p1 || Triangle.p1 == SuperTriangle.p2 || Triangle.p1 == SuperTriangle.p3 ||
                Triangle.p2 == SuperTriangle.p1 || Triangle.p2 == SuperTriangle.p2 || Triangle.p2 == SuperTriangle.p3 ||
                Triangle.p3 == SuperTriangle.p1 || Triangle.p3 == SuperTriangle.p2 || Triangle.p3 == SuperTriangle.p3);
    });

    return Triangles;
}
