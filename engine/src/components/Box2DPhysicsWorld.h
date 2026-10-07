#pragma once

#include <unordered_map>

#include <box2d/box2d.h>

#include "../../include/IPhysicsWorld.h"
#include "../../include/Types.h"

// Adapter: translates the engine's IPhysicsWorld interface into Box2D (v3) calls.
class Box2DPhysicsWorld : public IPhysicsWorld
{
    public:
        explicit Box2DPhysicsWorld(Vec2 gravity = {0.0f, -10.0f});
        ~Box2DPhysicsWorld() override;

        Box2DPhysicsWorld(const Box2DPhysicsWorld&) = delete;
        Box2DPhysicsWorld& operator=(const Box2DPhysicsWorld&) = delete;

        void step(float fixedDt) override;

        // Creates a b2Body from go's Rigidbody (static if it has none) and attaches go's collider.
        // Throws std::invalid_argument if go has no collider, or one not made by Box2DColliderFactory.
        void addObject(GameObject& go) override;
        void removeObject(GameObject& go) override;
        bool hasObject(const GameObject& go) const override;

        // Copies each body's position/angle back into its GameObject.
        void syncTransforms() override;

    private:
        struct Entry
        {
            GameObject* gameObject;
            b2BodyId    body;
        };

        static constexpr int SUB_STEPS = 4;

        b2WorldId worldId;
        std::unordered_map<int, Entry> entries; // keyed by GameObject::getId()
};
