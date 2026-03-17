add_library(core STATIC
        maze.cpp
        maze.h
)

target_include_directories(core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)