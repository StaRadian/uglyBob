cmake_minimum_required(VERSION 3.20)

project("uglyBob" C CXX ASM)

include(cmake/project.cmake)

add_executable(${PROJECT_NAME})
add_target_properties(${PROJECT_NAME})