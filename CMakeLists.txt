cmake_minimum_required(VERSION 3.0)

project(plotdiffusionprocesses)

set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui PrintSupport)
find_package(MuParser REQUIRED)

add_subdirectory(app)
add_subdirectory(qcustomplot)
