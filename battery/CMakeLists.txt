project(lxqt-battery)

find_package(LXQT REQUIRED)
include(${LXQT_USE_FILE})

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR}
                    )

set ( lxqt-battery_MOCS
      batterywatcherd.h
      trayicon.h
      battery.h
      batteryinfo.h
    )

set ( lxqt-battery_SRCS
      main.cpp
      trayicon.cpp
      batterywatcherd.cpp
      battery.cpp
      batteryinfo.cpp
    )

set ( lxqt-battery_UIS
      batteryinfo.ui
    )

set (  DESKTOP_FILES_IN
      lxqt-battery.desktop.in
    )

QT4_WRAP_CPP(MOCS ${lxqt-battery_MOCS})

QT4_WRAP_UI( UIS ${lxqt-battery_UIS})

add_executable ( lxqt-battery 
                 ${lxqt-battery_SRCS} 
                 ${UIS} 
                 ${RSCS} 
                 ${QM_FILES} 
                 ${MOCS} 
                 ${DESKTOP_FILES}
               )

target_link_libraries ( lxqt-battery  
                        lxqt
                        ${QTXDG_LIBRARY}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTDBUS_LIBRARY} 
                        ${QT_QTSVG_LIBRARY}
                      )


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES} DESTINATION share/applications)
