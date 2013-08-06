project(lz-config-powermanagement)

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_SOURCE_DIR}/libraries
                    )

set ( config-powermanagement_SRCS
      main.cpp
      mainwindow.cpp
      generalsettings.cpp
      lidsettings.cpp
      powerlowsettings.cpp
    )

set ( config-powermanagement_UIS
      generalsettings.ui
      lidsettings.ui
      powerlowsettings.ui  
    )

set ( config-powermanagement_RSCS
        razorautosuspend-config.qrc
    )

QT4_ADD_RESOURCES( RSCS
                   ${config-powermanagement_RSCS}
                 )

set(DESKTOP_FILES_IN
    lz-config-powermanagement.desktop.in
)

set ( config-powermanagement_MOCS
      mainwindow.h
      generalsettings.h
      lidsettings.h
      powerlowsettings.h
    )


set ( ICON_FILES icons/laptop-lid.svg
    )

# Translations **********************************
include(RazorTranslate)
razor_translate_ts(QM_FILES
    SOURCES
        ${config-powermanagement_HDRS}
        ${config-powermanagement_SRCS}
        ${config-powermanagement_UIS}
)

razor_translate_desktop(DESKTOP_FILES
    SOURCES
        ${DESKTOP_FILES_IN}
)
#************************************************

QT4_WRAP_CPP( MOCS ${config-powermanagement_MOCS}
            )

QT4_WRAP_UI( UIS ${config-powermanagement_UIS}
           )

add_executable ( ${PROJECT_NAME} 
                 ${config-powermanagement_SRCS} 
                 ${UIS} 
                 ${MOCS} 
                 ${DESKTOP_FILES}
                 ${QM_FILES}
               )

message(STATUS "QT_QTCORE_LIBRARY: ${QT_QTCORE_LIBRARY}")

target_link_libraries ( lz-config-powermanagement  
                        ${RAZORQT_LIBRARIES}
                        ${RAZORXDG_LIBRARIES}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY}  
                        ${QT_QTDBUS_LIBRARY}
                      )



install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)
install(FILES   ${ICON_FILES}      DESTINATION share/icons/hicolor/scalable/apps)

