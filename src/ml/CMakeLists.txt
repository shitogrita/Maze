add_library(ml STATIC
        MazeQSolver.cpp
)

target_link_libraries(ml PUBLIC core)
target_include_directories(ml
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)