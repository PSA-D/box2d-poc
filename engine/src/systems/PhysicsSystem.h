#pragma once

#include "abstract/IPhysicsWorld.h"

#include <memory>
#include <utility>

class GameObject;
class Registry;
struct CollisionEvent;

class PhysicsSystem
{
    public:
        PhysicsSystem(Registry& registry, std::unique_ptr<IPhysicsWorld> world)
            : registry(registry), world(std::move(world)) {}

        void addObject(GameObject& go) { world->addObject(go); }
        void removeObject(GameObject& go) { world->removeObject(go); }

        void update(float dt);
        void dispatchEvents();

        void onCollisionDetected(CollisionEvent& event);
        void onCollisionExit(CollisionEvent& event);
        void onTriggerEnter(CollisionEvent& event);
        void onTriggerExit(CollisionEvent& event);

    private:
        static constexpr float FIXED_DT = 1.0f / 60.0f; // Fixed timestamp (60 FPS)
        static constexpr float MAX_ACCUMULATED = 0.25f; // Max accumulated time. Prevents an infinite spiral of updates

        Registry& registry;
        std::unique_ptr<IPhysicsWorld> world;
        float accumulator = 0.0f; // To check whether a step() needs to be taken
};
