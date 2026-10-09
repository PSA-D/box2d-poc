#pragma once

#include <unordered_map>

#include <box2d/box2d.h>

#include "../../include/IPhysicsWorld.h"
#include "../../include/Types.h"

class Registry;

// Adapter: translates IPhysicsWorld into Box2D calls.
class Box2DPhysicsWorld : public IPhysicsWorld
{
    public:
        explicit Box2DPhysicsWorld(Registry& registry, Vec2 gravity = {0.0f, -10.0f});
        ~Box2DPhysicsWorld() override;

        // No copies
        Box2DPhysicsWorld(const Box2DPhysicsWorld&) = delete;
        Box2DPhysicsWorld& operator=(const Box2DPhysicsWorld&) = delete;

        void step(float fixedDt) override;

        // Needs Transform + Collider, Rigidbody is optional (no Rigidbody = static)
        void addObject(GameObject& go) override;
        void removeObject(GameObject& go) override;
        bool hasObject(const GameObject& go) const override;

        void syncTransforms() override;

    private:
        static constexpr int SUB_STEPS = 4; // Amount of substeps in the step()

        Registry& registry;
        b2WorldId worldId;
        std::unordered_map<int, b2BodyId> bodies; // keyed by GameObject::getId()
};
