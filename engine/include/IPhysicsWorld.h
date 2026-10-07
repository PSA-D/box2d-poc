#pragma once

class GameObject;

class IPhysicsWorld
{
public:
    virtual ~IPhysicsWorld() = default;

    virtual void step(float fixedDt) = 0;

    virtual void addObject(GameObject& go) = 0;
    virtual void removeObject(GameObject& go) = 0;
    virtual bool hasObject(const GameObject& go) const = 0;

    virtual void syncTransforms() = 0;
};