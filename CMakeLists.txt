set(PROJECT lz-powermanagement)
cmake_minimum_required( VERSION 2.6 )
project(${PROJECT})

message(STATUS INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX})
message(STATUS CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH})

if (NOT BUILD_TOOLS_DIR)
    set (BUILD_TOOLS_DIR ${CMAKE_SOURCE_DIR}/../buildtools)
endif (NOT BUILD_TOOLS_DIR) 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${BUILD_TOOLS_DIR})

find_package(Razor REQUIRED)

set(QT_USE_QTDBUS 1)

find_package(Qt4 REQUIRED)

set(EKSTRA_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
message(STATUS "${EKSTRA_INCLUDE_DIR}")
include_directories(${EKSTRA_INCLUDE_DIR})
message(STATUS "INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
include(${QT_USE_FILE})

add_subdirectory(config)
add_subdirectory(battery)
add_subdirectory(idleness)
add_subdirectory(lid)
