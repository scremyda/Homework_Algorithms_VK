cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(GraphViews)

include_directories(headers)


set(SOURCES
    main.cpp
    ArcGraph.cpp
    ListsGraph.cpp
    MatrixGraph.cpp
    SetGraph.cpp
)


add_executable(GraphViews ${SOURCES})