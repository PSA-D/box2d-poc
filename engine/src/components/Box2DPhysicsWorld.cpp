#include "Box2DPhysicsWorld.h"

#include <stdexcept>
#include <vector>

#include "../../include/RigidBody.h"
#include "../../include/Types.h"
#include "../../include/abstract/Collider.h"
#include "GameObject.h"
#include "Registry.h"
#include "Box2DShape.h"

namespace
{
    b2BodyType toBox2D(BodyType type)
    {
        switch (type)
        {
            case BodyType::Static:    return b2_staticBody;
            case BodyType::Kinematic: return b2_kinematicBody;
            case BodyType::Dynamic:   return b2_dynamicBody;
        }
        return b2_staticBody;
    }
}

Box2DPhysicsWorld::Box2DPhysicsWorld(Registry& registry, Vec2 gravity) : registry(registry)
{
    b2WorldDef def = b2DefaultWorldDef();
    def.gravity = b2Vec2{gravity.x, gravity.y};
    worldId = b2CreateWorld(&def);
}

Box2DPhysicsWorld::~Box2DPhysicsWorld()
{
    // Destroying the world destroys every shape, so tell the colliders.
    for (auto& [id, body] : bodies)
        if (auto* shape = dynamic_cast<Box2DShape*>(registry.get<Collider>(GameObject{id})))
            shape->detach();

    b2DestroyWorld(worldId);
}

void Box2DPhysicsWorld::step(float fixedDt)
{
    b2World_Step(worldId, fixedDt, SUB_STEPS);
}

void Box2DPhysicsWorld::addObject(GameObject& go)
{
    if (hasObject(go))
        return;

    const Transform* transform = registry.get<Transform>(go);
    Collider* collider = registry.get<Collider>(go);
    if (transform == nullptr || collider == nullptr)
        throw std::invalid_argument("Box2DPhysicsWorld::addObject: entity needs a Transform and a Collider");

    auto* shape = dynamic_cast<Box2DShape*>(collider);
    if (shape == nullptr)
        throw std::invalid_argument("Box2DPhysicsWorld::addObject: collider was not created by Box2DColliderFactory");

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = b2Vec2{transform->position.x, transform->position.y};
    bodyDef.rotation = b2MakeRot(transform->rotation);

    if (const Rigidbody* rb = registry.get<Rigidbody>(go))
    {
        bodyDef.type          = toBox2D(rb->getBodyType());
        bodyDef.gravityScale  = rb->getGravityScale();
        bodyDef.fixedRotation = rb->isFixedRotation();
    }
    else
    {
        bodyDef.type = b2_staticBody;
    }

    const b2BodyId body = b2CreateBody(worldId, &bodyDef);
    shape->attachTo(body);
    bodies.emplace(go.getId(), body);
}

void Box2DPhysicsWorld::removeObject(GameObject& go)
{
    const auto it = bodies.find(go.getId());
    if (it == bodies.end())
        return;

    b2DestroyBody(it->second); // also destroys its shapes
    if (auto* shape = dynamic_cast<Box2DShape*>(registry.get<Collider>(go)))
        shape->detach();

    bodies.erase(it);
}

bool Box2DPhysicsWorld::hasObject(const GameObject& go) const
{
    return bodies.contains(go.getId());
}

void Box2DPhysicsWorld::syncTransforms()
{
    std::vector<GameObject> stale;

    for (const auto& [id, body] : bodies)
    {
        GameObject go{id};
        Transform* transform = registry.get<Transform>(go);
        if (transform == nullptr || !registry.has<Collider>(go))
        {
            stale.push_back(go);
            continue;
        }

        const b2Vec2 pos = b2Body_GetPosition(body);
        transform->position = Vec2{pos.x, pos.y};
        transform->rotation = b2Rot_GetAngle(b2Body_GetRotation(body));
    }

    for (GameObject& go : stale)
        removeObject(go);
}
