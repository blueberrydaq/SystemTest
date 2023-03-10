cmake_minimum_required(VERSION 3.20)
project(SystemTest VERSION 1.0)

find_package(Blueberry)
add_compile_definitions(MODULE_PATH="${BLUEBERRY_MODULES_DIR}")

#locate googletest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(SystemTest Program.cpp)
target_link_libraries(SystemTest PRIVATE bb::blueberry ${GTEST_LIBRARIES} pthread)