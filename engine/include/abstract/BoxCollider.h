#pragma once

#include "Collider.h"

class BoxCollider : Collider
{
    public:
        BoxCollider() = default;
        BoxCollider(Vec2 size) : size(size) { };

        virtual ~BoxCollider() = default;

        virtual Vec2 getSize() const { return size; };
        virtual void setSize(const Vec2& newSize) { size = newSize; };

        bool collidesWith(const Collider& collider) const override = 0;
    protected:
        Vec2 size { 0, 0 };
};