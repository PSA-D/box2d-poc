#include "Box2DPhysicsWorld.h"

#include "../../include/GameObject.h"
#include "Box2DShape.h"

#include <stdexcept>

namespace // anonymous namespace, so linking doesn't fail
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

    Box2DShape* asBox2DShape(Collider* collider)
    {
        return dynamic_cast<Box2DShape*>(collider);
    }
}

Box2DPhysicsWorld::Box2DPhysicsWorld(Vec2 gravity)
{
    b2WorldDef def = b2DefaultWorldDef();
    def.gravity = b2Vec2{gravity.x, gravity.y};
    worldId = b2CreateWorld(&def);
}

Box2DPhysicsWorld::~Box2DPhysicsWorld()
{
    // Destroying the world destroys every body/shape, so tell colliders their shape is deleted
    for (auto& [id, entry] : entries)
        if (auto* shape = asBox2DShape(entry.gameObject->getCollider()))
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

    Collider* collider = go.getCollider();
    if (collider == nullptr)
        throw std::invalid_argument("Box2DPhysicsWorld::addObject: GameObject has no collider");

    Box2DShape* shape = asBox2DShape(collider);
    if (shape == nullptr)
        throw std::invalid_argument("Box2DPhysicsWorld::addObject: collider was not created by Box2DColliderFactory");

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = b2Vec2{go.position.x, go.position.y};
    bodyDef.rotation = b2MakeRot(go.rotation);

    if (const Rigidbody* rb = go.getRigidbody())
    {
        bodyDef.type          = toBox2D(rb->getBodyType());
        bodyDef.gravityScale  = rb->getGravityScale();
        bodyDef.fixedRotation = rb->isFixedRotation();
    }
    else
    {
        bodyDef.type = b2_staticBody; // no Rigidbody => scenery
    }

    const b2BodyId body = b2CreateBody(worldId, &bodyDef);
    shape->attachTo(body);

    entries.emplace(go.getId(), Entry{&go, body});
}

void Box2DPhysicsWorld::removeObject(GameObject& go)
{
    const auto it = entries.find(go.getId());
    if (it == entries.end())
        return;

    b2DestroyBody(it->second.body); // also destroys the body's shapes
    if (auto* shape = asBox2DShape(go.getCollider()))
        shape->detach();

    entries.erase(it);
}

bool Box2DPhysicsWorld::hasObject(const GameObject& go) const
{
    return entries.contains(go.getId());
}

void Box2DPhysicsWorld::syncTransforms()
{
    for (auto& [id, entry] : entries)
    {
        const b2Vec2 pos = b2Body_GetPosition(entry.body);
        entry.gameObject->position = Vec2{pos.x, pos.y};
        entry.gameObject->rotation = b2Rot_GetAngle(b2Body_GetRotation(entry.body));
    }
}
