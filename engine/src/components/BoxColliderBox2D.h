#pragma once

#include "../../include/abstract/BoxCollider.h"
#include "Box2DShape.h"

class BoxColliderBox2D : public BoxCollider, public Box2DShape
{
    public:
        using BoxCollider::BoxCollider; // BoxCollider(Vec2 size)

        bool collidesWith(const Collider& other) const override;
        void attachTo(b2BodyId body) override;
};
