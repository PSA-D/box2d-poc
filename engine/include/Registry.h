#pragma once

#include "GameObject.h"
#include "Types.h"
#include "abstract/Component.h"

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
        template<typename Component>
        Component& add(const GameObject& go, std::unique_ptr<Component> component)
        {
            Component& ref = *component;

            ComponentPool<Component>& pool = getOrCreatePool<Component>();
            pool.items[go.getId()] = std::move(component); // Attach component to game object

            return ref;
        }

        // Construct a component in place and adds it
        template <typename Component, class... Args>
        Component& createAndAdd(const GameObject& go, Args&&... args)
        {
            std::unique_ptr<Component> component = std::make_unique<Component>(std::forward<Args>(args)...); // Perfect forwarding with the arguments given
            return add<Component>(go, std::move(component));
        }

        template<typename Component>
        Component* get(const GameObject& go) const
        {
            const ComponentPool<Component>* pool = findPool<Component>();
            if (pool == nullptr) return nullptr;

            const auto found = pool->items.find(go.getId());

            return found == pool->items.end() ? nullptr : found->second.get();
        }

        template<typename Component>
        bool has(const GameObject& go) const
        {
            return get<Component>(go) != nullptr;
        }

        template<typename Component>
        void remove(const GameObject& go)
        {
            if (ComponentPool<Component>* pool = findPool<Component>())
                pool->items.erase(go.getId());
        }

        // Remove every component of the entity
        void removeAll(const GameObject& go) const
        {
            for (const auto& entry : pools)
                entry.second->erase(go.getId()); // Erase from pool / Decouple from game object
        }

        // Calls the function for every entity that has a type T. Function must have these parameters: GameObject, Component
        template <typename Component, class Function>
        void each(Function&& func)
        {
            ComponentPool<Component>* pool = findPool<Component>();
            if (pool == nullptr) return;

            for (auto& [id, component] : pool->items)
                func(GameObject{id}, *component);
        }

    private:
        template<typename Component>
        ComponentPool<Component>& getOrCreatePool()
        {
            // std::type_index(typeid(T)) is a unique label for the type T, like "Transform" or "Rigidbody"
            std::unique_ptr<IPool>& slot = pools[std::type_index(typeid(Component))]; // if nullptr, inserts a new empty one
            if (!slot)
                slot = std::make_unique<ComponentPool<Component>>();

            return dynamic_cast<ComponentPool<Component>&>(*slot); // Cast to the right type
        }

        template<typename Component>
        ComponentPool<Component>* findPool() const
        {
            const auto found = pools.find(std::type_index(typeid(Component)));

            return found == pools.end() ? nullptr : dynamic_cast<ComponentPool<Component>*>(found->second.get()); // Cast to the right type
        }

        std::unordered_map<std::type_index, std::unique_ptr<IPool>> pools; // type_index is just the object type as an index
};
