#pragma once

#include "abstract/CircleCollider.h"
#include "Box2DShape.h"

class CircleColliderBox2D : public CircleCollider, public Box2DShape
{
    public:
        using CircleCollider::CircleCollider; // CircleCollider(const float& radius)

        bool collidesWith(const Collider& other) const override;
        void attachTo(b2BodyId body) override;
};
