cmake_minimum_required(VERSION 3.5...3.16)
project(fastNoiseSIMD)

#set MY_SOURCES to be a list of all the source files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/FastNoiseSIMD/*.cpp")

add_library(fastNoiseSIMD)

target_sources(fastNoiseSIMD PRIVATE ${MY_SOURCES})
target_include_directories(fastNoiseSIMD PUBLIC "FastNoiseSIMD")