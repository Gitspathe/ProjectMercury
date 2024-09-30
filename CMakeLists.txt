cmake_minimum_required(VERSION 3.29)
project(test)
set(CMAKE_CXX_STANDARD 20)

option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_SCAN_FOR_MODULES 0)
endif()

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

add_executable(test lib/json.hpp
        src/main.cpp
        src/Engine/Render/Renderer.h
        src/Engine/World/GameObject.h
        src/Engine/World/GameWorld.h
        src/Engine/World/Component.h
        src/Engine/World/GameWorld.cpp
        src/Engine/Common/Vector2.h
        src/Engine/Common/ColorRGBA.h
        src/Engine/Render/Screen.h
        src/Engine/Render/OpenGLRenderer.h
        src/Engine/Input/InputManager.h
        src/Engine/Input/InputManager.cpp
        src/Engine/Common/ColorRGB.h
        src/Engine/Common/RectF.h
        src/Engine/Render/Surface.h
        src/Engine/Render/Primitives/RectGraphic.h
        src/Engine/Common/ColorRGB.cpp
        src/Engine/Common/ColorRGBA.cpp
        lib/json.hpp
        src/Engine/World/TransformComponent.h
        src/Engine/World/TestGameObject.h
        src/Engine/World/GameObject.cpp
        src/Engine/Common/Transform.h
        src/Engine/Common/Vector2.cpp
        src/Engine/World/Component.cpp
        src/Engine/Common/ColorConversion.h
        src/Engine/Common/Colors.h
        src/Engine/Core/Game.h
        src/Game/TestGame.h
        src/Engine/Common/BaseColor.h
        src/Engine/Common/ColorConstants.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    if(JS_ONLY)
        message(STATUS "Setting compilation target to native JavaScript")
        set_target_properties(test PROPERTIES LINK_FLAGS "-s WASM=0 -s EXPORTED_FUNCTIONS='[_main]'")
    else(JS_ONLY)
        message(STATUS "Setting compilation target to WASM")
        set_target_properties(test PROPERTIES LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='[_main]'")
    endif(JS_ONLY)
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fopenmp")