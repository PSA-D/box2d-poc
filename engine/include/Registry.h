#pragma once

#include "GameObject.h"
#include "Types.h"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

// Minimal ECS storage. All component data lives here
// One pool per component type, keyed by entity id
class Registry
{
    public:
        // Attach a component to a game object
        template <class T>
        T& add(const GameObject& go, std::unique_ptr<T> component)
        {
            T& ref = *component;

            ComponentPool<T>& pool = getOrCreatePool<T>();
            pool.items[go.getId()] = std::move(component); // Attach component to game object

            return ref;
        }

        // Construct a component in place and adds it
        template <class T, class... Args>
        T& createAndAdd(const GameObject& go, Args&&... args)
        {
            std::unique_ptr<T> component = std::make_unique<T>(std::forward<Args>(args)...); // Perfect forwarding with the arguments given
            return add<T>(go, std::move(component));
        }

        template <class T>
        T* get(const GameObject& go) const
        {
            const ComponentPool<T>* pool = findPool<T>();
            if (pool == nullptr) return nullptr;

            const auto found = pool->items.find(go.getId());

            return found == pool->items.end() ? nullptr : found->second.get();
        }

        template <class T>
        bool has(const GameObject& go) const
        {
            return get<T>(go) != nullptr;
        }

        template <class T>
        void remove(const GameObject& go)
        {
            if (ComponentPool<T>* pool = findPool<T>())
                pool->items.erase(go.getId());
        }

        // Remove every component of the entity
        void removeAll(const GameObject& go) const
        {
            for (const auto& entry : pools)
                entry.second->erase(go.getId()); // Erase from pool / Decouple from game object
        }

        // Calls the function for every entity that has a type T. Function must have these parameters: GameObject, Component
        template <class T, class Function>
        void each(Function&& func)
        {
            ComponentPool<T>* pool = findPool<T>();
            if (pool == nullptr) return;

            for (auto& [id, component] : pool->items)
                func(GameObject{id}, *component);
        }

    private:
        template <class T>
        ComponentPool<T>& getOrCreatePool()
        {
            // std::type_index(typeid(T)) is a unique label for the type T, like "Transform" or "Rigidbody"
            std::unique_ptr<IPool>& slot = pools[std::type_index(typeid(T))]; // if nullptr, inserts a new empty one
            if (!slot)
                slot = std::make_unique<ComponentPool<T>>();

            return static_cast<ComponentPool<T>&>(*slot); // Cast to the right type
        }

        template <class T>
        ComponentPool<T>* findPool() const
        {
            const auto found = pools.find(std::type_index(typeid(T)));

            return found == pools.end() ? nullptr : static_cast<ComponentPool<T>*>(found->second.get()); // Cast to the right type
        }

        std::unordered_map<std::type_index, std::unique_ptr<IPool>> pools; // type_index is just the object type as an index
};
