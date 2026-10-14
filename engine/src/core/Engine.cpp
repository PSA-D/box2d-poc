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
                      << "  pos=(" << std::setw(6) << t.position.x << ", " << std::setw(6) << t.position.y << ")"
                      << "  angle=" << std::setw(6) << t.rotation << " rad\n";
        }
    }

    void Engine::runBox2DExample()
    {
        std::cout << std::endl << "Engine run Box2DExample" << std::endl;
        // Declared first => destroyed last (the physics world reads the registry while shutting down).
        Registry registry;

        // Pick the backend once; everything below only uses abstract interfaces.
        // The only place that has Box2D is in the initialization.
        std::unique_ptr<ColliderFactory> factory = std::make_unique<Box2DColliderFactory>();
        PhysicsSystem physics(registry, std::make_unique<Box2DPhysicsWorld>(registry));

        GameObject ground, ball, crate;

        // Ground: no Rigidbody, so static type
        registry.createAndAdd<Transform>(ground, Vec2{0.0f, -0.5f});
        registry.add<Collider>(ground, factory->createBoxCollider({20.0f, 1.0f}));

        // Ball: dynamic, bouncy.
        registry.createAndAdd<Transform>(ball, Vec2{-1.0f, 6.0f});
        registry.createAndAdd<Rigidbody>(ball);
        auto ballCollider = factory->createCircleCollider(0.5f);
        ballCollider->setRestitution(0.8f); // set before the first update: the Box2D shape is created then
        registry.add<Collider>(ball, std::move(ballCollider));

        // Crate: dynamic, slightly rotated so it tumbles.
        registry.createAndAdd<Transform>(crate, Vec2{1.0f, 4.0f}, 0.3f);
        registry.createAndAdd<Rigidbody>(crate);
        registry.add<Collider>(crate, factory->createBoxCollider({1.0f, 1.0f}));

        std::cout << "Ground: " << ground.getId() << std::endl;
        std::cout << "Ball: " << ball.getId() << std::endl;
        std::cout << "Crate: " << crate.getId() << std::endl;
        std::cout << std::endl;

        // Game loop
        constexpr float frameDt = 1.0f / 30.0f;
        float time = 0.0f;
        for (int frame = 0; frame <= 350; ++frame)
        {
            if (frame % 30 == 0)
            {
                printBody(time, "ball", *registry.get<Transform>(ball));
                printBody(time, "crate", *registry.get<Transform>(crate));
            }
            physics.update(frameDt);
            time += frameDt;
        }

        const Collider& groundCollider = *registry.get<Collider>(ground);
        std::cout << std::boolalpha
                  << "\nball  touching ground? " << registry.get<Collider>(ball)->collidesWith(groundCollider)
                  << "\ncrate touching ground? " << registry.get<Collider>(crate)->collidesWith(groundCollider)
                  << "\nball  touching crate?  " << registry.get<Collider>(ball)->collidesWith(*registry.get<Collider>(crate)) << '\n';

        std::cout << std::endl << "Box2DExample Finished" << std::endl << std::endl;
    }
} // namespace engine
