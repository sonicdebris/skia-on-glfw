cmake_minimum_required(VERSION 2.8)

project(skia-on-glfw)
add_executable(${PROJECT_NAME} "main.cpp")

add_subdirectory(glfw)
include_directories(glfw/include)

add_library(skia SHARED IMPORTED)
set_target_properties(skia PROPERTIES IMPORTED_LOCATION /home/athos/Thinkering/skia-on-glfw/skia/out/Shared/libskia.so)

target_include_directories(skia INTERFACE
    skia/include/core
    skia/include/config
    skia/include/gpu
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} skia)
