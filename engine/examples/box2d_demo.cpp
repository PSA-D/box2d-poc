// Box2D demo: a ball and a crate fall onto a static ground box.
// Shows the whole chain:  Factory -> GameObject -> PhysicsSystem -> IPhysicsWorld (Box2D adapter)

#include <iomanip>
#include <iostream>
#include <memory>

#include "../include/abstract/BoxCollider.h"
#include "../include/abstract/CircleCollider.h"
#include "components/Box2DPhysicsWorld.h"
#include "core/GameObject.h"
#include "factories/Box2DColliderFactory.h"
#include "systems/PhysicsSystem.h"

namespace
{
    void print(float time, const char* name, const GameObject& go)
    {
        std::cout << std::fixed << std::setprecision(2)
                  << "t=" << std::setw(5) << time << "s  "
                  << std::setw(6) << name
                  << "  pos=(" << std::setw(6) << go.position.x << ", " << std::setw(6) << go.position.y << ")"
                  << "  angle=" << std::setw(6) << go.rotation << " rad\n";
    }
}

int main()
{
    // 1. Pick the backend ONCE. Everything below only talks to abstract interfaces.
    std::unique_ptr<ColliderFactory> factory = std::make_unique<Box2DColliderFactory>();
    PhysicsSystem physics(std::make_unique<Box2DPhysicsWorld>(Vec2{0.0f, -10.0f}));

    // 2. Ground: 20 x 1 box, no Rigidbody => static.
    GameObject ground;
    ground.position = {0.0f, -0.5f};
    ground.setCollider(factory->createBoxCollider({20.0f, 1.0f}));

    // 3. Ball: dynamic circle, bouncy.
    GameObject ball;
    ball.position = {-1.0f, 6.0f};
    ball.setRigidbody(std::make_unique<Rigidbody>());
    auto ballCollider = factory->createCircleCollider(0.5f);
    ballCollider->setRestitution(0.6f); // set material BEFORE addObject: the Box2D shape is created there
    ball.setCollider(std::move(ballCollider));

    // 4. Crate: dynamic 1x1 box, slightly rotated so it tumbles.
    GameObject crate;
    crate.position = {1.0f, 4.0f};
    crate.rotation = 0.3f;
    crate.setRigidbody(std::make_unique<Rigidbody>());
    crate.setCollider(factory->createBoxCollider({1.0f, 1.0f}));

    physics.addObject(ground);
    physics.addObject(ball);
    physics.addObject(crate);

    // 5. Main loop. We pretend the game renders at 30 FPS; PhysicsSystem still steps Box2D at a
    //    fixed 60 Hz, so it runs ~2 physics steps per frame.
    constexpr float frameDt = 1.0f / 30.0f;
    float time = 0.0f;
    for (int frame = 0; frame <= 300; ++frame)
    {
        if (frame % 30 == 0)
        {
            print(time, "ball", ball);
            print(time, "crate", crate);
        }
        physics.update(frameDt);
        time += frameDt;
    }

    std::cout << std::boolalpha
              << "\nball  touching ground? " << ball.getCollider()->collidesWith(*ground.getCollider())
              << "\ncrate touching ground? " << crate.getCollider()->collidesWith(*ground.getCollider())
              << "\nball  touching crate?  " << ball.getCollider()->collidesWith(*crate.getCollider()) << '\n';
    return 0;
}
