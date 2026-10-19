#include "PhysicsSystem.h"

#include "Registry.h"
#include "Transform.h"
#include "abstract/Collider.h"

#include <algorithm>
#include <iostream>
#include <ostream>

void PhysicsSystem::update(float dt)
{
    // Sync colliders
    registry.each<Collider>([this](GameObject go, Collider& c)
    {
        if (registry.has<Transform>(go) && !world->hasObject(go) && c.isActive())
            world->addObject(go);
        else if (!c.isActive())
            world->removeObject(go);
    });

    accumulator += std::min(dt, MAX_ACCUMULATED); // Safety limit

    // Sync the physics world
    while (accumulator >= FIXED_DT)
    {
        world->step(FIXED_DT);
        accumulator -= FIXED_DT;
        dispatchEvents();
    }

    world->syncTransforms();
}

void PhysicsSystem::dispatchEvents()
{
    for (auto& collisionEvent : world->consumeCollisionEvents())
    {
        switch (collisionEvent.type)
        {
            case CollisionType::Begin:
                collisionEvent.isTrigger
                    ? onTriggerEnter(collisionEvent)
                    : onCollisionDetected(collisionEvent);

                break;
            case CollisionType::End:
                collisionEvent.isTrigger
                    ? onTriggerExit(collisionEvent)
                    : onCollisionExit(collisionEvent);
                break;
            default:
                std::cout << "Unknown collision type" << std::endl;
                break;
        }
    }
}

void PhysicsSystem::onCollisionDetected(CollisionEvent& event)
{
    std::cout << "Collision detected with objects " << event.one.getId() << " and " << event.two.getId() << '\n';
}

void PhysicsSystem::onCollisionExit(CollisionEvent& event)
{
    std::cout << "Collision exited with objects " << event.one.getId() << " and " << event.two.getId() << '\n';
}

void PhysicsSystem::onTriggerEnter(CollisionEvent& event)
{
    std::cout << "Trigger " << event.one.getId() << " entered with object " << event.two.getId() << '\n';
}

void PhysicsSystem::onTriggerExit(CollisionEvent& event)
{
    std::cout << "Trigger " << event.one.getId() << " exited with object " << event.two.getId() << '\n';
}

