add_executable(main)

target_sources(main
    PRIVATE
        src/main.cpp
        src/cube.cpp
)

target_include_directories(main
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(main
    PRIVATE
        engine
        glfw
)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)