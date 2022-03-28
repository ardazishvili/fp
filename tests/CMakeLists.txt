cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable( optional optional.cpp)
target_link_libraries( optional gtest_main)
target_include_directories(optional PRIVATE ${CMAKE_SOURCE_DIR})

add_executable( writer writer.cpp)
target_link_libraries( writer gtest_main)
target_include_directories(writer PRIVATE ${CMAKE_SOURCE_DIR})

add_executable( generic generic.cpp)
target_link_libraries( generic gtest_main)
target_include_directories(generic PRIVATE ${CMAKE_SOURCE_DIR})

add_executable( task task.cpp)
target_link_libraries( task gtest_main)
target_include_directories(task PRIVATE ${CMAKE_SOURCE_DIR})

add_executable( task_optional task_optional.cpp)
target_link_libraries( task_optional gtest_main)
target_include_directories(task_optional PRIVATE ${CMAKE_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(optional)
gtest_discover_tests(writer)
gtest_discover_tests(generic)
gtest_discover_tests(task)
gtest_discover_tests(task_optional)
