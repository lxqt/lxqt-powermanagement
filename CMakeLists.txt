cmake_minimum_required(VERSION 2.8.11)

project(lxqt-powermanagement)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5LinguistTools REQUIRED QUIET)
find_package(lxqt REQUIRED QUIET)
find_package(Qt5Xdg REQUIRED)
message(STATUS "Building with Qt${Qt5Core_VERSION_STRING}")

include(${LXQT_USE_FILE})
include(${QTXDG_USE_FILE})

include(LXQtTranslate)

add_subdirectory(config)
add_subdirectory(src)

# building tarball with CPack -------------------------------------------------
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR ${LXQT_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${LXQT_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${LXQT_PATCH_VERSION})
set(CPACK_GENERATOR TBZ2)
set(CPACK_SOURCE_GENERATOR TBZ2)
set(CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
include(CPack)
