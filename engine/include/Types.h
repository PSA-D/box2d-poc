#pragma once

#include "GameObject.h"

#include <memory>
#include <unordered_map>

struct Vec2 {
    float x, y;
};

struct IPool
{
    virtual ~IPool() = default;
    virtual void erase(int id) = 0;
};

template <class T>
struct ComponentPool : IPool // Object pool for all components
{
    std::unordered_map<int, std::unique_ptr<T>> items;
    void erase(int id) override { items.erase(id); }
};

struct CollisionEvent
{
    GameObject one; // For trigger: sensor
    GameObject two; // For trigger: visitor
    bool isTrigger {false};
};

enum class CollisionType
{
    Begin,
    Stay, // Not Implemented yet
    End,
}; // Can be extended with Hit events. Hit events are brutal touches. Example Hit event: ball slamming in wall.