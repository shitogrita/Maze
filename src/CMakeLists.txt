cmake_minimum_required(VERSION 3.16)

project(Maze)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_subdirectory(core)
add_subdirectory(view)

add_executable(Main main.cpp)

target_link_libraries(Main
        PRIVATE
        Qt6::Widgets
        core
        view
)

target_include_directories(Main
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/view
)