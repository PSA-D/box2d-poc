#pragma once

#include "abstract/BoxCollider.h"
#include "Box2DShape.h"

class BoxColliderBox2D : public BoxCollider, public Box2DShape
{
    public:
        using BoxCollider::BoxCollider; // BoxCollider(Vec2 size)

        void attachTo(b2BodyId body) override;
};
