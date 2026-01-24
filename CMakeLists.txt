cmake_minimum_required(VERSION 4.0)
project(opengl_learn LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# Third-party
add_subdirectory(external/glfw)

add_library(glad STATIC
    external/glad/src/glad.c
)
target_include_directories(glad
    PUBLIC
        ${CMAKE_SOURCE_DIR}/external/glad/include
)

add_library(glm INTERFACE)

target_include_directories(glm
    INTERFACE
        ${CMAKE_SOURCE_DIR}/external/glm
)

find_package(OpenGL REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Project modules
add_subdirectory(engine)
add_subdirectory(app)