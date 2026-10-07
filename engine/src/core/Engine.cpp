#include "Engine.h"

#include <iostream>
#include <box2d/box2d.h>

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
} // namespace engine
