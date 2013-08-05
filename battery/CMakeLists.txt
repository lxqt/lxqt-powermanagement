project(lz-battery)

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_SOURCE_DIR}/libraries/
                    )

set ( lz-battery_HDRS
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

set ( lz-battery_RSCS
        )
QT4_ADD_RESOURCES(RSCS ${lz-battery_RSCS})

set ( lz-battery_MOCS
      batterywatcherd.h
      trayicon.h
      battery.h
      batteryinfo.h
    )

# TODO: remove actual file and its translations
set(DESKTOP_FILES_IN
#    lz-battery.desktop.in
)

set(CONFIG_FILES
    lz-battery.conf
    )

QT4_WRAP_CPP(MOCS ${lz-battery_MOCS})

QT4_WRAP_UI( UIS ${lz-battery_UIS}
           )


set ( ICON_FILES icons/lz-battery.svg
    )

# Translations **********************************
include(RazorTranslate)
razor_translate_ts(QM_FILES
    SOURCES
        ${lz-battery_HDRS}
        ${lz-battery_SRCS}
        ${lz-battery_UIS}
)

razor_translate_desktop(DESKTOP_FILES
    SOURCES
        ${DESKTOP_FILES_IN}
)
#************************************************

add_executable ( lz-battery ${lz-battery_SRCS} ${UIS} ${RSCS} ${QM_FILES} ${MOCS} ${DESKTOP_FILES})
target_link_libraries ( lz-battery  
                        ${RAZORQT_LIBRARIES}
                        ${RAZORXDG_LIBRARIES}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTDBUS_LIBRARY} 
                        ${QT_QTSVG_LIBRARY})


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)
install(FILES   ${ICON_FILES}      DESTINATION share/icons/hicolor/scalable/apps)
install(FILES   ${CONFIG_FILES}    DESTINATION ${RAZOR_ETC_XDG_DIRECTORY}/razor)
