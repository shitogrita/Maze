cmake_minimum_required(VERSION 3.16)

project(Maze)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(GTest REQUIRED)

add_subdirectory(core)
add_subdirectory(view)
add_subdirectory(test)
add_subdirectory(ml)
add_subdirectory(web)

add_executable(Main main.cpp)

target_include_directories(Main PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/view
        ${CMAKE_CURRENT_SOURCE_DIR}/ml
        ${CMAKE_CURRENT_SOURCE_DIR}/web
)

target_link_libraries(Main PRIVATE
        Qt6::Widgets
        core
        view
        ml
)

add_executable(run_test
        run_tests.cpp
)

target_include_directories(run_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/view
        ${CMAKE_CURRENT_SOURCE_DIR}/test
        ${CMAKE_CURRENT_SOURCE_DIR}/ml
        ${CMAKE_CURRENT_SOURCE_DIR}/web
)

target_link_libraries(run_test PRIVATE
        Qt6::Widgets
        core
        view
        ml
        GTest::gtest
        GTest::gtest_main
)