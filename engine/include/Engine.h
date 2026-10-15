#pragma once

#include <memory>

class ColliderFactory;
class PhysicsSystem;
class Registry;

namespace engine
{
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    auto operator=(const Engine&) -> Engine& = delete;
    Engine(Engine&&) noexcept = default;
    auto operator=(Engine&&) noexcept -> Engine& = default;

    void init();
    void shutdown();
    [[nodiscard]] bool isRunning() const;

    // Runs the Box2D ECS example
    void runBox2DExample();
private:
    bool running{false};

    std::unique_ptr<Registry> registry;
    std::unique_ptr<PhysicsSystem> physicsSystem;
    std::unique_ptr<ColliderFactory> factory;
};
} // namespace engine
