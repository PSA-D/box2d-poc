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
    registry.each<Collider>([this](GameObject go, Collider&)
    {
        if (registry.has<Transform>(go) && !world->hasObject(go))
            world->addObject(go);
    });

    accumulator += std::min(dt, MAX_ACCUMULATED); // Safety limit

    // Sync the physics world
    while (accumulator >= FIXED_DT)
    {
        world->step(FIXED_DT);

        std::map<CollisionType, CollisionEvent> events = world->consumeCollisionEvents();

        for (auto [type, event] : events)
        {
            switch (type)
            {
                case CollisionType::Begin:
                    onCollisionDetected(event);
                    break;
                case CollisionType::End:
                    onCollisionExit(event);
                    break;
            }
        }

        accumulator -= FIXED_DT;
    }

    world->syncTransforms();
}

void PhysicsSystem::onCollisionDetected(CollisionEvent& event)
{
    std::cout << "Collision detected with objects " << event.one.getId() << " and " << event.two.getId() << '\n';
}

void PhysicsSystem::onCollisionExit(CollisionEvent& event)
{
    std::cout << "Collision exited with objects " << event.one.getId() << " and " << event.two.getId() << '\n';
}
