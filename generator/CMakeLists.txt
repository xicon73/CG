cmake_minimum_required(VERSION 2.8)
project(generator)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES generator.c model.c model.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} m)
