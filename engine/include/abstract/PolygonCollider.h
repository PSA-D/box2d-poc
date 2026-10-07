#pragma once

#include <vector>

struct Vec2;

class PolygonCollider
{
    public:
        virtual ~PolygonCollider() = default;

        virtual const std::vector<Vec2>& getPoints() const { return points; };
        // virtual void setPoints(const std::vector<Vec2>& pts) { points = pts; };
    protected:
        std::vector<Vec2> points;
};