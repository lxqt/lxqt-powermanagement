project(lxqt-powermanagement)
# CMP0000 - A minimum required CMake version must be specified.
# Note that the command invocation must appear in the CMakeLists.txt
# file itself; a call in an included file is not sufficient.
cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

find_package(Qt5DBus REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
message(STATUS "Building with Qt${Qt5Core_VERSION}")

find_package(KF5Solid REQUIRED)

find_package(lxqt REQUIRED)

# Patch Version
set(LXQT_POWERMANAGEMENT_PATCH_VERSION 0)

set(LXQT_POWERMANAGEMENT_VERSION ${LXQT_MAJOR_VERSION}.${LXQT_MINOR_VERSION}.${LXQT_POWERMANAGEMENT_PATCH_VERSION})
add_definitions(
    "-DLXQT_POWERMANAGEMENT_VERSION=\"${LXQT_POWERMANAGEMENT_VERSION}\""
    "-DQT_USE_QSTRINGBUILDER"
    "-DQT_NO_CAST_FROM_ASCII"
    "-DQT_NO_CAST_TO_ASCII"
    "-DQT_NO_URL_CAST_FROM_STRING"
    "-DQT_NO_CAST_FROM_BYTEARRAY"
)

include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)

# Translations **********************************
include(LXQtTranslate)

add_subdirectory(config)
add_subdirectory(src)

# merged from lxqt-common
add_subdirectory(autostart)
