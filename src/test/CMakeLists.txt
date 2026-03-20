add_library(test STATIC
        test_maze_generator.h
        test_maze_solver.h
        test_cave.h
        test_maze_qsolver.h
)

target_link_libraries(test PRIVATE core
)