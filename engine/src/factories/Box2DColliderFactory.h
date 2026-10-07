#pragma once

#include "../../include/Types.h"
#include "../../include/abstract/ColliderFactory.h"

// Concrete factory: every collider it creates is a Box2D collider.
class Box2DColliderFactory : public ColliderFactory
{
    public:
        Box2DColliderFactory() = default;

        std::unique_ptr<BoxCollider> createBoxCollider(Vec2 size) override;
        std::unique_ptr<CircleCollider> createCircleCollider(float radius) override;
        // std::unique_ptr<PolygonCollider> createPolygonCollider(std::vector<Vec2> p) override;
};
