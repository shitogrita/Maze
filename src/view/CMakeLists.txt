add_library(view STATIC
        mainwindow.cpp
        mainwindow.h
        mazeview.cpp
        mazeview.h
        caveview.cpp
        caveview.h

)

target_link_libraries(view
        PUBLIC
        Qt6::Widgets
        core
        ml
)

target_include_directories(view
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/core

)