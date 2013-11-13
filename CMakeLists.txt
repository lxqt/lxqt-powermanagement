set(PROJECT lxqt-powermanagement)
cmake_minimum_required( VERSION 2.6 )
project(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${BUILD_TOOLS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(QTXDG REQUIRED)

set(QT_USE_QTDBUS 1)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

add_subdirectory(config)
add_subdirectory(battery)
add_subdirectory(idleness)
add_subdirectory(lid)
