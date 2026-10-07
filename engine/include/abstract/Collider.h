#pragma once

#include "../Types.h"
#include "Component.h"

class Collider : Component
{
    public:
        virtual ~Collider() = default;

        [[nodiscard]] virtual bool collidesWith(const Collider& collider) const = 0;

        virtual Vec2 getOffset() const { return offset; };
        virtual void setOffset(const Vec2& newOffset) { offset = newOffset; };

        virtual float getDensity() const { return density; };
        virtual void setDensity(const float newDensity) { density = newDensity; };

        virtual float getFriction() const { return friction; };
        virtual void setFriction(const float newFriction) { friction = newFriction; };

        virtual float getRestitution() const { return restitution; };
        virtual void setRestitution(const float newRestitution) { restitution = newRestitution; };

        virtual bool isTrigger() const { return trigger; };
        virtual void setTrigger(const bool newTrigger) { trigger = newTrigger; };

    protected:
        Vec2  offset {0, 0};
        float density { 1.0 }; // How heavy an object is for it's size. It influences the mass of the RigidBody
        float friction { 0.3 }; // How much sliding (0.0-1.0)
        float restitution { 0.3 }; // Bounciness (0.0-1.0)
        bool trigger { false }; // Whether it's a trigger or an object
};