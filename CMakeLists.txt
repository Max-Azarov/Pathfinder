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
    gui/MyGraphicsView.h
    gui/control/FieldCreatorAndPathFinder.h
    gui/control/Manager.h
    gui/control/Manager.cpp
    gui/control/VectorHolder.h

    gui/sceneItems/AbstractCell.h
    gui/sceneItems/AbstractWayPoint.h
    gui/sceneItems/AWayPoint.h
    gui/sceneItems/AWayPoint.cpp
    gui/sceneItems/BWayPoint.h
    gui/sceneItems/BWayPoint.cpp
    gui/sceneItems/MovableCell.h
    gui/sceneItems/MovableCell.cpp
    gui/sceneItems/PathLine.h
    gui/sceneItems/PathLine.cpp
    gui/sceneItems/WallCell.h
    gui/sceneItems/WallCell.cpp

    core/include/Worker.h
    core/include/Graph.h
    core/include/utils.h
    core/include/Field.h
    core/src/utils.cpp
    core/src/Worker.cpp
    core/src/Field.cpp
    )

target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets)
