cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
project(FP)

add_subdirectory(tests)

file(GLOB_RECURSE SOURCE_FILES test.cpp)
file(GLOB_RECURSE HEADER_FILES *.hpp )
add_executable(fp ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(fp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(fp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
