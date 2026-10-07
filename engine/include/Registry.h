#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "GameObject.h"

// Minimal ECS storage. A GameObject is just an id (the entity); all component data lives here.
// One pool per component type, keyed by entity id.
//
// Note: do not add/remove components of type T from inside an each<T>() callback.
class Registry
{
    public:
        // Attach a component. A unique_ptr<Derived> is stored under its base type,
        // e.g. add<Collider>(go, factory.createBoxCollider(size)).
        template <class T>
        T& add(const GameObject& go, std::unique_ptr<T> component)
        {
            T& ref = *component;
            pool<T>().items[go.getId()] = std::move(component);
            return ref;
        }

        // Construct a component in place: emplace<Transform>(go, Vec2{1, 2}).
        template <class T, class... Args>
        T& emplace(const GameObject& go, Args&&... args)
        {
            return add<T>(go, std::make_unique<T>(std::forward<Args>(args)...));
        }

        // nullptr if the entity has no such component.
        template <class T>
        T* get(const GameObject& go) const
        {
            const Pool<T>* p = findPool<T>();
            if (p == nullptr)
                return nullptr;
            const auto it = p->items.find(go.getId());
            return it == p->items.end() ? nullptr : it->second.get();
        }

        template <class T>
        bool has(const GameObject& go) const { return get<T>(go) != nullptr; }

        template <class T>
        void remove(const GameObject& go)
        {
            if (Pool<T>* p = findPool<T>())
                p->items.erase(go.getId());
        }

        // Remove every component of the entity.
        void destroy(const GameObject& go)
        {
            for (auto& [type, p] : pools)
                p->erase(go.getId());
        }

        // Calls fn(GameObject, T&) for every entity that has a T.
        template <class T, class Fn>
        void each(Fn&& fn)
        {
            Pool<T>* p = findPool<T>();
            if (p == nullptr)
                return;
            for (auto& [id, component] : p->items)
                fn(GameObject{id}, *component);
        }

    private:
        struct IPool
        {
            virtual ~IPool() = default;
            virtual void erase(int id) = 0;
        };

        template <class T>
        struct Pool : IPool
        {
            std::unordered_map<int, std::unique_ptr<T>> items;
            void erase(int id) override { items.erase(id); }
        };

        template <class T>
        Pool<T>& pool()
        {
            auto& slot = pools[std::type_index(typeid(T))];
            if (!slot)
                slot = std::make_unique<Pool<T>>();
            return static_cast<Pool<T>&>(*slot);
        }

        template <class T>
        Pool<T>* findPool() const
        {
            const auto it = pools.find(std::type_index(typeid(T)));
            return it == pools.end() ? nullptr : static_cast<Pool<T>*>(it->second.get());
        }

        std::unordered_map<std::type_index, std::unique_ptr<IPool>> pools;
};
