#include "Engine.h"

#include "../components/Box2DPhysicsWorld.h"
#include "../factories/Box2DColliderFactory.h"
#include "GameObject.h"
#include "abstract/BoxCollider.h"
#include "abstract/CircleCollider.h"
#include "../systems/PhysicsSystem.h"
#include "Registry.h"
#include "RigidBody.h"

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
        std::unique_ptr<ColliderFactory> factory = std::make_unique<Box2DColliderFactory>();
        PhysicsSystem physics(registry, std::make_unique<Box2DPhysicsWorld>(registry));

        // Entities are just ids; their data lives in the registry.
        GameObject ground, ball, crate;

        // Ground: no Rigidbody => static.
        registry.emplace<Transform>(ground, Vec2{0.0f, -0.5f});
        registry.add<Collider>(ground, factory->createBoxCollider({20.0f, 1.0f}));

        // Ball: dynamic, bouncy.
        registry.emplace<Transform>(ball, Vec2{-1.0f, 6.0f});
        registry.emplace<Rigidbody>(ball);
        auto ballCollider = factory->createCircleCollider(0.5f);
        ballCollider->setRestitution(0.6f); // set before the first update: the Box2D shape is created then
        registry.add<Collider>(ball, std::move(ballCollider));

        // Crate: dynamic, slightly rotated so it tumbles.
        registry.emplace<Transform>(crate, Vec2{1.0f, 4.0f}, 0.3f);
        registry.emplace<Rigidbody>(crate);
        registry.add<Collider>(crate, factory->createBoxCollider({1.0f, 1.0f}));

        // Pretend we render at 30 FPS; the system still steps Box2D at a fixed 60 Hz.
        constexpr float frameDt = 1.0f / 30.0f;
        float time = 0.0f;
        for (int frame = 0; frame <= 300; ++frame)
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
