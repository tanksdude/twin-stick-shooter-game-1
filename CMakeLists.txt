cmake_minimum_required(VERSION 3.16)
project(twin-stick-shooter-game-1 VERSION 1.0)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	#nothing
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	#nothing
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	message(SEND_ERROR "You are compiling with MSVC. CMake & MSVC compilation is unsupported, use the .sln project instead.\n")
else()
	message(WARNING "Unknown compiler, assuming it's fine...")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED False)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(stb      PRIVATE Dependencies/stb)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

add_executable(${PROJECT_NAME}
	twin-stick-shooter-game-1/circle.cpp
	twin-stick-shooter-game-1/collision-handler.cpp
	twin-stick-shooter-game-1/game-main-loop.cpp
	twin-stick-shooter-game-1/game-scene-manager.cpp
	twin-stick-shooter-game-1/game-scene.cpp
	twin-stick-shooter-game-1/enemy.cpp
	twin-stick-shooter-game-1/player.cpp
	twin-stick-shooter-game-1/powerup.cpp
	twin-stick-shooter-game-1/projectile.cpp
	twin-stick-shooter-game-1/rect.cpp
	twin-stick-shooter-game-1/rng.cpp
	twin-stick-shooter-game-1/simple-vector-2d.cpp
	twin-stick-shooter-game-1/sprite.cpp
	twin-stick-shooter-game-1/texture-manager.cpp
	twin-stick-shooter-game-1/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE GL)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::glew)
target_link_libraries(${PROJECT_NAME} PRIVATE GLUT::GLUT)
