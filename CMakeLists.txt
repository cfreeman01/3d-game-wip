cmake_minimum_required(VERSION 3.2)
project(Game VERSION 1.0)

find_package(OpenGL REQUIRED)

#copy resources to build directory
file(COPY shaders textures audio models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}
               src/audioPlayer.h
               src/boss.cpp
               src/boss.h
               src/camera.cpp
               src/camera.h
               src/character.cpp
               src/character.h
               src/enemy.cpp
               src/enemy.h
               src/enemy1.cpp
               src/enemy1.h
               src/enemy2.cpp
               src/enemy2.h
               src/enemy3.cpp
               src/enemy3.h
               src/game.cpp
               src/game.h
               src/gameObject.cpp
               src/gameObject.h
               src/glad.c
               src/hud.cpp
               src/hud.h
               src/level.cpp
               src/level.h
               src/main.cpp
               src/player.cpp
               src/player.h
               src/resource_manager.cpp
               src/resource_manager.h
               src/shader.cpp
               src/shader.h
               src/skybox.cpp
               src/skybox.h
               src/spriteRenderer.cpp
               src/spriteRenderer.h
               src/texture.cpp
               src/texture.h
               src/trailGenerator.cpp
               src/trailGenerator.h
               src/VoxelLoader.cpp
               src/VoxelLoader.h
               src/VoxelModel.cpp
               src/VoxelModel.h
               src/VoxelRenderer.cpp
               src/VoxelRenderer.h)

include_directories(${PROJECT_NAME} PUBLIC includes)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.4/)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
