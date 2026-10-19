#include "Engine.h"

#include "../components/Box2DPhysicsWorld.h"
#include "../factories/Box2DColliderFactory.h"
#include "../systems/PhysicsSystem.h"
#include "GameObject.h"
#include "Registry.h"
#include "RigidBody.h"
#include "Transform.h"
#include "abstract/BoxCollider.h"
#include "abstract/CircleCollider.h"

#include <iomanip>
#include <iostream>
#include <memory>

namespace engine
{
    Engine::Engine() = default;

    Engine::~Engine()
    {
        shutdown();
    }

    void Engine::init()
    {
        std::cout << "Engine initialized\n";
        running = true;

        registry = std::make_unique<Registry>();
        physicsSystem = std::make_unique<PhysicsSystem>(*registry, std::make_unique<Box2DPhysicsWorld>(*registry));
        factory = std::make_unique<Box2DColliderFactory>();
    }

    void Engine::shutdown()
    {
        if (!running)
            return;

        running = false;
        std::cout << "Engine shut down\n";
    }

    bool Engine::isRunning() const
    {
        return running;
    }

    namespace
    {
        void printBody(float time, const char* name, const Transform& t)
        {
            std::cout << std::fixed << std::setprecision(2)
                      << "t=" << std::setw(5) << time << "s  " << std::setw(6) << name
                      << "  pos=(" << std::setw(6) << t.getPosition().x << ", " << std::setw(6) << t.getPosition().y << ")"
                      << "  angle=" << std::setw(6) << t.getRotation() << " rad\n";
        }
    }

    void Engine::runBox2DExample()
    {
        std::cout << std::endl << "Engine run Box2DExample" << std::endl;

        GameObject ground;

        // Ground: no Rigidbody, so static type
        registry->createAndAdd<Transform>(ground, Vec2{0.0f, -0.5f});
        auto groundCollider = factory->createBoxCollider({24.0f, 1.0f});
        groundCollider->setFriction(0.8f);
        registry->add<Collider>(ground, std::move(groundCollider));

        // 3 balls with different bounciness (restitution)
        auto createBall = [&](Vec2 position,
                          float restitution) -> GameObject
        {
            GameObject ball;

            registry->createAndAdd<Transform>(ball, position);
            registry->createAndAdd<Rigidbody>(ball);

            auto collider = factory->createCircleCollider(0.5f);
            collider->setRestitution(restitution);

            registry->add<Collider>(ball, std::move(collider));

            return ball;
        };

        GameObject ball1 = createBall({-1.0f, 7.0f}, 0.1f);
        GameObject ball2 = createBall({3.0f, 7.0f}, 0.3f);
        GameObject ball3 = createBall({ 5.0f, 7.0f}, 0.8f);
        GameObject ball4 = createBall({ 5.0f, 7.0f}, 0.8f);

        // Set ball 4 collider inactive
        registry->get<Collider>(ball4)->setActive(false);

        // Crate: dynamic, slightly rotated so it tumbles.
        GameObject crate;
        registry->createAndAdd<Transform>(crate, Vec2{8.0f, 4.0f}, 0.25f);
        registry->createAndAdd<Rigidbody>(crate);

        auto crateCollider = factory->createBoxCollider({1.0f, 1.0f});
        crateCollider->setDensity(2.0f);
        crateCollider->setFriction(0.6f);
        crateCollider->setRestitution(0.0f);
        registry->add<Collider>(crate, std::move(crateCollider));

        // Trigger
        GameObject triggerZone;

        registry->createAndAdd<Transform>(triggerZone, Vec2{3.0f, 1.0f});

        auto triggerCollider = factory->createBoxCollider({3.0f, 1.0f});
        triggerCollider->setTrigger(true);

        registry->add<Collider>(triggerZone, std::move(triggerCollider));

        std::cout << "Ground: " << ground.getId() << std::endl;
        std::cout << "Ball 1: " << ball1.getId() << std::endl;
        std::cout << "Ball 2: " << ball2.getId() << std::endl;
        std::cout << "Ball 3: " << ball3.getId() << std::endl;
        std::cout << "Ball 4: " << ball4.getId() << std::endl;
        std::cout << "Crate: " << crate.getId() << std::endl;
        std::cout << "Trigger: " << triggerZone.getId() << std::endl;
        std::cout << std::endl;

        // Game loop
        constexpr float frameDt = 1.0f / 30.0f;
        float time = 0.0f;
        for (int frame = 0; frame <= 350; ++frame)
        {
            if (frame % 30 == 0)
            {
                printBody(time, "ball 1", *registry->get<Transform>(ball1));
                printBody(time, "ball 2", *registry->get<Transform>(ball2));
                printBody(time, "ball 3", *registry->get<Transform>(ball3));
                printBody(time, "ball 4", *registry->get<Transform>(ball4));
                printBody(time, "crate", *registry->get<Transform>(crate));
                std::cout << std::endl;
            }
            physicsSystem->update(frameDt);
            time += frameDt;
        }

        std::cout << std::endl << "Box2DExample Finished" << std::endl << std::endl;
    }
} // namespace engine
