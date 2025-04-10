cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project(lxqt-powermanagement)

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

set(LXQTBT_MINIMUM_VERSION "2.2.0")
set(KF6_MINIMUM_VERSION "6.0.0")
set(LXQT_MINIMUM_VERSION "2.2.0")
set(QT_MINIMUM_VERSION "6.6.0")

find_package(lxqt2-build-tools ${LXQTBT_MINIMUM_VERSION} REQUIRED)
find_package(Qt6DBus ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt6LinguistTools ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt6Svg ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt6Widgets ${QT_MINIMUM_VERSION} REQUIRED)
find_package(KF6IdleTime ${KF6_MINIMUM_VERSION} REQUIRED)
find_package(KF6Solid ${KF6_MINIMUM_VERSION} REQUIRED)
find_package(lxqt ${LXQT_MINIMUM_VERSION} REQUIRED)
find_package(lxqt-globalkeys-ui ${LXQT_GLOBALKEYS_MINIMUM_VERSION} REQUIRED)
find_package(XCB REQUIRED COMPONENTS xcb-dpms xcb-screensaver)

message(STATUS "Building with Qt${Qt6Core_VERSION}")

# Patch Version
set(LXQT_POWERMANAGEMENT_PATCH_VERSION 0)

set(LXQT_POWERMANAGEMENT_VERSION ${LXQT_MAJOR_VERSION}.${LXQT_MINOR_VERSION}.${LXQT_POWERMANAGEMENT_PATCH_VERSION})
add_definitions(
    "-DLXQT_POWERMANAGEMENT_VERSION=\"${LXQT_POWERMANAGEMENT_VERSION}\""
)

include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)

# Translations **********************************
include(LXQtTranslate)

add_subdirectory(config)
add_subdirectory(src)
add_subdirectory(autostart)
