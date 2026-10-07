cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
        src/core/Engine.cpp
        src/core/GameObject.cpp
        src/systems/PhysicsSystem.cpp
        src/components/Box2DShape.cpp
        src/components/BoxColliderBox2D.cpp
        src/components/CircleColliderBox2D.cpp
        src/components/Box2DPhysicsWorld.cpp
        src/factories/Box2DColliderFactory.cpp
)
find_package(
        box2d CONFIG REQUIRED
)

target_include_directories(engine PUBLIC
        include
)

target_link_libraries(engine PUBLIC
        project_sanitizers
        project_warnings
        box2d::box2d
)

target_compile_features(engine PUBLIC cxx_std_20)
