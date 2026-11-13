cmake_minimum_required(VERSION 3.20)

project(ImageEngine LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# GLAD
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# Our application
add_executable(image-engine
    main.cpp
)

target_compile_definitions(image-engine PRIVATE GLFW_INCLUDE_NONE)

target_link_libraries(image-engine PRIVATE
    glfw
    glad
    OpenGL::GL
)
