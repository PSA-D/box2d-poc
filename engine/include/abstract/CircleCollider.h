#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
    public:
        CircleCollider() = default;
        CircleCollider(const float& radius) : radius(radius) {}

        virtual ~CircleCollider() = default;

        virtual float getRadius() const { return radius; };
        virtual void setRadius(const float newRadius) { radius = newRadius; };

        bool collidesWith(const Collider& collider) const override = 0;
    protected:
        float radius {0};
};
