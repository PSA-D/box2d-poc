#pragma once

class GameObject;

class IPhysicsWorld
{
public:
    virtual ~IPhysicsWorld() = default;

    virtual void step(float fixedDt) = 0; // Time step for collision detection etc.

    // Object-handling for the physics world
    virtual void addObject(GameObject& go) = 0;
    virtual void removeObject(GameObject& go) = 0;
    virtual bool hasObject(const GameObject& go) const = 0;

    virtual void syncTransforms() = 0; // Sync the transforms with the colliders
};