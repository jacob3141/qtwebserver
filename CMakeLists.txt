cmake_minimum_required(VERSION 3.6.0)

project(qtwebserver
    VERSION 1.0.0
    DESCRIPTION "Glacier applications wrapper library")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)

option(USE_QTGUI "Use QtGui" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

set(QT_MIN_VERSION "6.2.0")
set(CMAKE_INSTALL_PREFIX /usr)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED
    Core
    Network
    Sql
    Xml)

add_subdirectory(src)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(USE_QTGUI)
    add_definitions("-DUSE_QTGUI")
    find_package(Qt6 ${QT_MIN_VERSION} REQUIRED Gui)
endif()
