add_library(Tests STATIC
        test_maze_generator.h
        test_maze_solver.h
)
target_link_libraries(Tests PRIVATE core
)