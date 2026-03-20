find_package(Crow REQUIRED)

add_executable(WebMaze
        server.cpp
)

target_link_libraries(WebMaze
        PRIVATE
        Crow::Crow
        core
)

target_include_directories(WebMaze
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../core
)