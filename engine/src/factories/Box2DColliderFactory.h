#pragma once

#include "ColliderFactory.h"

class Box2DColliderFactory : public ColliderFactory
{
    public:
        Box2DColliderFactory();

        std::unique_ptr<BoxCollider> createBoxCollider(Vec2 s) override;
        std::unique_ptr<CircleCollider> createCircleCollider(float r) override;
        // std::unique_ptr<PolygonCollider> createPolygonCollider(std::vector<Vec2> p) override;
};