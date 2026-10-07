#pragma once

#include "Collider.h"

class CircleCollider : Collider
{
    public:
        CircleCollider() = default;
        CircleCollider(const float& radius) : radius(radius) {}

        virtual ~CircleCollider() = default;

        virtual float getRadius() const { return radius; };
        virtual void setRadius(const float newRadius) { radius = newRadius; };
    protected:
        float radius {0};
};