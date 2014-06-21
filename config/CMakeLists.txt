project(lxqt-config-powermanagement)

if(USE_QT5)
    set(QTX_INCLUDE_DIRS "")
    set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus)
else()
    set(QTX_INCLUDE_DIRS ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTDBUS_INCLUDE_DIR})
    set(QTX_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY})
endif()

set ( config-powermanagement_MOCS
    mainwindow.h
    lidwatchersettings.h
    batterywatchersettings.h
    idlenesswatchersettings.h
    powermanagementsettings.h
)

set ( config-powermanagement_SRCS
    main.cpp
    mainwindow.cpp
    lidwatchersettings.cpp
    batterywatchersettings.cpp
    idlenesswatchersettings.cpp
    powermanagementsettings.cpp
    helpers.cpp
)

set ( config-powermanagement_UIS
    lidwatchersettings.ui
    batterywatchersettings.ui
    idlenesswatchersettings.ui
)

set ( config-powermanagement_RSCS
    config-powermanagement.qrc
)

set ( ICON_FILES
    icons/laptop-lid.svg
)

set (DESKTOP_FILES_IN
    lxqt-config-powermanagement.desktop.in
)

# Translations **********************************
include(LxQtTranslate)

lxqt_translate_desktop(DESKTOP_FILES SOURCES ${DESKTOP_FILES_IN})

#************************************************

if(USE_QT5)
    qt5_add_resources( RSCS ${config-powermanagement_RSCS})
    qt5_wrap_ui( UIS ${config-powermanagement_UIS})
else()
    qt4_add_resources( RSCS ${config-powermanagement_RSCS})
    qt4_wrap_ui( UIS ${config-powermanagement_UIS})
endif()

include_directories ( ${QTX_INCLUDE_DIRS}
                      ${CMAKE_CURRENT_BINARY_DIR}
                    )


add_executable ( ${PROJECT_NAME} 
                 ${config-powermanagement_SRCS} 
                 ${UIS} 
                 ${DESKTOP_FILES}
                 ${QM_FILES}
               )

target_link_libraries ( lxqt-config-powermanagement  
                        ${LXQT_LIBRARIES}
                        ${QTXDG_LIBRARIES}
                        ${QTX_LIBRARIES}
                      )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)
install(FILES ${ICON_FILES} DESTINATION share/icons/hicolor/scalable/devices)
