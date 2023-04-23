cmake_minimum_required(VERSION 3.5)

project(pathfinder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME}
    PUBLIC
    main.cpp

    PRIVATE
    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/mainwindow.ui
    gui/Scene.h
    gui/Scene.cpp

    core/include/Worker.h
    core/include/Graph.h
    core/include/utils.h
    core/include/Field.h

    core/src/utils.cpp
    core/src/Worker.cpp
    core/src/Field.cpp

    )

target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets)
