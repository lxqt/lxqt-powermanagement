set(PROJECT lxqt-powermanagement)
cmake_minimum_required( VERSION 2.6 )
project(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${BUILD_TOOLS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(QTXDG REQUIRED)

set(QT_USE_QTDBUS 1)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

add_subdirectory(config)
add_subdirectory(src)


# building tarball with CPack -------------------------------------------------
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_VERSION_MAJOR ${LXQT_MAJOR_VERSION})
set (CPACK_PACKAGE_VERSION_MINOR ${LXQT_MINOR_VERSION})
set (CPACK_PACKAGE_VERSION_PATCH ${LXQT_PATCH_VERSION})
set (CPACK_GENERATOR TBZ2)
set (CPACK_SOURCE_GENERATOR TBZ2)
set (CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
include (CPack)
