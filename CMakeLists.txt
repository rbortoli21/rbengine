cmake_minimum_required(VERSION 3.30)
project(rbengine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(rbengine src/main.cpp
        src/core/EngineDefines.h
        src/core/Player.cpp
        src/core/Player.h
        src/core/Game.cpp
        src/core/Game.h
        src/core/Entity.h
        src/core/Component.h
        src/core/Constants.h
        src/core/Camera.cpp
        src/core/Camera.h
        src/core/GameObject.h
        src/core/InputManager.cpp
        src/core/InputManager.h
        src/core/TextureManager.cpp
        src/core/TextureManager.h
        src/core/Vector2D.h
        src/core/Arrow.cpp
        src/core/Arrow.h
        src/core/Menu.cpp
        src/core/Menu.h
)
target_link_libraries(rbengine ${SDL2_LIBRARIES} SDL2_ttf SDL2_image)