cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
    src/core/Engine.cpp
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
