cmake_minimum_required(VERSION 3.30)
project(rbengine)

set(CMAKE_CXX_STANDARD 20)

add_executable(rbengine src/main.cpp
        src/core/Application.cpp
        src/core/Application.h
        src/core/ApplicationInterface.h
        src/core/EngineDefines.h
        src/sample/ApplicationEntrypoint.cpp
        src/sample/ApplicationEntrypoint.h
)
