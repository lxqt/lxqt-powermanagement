project(lz-battery)

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR}
                    )

set ( lz-battery_MOCS
      batterywatcherd.h
      trayicon.h
      battery.h
      batteryinfo.h
    )

set ( lz-battery_SRCS
      main.cpp
      trayicon.cpp
      batterywatcherd.cpp
      battery.cpp
      batteryinfo.cpp
    )

set ( lz-battery_UIS
      batteryinfo.ui
    )

set (  DESKTOP_FILES_IN
      lz-battery.desktop.in
    )

QT4_WRAP_CPP(MOCS ${lz-battery_MOCS})

QT4_WRAP_UI( UIS ${lz-battery_UIS})

add_executable ( lz-battery 
                 ${lz-battery_SRCS} 
                 ${UIS} 
                 ${RSCS} 
                 ${QM_FILES} 
                 ${MOCS} 
                 ${DESKTOP_FILES}
               )

target_link_libraries ( lz-battery  
                        ${RAZOR_LIBRARY}
                        ${QTXDG_LIBRARY}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTDBUS_LIBRARY} 
                        ${QT_QTSVG_LIBRARY}
                      )


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES} DESTINATION share/applications)
