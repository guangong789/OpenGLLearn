cmake_minimum_required(VERSION 4.0)
project(OPENGL_LEARN LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== Build type =====
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-O3)
    endif()
endif()

# ===== Include =====
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/glm
    ${PROJECT_SOURCE_DIR}/glad/include
)

# ===== GLAD =====
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# ===== GLFW =====
add_subdirectory(glfw)

# ===== OpenGL =====
find_package(OpenGL REQUIRED)

# ===== Resource Path =====
file(COPY ${PROJECT_SOURCE_DIR}/docs DESTINATION ${CMAKE_BINARY_DIR})

# ===== Main =====
add_executable(main
    src/main.cpp
    src/GRAPHICS/shader.cpp
    src/uti.cpp
    src/read_obj.cpp
    src/camera.cpp
    src/CUBE/cube.cpp
    src/CUBE/light.cpp
    src/global.cpp
    src/stb_image.cpp
    src/CORE/mesh.cpp
    src/CORE/material.cpp
    src/CORE/renderable.cpp
    src/MANAGER/LightManager.cpp
)

if (WIN32 AND MINGW)
    option(STATIC_MINGW_RUNTIME "Static link MinGW runtime" ON)
    if (STATIC_MINGW_RUNTIME)
        target_link_options(main PRIVATE
            -static-libstdc++
            -static-libgcc
        )
    endif()
endif()

target_link_libraries(main
    PRIVATE
        glad
        glfw
        OpenGL::GL
)

# ===== Linux only =====
if (UNIX AND NOT APPLE)
    target_link_libraries(main PRIVATE dl pthread)
endif()