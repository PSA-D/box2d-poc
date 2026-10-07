#pragma once

#include <memory>
#include <vector>

struct Vec2;
class BoxCollider;
class CircleCollider;
class PolygonCollider;

// Abstract Factory
class ColliderFactory
{
    public:
        virtual ~ColliderFactory() = default;

        virtual std::unique_ptr<BoxCollider> createBoxCollider(Vec2 size) = 0;
        virtual std::unique_ptr<CircleCollider> createCircleCollider(float radius) = 0;
        // virtual std::unique_ptr<PolygonCollider> createPolygonCollider(std::vector<Vec2> pts) = 0;
};