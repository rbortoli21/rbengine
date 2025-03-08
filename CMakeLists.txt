cmake_minimum_required(VERSION 3.30)
project(rbengine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(rbengine src/main.cpp
        src/core/Application.cpp
        src/core/Application.h
        src/core/EngineDefines.h
        src/core/Window.cpp
        src/core/Window.h
        src/core/Player.cpp
        src/core/Player.h
        src/core/Game.cpp
        src/core/Game.h
)
target_link_libraries(rbengine ${SDL2_LIBRARIES} SDL2_image)