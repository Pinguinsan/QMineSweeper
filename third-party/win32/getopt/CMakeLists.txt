cmake_minimum_required(VERSION 3.1)
project(getopt-win32 C)

set (SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set (${PROJECT_NAME}_SOURCE_FILES "${SOURCE_ROOT}/getopt.c")
set (${PROJECT_NAME}_HEADER_FILES "${SOURCE_ROOT}/getopt.h")

add_library(${PROJECT_NAME}
        STATIC
        ${${PROJECT_NAME}_SOURCE_FILES}
        ${${PROJECT_NAME}_HEADER_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")