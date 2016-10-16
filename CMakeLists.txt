cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(lxqt-powermanagement)

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

find_package(Qt5DBus REQUIRED)
find_package(Qt5LinguistTools REQUIRED QUIET)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
message(STATUS "Building with Qt${Qt5Core_VERSION_STRING}")

find_package(KF5Solid REQUIRED)

find_package(lxqt REQUIRED)

include(LXQtCompilerSettings NO_POLICY_SCOPE)

# Translations **********************************
include(LXQtTranslate)

add_subdirectory(config)
add_subdirectory(src)
