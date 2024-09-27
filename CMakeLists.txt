cmake_minimum_required(VERSION 3.29)
project(test)
set(CMAKE_CXX_STANDARD 20)

option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2 -s")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_SCAN_FOR_MODULES 0)
else()
    find_package(SDL2 REQUIRED)
    find_package(OpenGL REQUIRED)
endif()

include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

add_executable(test src/main.cpp
        src/Render/Renderer.h
        src/World/GameObject.h
        src/World/GameWorld.h
        src/World/Component.h
        src/World/GameObject.cpp
        src/World/GameWorld.cpp
        src/World/Component.cpp
        src/Common/Vector2.h
        src/Common/Vector2.cpp
        src/Common/Color.h
        src/Common/Color.cpp
        src/Render/Screen.h
        src/Render/Screen.cpp
        src/Render/Renderer.cpp
        src/Render/OpenGLRenderer.h
        src/Render/OpenGLRenderer.cpp
        src/Input/InputManager.h
        src/Input/InputManager.cpp
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})