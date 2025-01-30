#pragma once

#include "Math/UnrealMathUtility.h"


struct Point
{
    Point(float _x = 0, float _y = 0) : x(_x), y(_y), weight(FLT_MAX) {}
    float x;
    float y;
    float weight;
    bool visited = false;

    /**
     *  Get the distance between current point and specified point
     * @param _p Other point
     * @return Distance between the 2 points
     */
    double Distance(Point _p) const
    {
        return FMath::Sqrt(FMath::Pow((_p.x - x), 2) + FMath::Pow((_p.y - y), 2));
    }

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Edge
{
    Edge(Point* _p1 = nullptr, Point* _p2 = nullptr) : p1(_p1), p2(_p2) {}
    Point* p1;
    Point* p2;

    /**
     * Get the length of the edge(distance between the 2 points)
     * @return Length of the edge
     */
    double Length() const
    {
        return p1->Distance(*p2);
    }

    bool operator==(const Edge& other) const
    {
        return (*p1 == *other.p1 && *p2 == *other.p2) || (*p1 == *other.p2 && *p2 == *other.p1);
    }
};

struct Triangle
{
    Triangle(Point* _p1, Point* _p2, Point* _p3) : p1(_p1), p2(_p2), p3(_p3)
    {
        e1 = Edge(p1, p2);
        e2 = Edge(p2, p3);
        e3 = Edge(p3, p1);
    }

    Point* p1;
    Point* p2;
    Point* p3;
    Edge e1;
    Edge e2;
    Edge e3;

    /**
     * Check if the point is inside the circumcircle of the triangle
     * @param _p Point to check
     * @return Whether point is inside the circumcircle of the triangle
     */
    bool IsInCircle(Point* _p)
    {
        double D = (p1->x * (p2->y - p3->y) + p2->x * (p3->y - p1->y) + p3->x * (p1->y - p2->y)) * 2;

        // Calculate circumcenter
        double cx = ((p1->x * p1->x + p1->y * p1->y) * (p2->y - p3->y) +
                     (p2->x * p2->x + p2->y * p2->y) * (p3->y - p1->y) +
                     (p3->x * p3->x + p3->y * p3->y) * (p1->y - p2->y)) / D;

        double cy = ((p1->x * p1->x + p1->y * p1->y) * (p3->x - p2->x) +
                     (p2->x * p2->x + p2->y * p2->y) * (p1->x - p3->x) +
                     (p3->x * p3->x + p3->y * p3->y) * (p2->x - p1->x)) / D;

        Point center = Point(cx, cy);

        double a = e1.Length();
        double b = e2.Length();
        double c = e3.Length();

        // Calculate radius
        double s = (a + b + c) / 2.0;
        double radius = (a * b * c) / (4.0 * FMath::Sqrt(s * (s - a) * (s - b) * (s - c)));

        double dx = center.x - _p->x;
        double dy = center.y - _p->y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }
};
