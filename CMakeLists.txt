cmake_minimum_required(VERSION 3.2)
project(Game VERSION 1.0)

find_package(OpenGL REQUIRED)

#copy resources to build directory
file(COPY shaders textures audio DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}
               src/audioPlayer.h
               src/game.cpp
               src/game.h
               src/glad.c
               src/main.cpp
               src/resource_manager.cpp
               src/resource_manager.h
               src/shader.cpp
               src/shader.h
               src/texture.cpp
               src/texture.h
               src/VoxelLoader.cpp
               src/VoxelLoader.h
               src/VoxelModel.cpp
               src/VoxelModel.h
               src/VoxelRenderer.cpp
               src/VoxelRenderer.h)

include_directories(${PROJECT_NAME} PUBLIC includes)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.4/)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
