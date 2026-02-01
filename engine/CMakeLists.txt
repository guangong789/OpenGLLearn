project(engine LANGUAGES CXX)

add_library(engine STATIC)

target_sources(engine
    PRIVATE
        src/core/Config/global.cpp
        src/core/Utils/stb_image.cpp
        src/core/Utils/uti.cpp
        src/core/camera.cpp

        src/graphics/mesh.cpp

        src/render/material.cpp
        src/render/shader.cpp
        src/render/GeometryPass.cpp
        src/render/OutlinePass.cpp
        
        src/resource/model.cpp
        src/resource/texture.cpp

        src/scene/Lighting/light.cpp
        src/scene/Lighting/LightManager.cpp
        src/scene/renderable.cpp
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(engine
    PUBLIC
        glm::glm
        glad
    PRIVATE
        glfw
        OpenGL::GL
        assimp::assimp
)

target_compile_features(engine PUBLIC cxx_std_23)

target_compile_options(engine
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_compile_definitions(engine
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)
