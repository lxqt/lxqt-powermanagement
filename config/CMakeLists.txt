project(lxqt-config-powermanagement)

set ( config-powermanagement_MOCS
      mainwindow.h
      generalsettings.h
      lidsettings.h
      powerlowsettings.h
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
      config-powermanagement.qrc
    )

set (DESKTOP_FILES_IN
     lxqt-config-powermanagement.desktop.in
    )

QT4_ADD_RESOURCES( RSCS ${config-powermanagement_RSCS})

QT4_WRAP_CPP( MOCS ${config-powermanagement_MOCS})

QT4_WRAP_UI( UIS ${config-powermanagement_UIS})

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR}
                    )


add_executable ( ${PROJECT_NAME} 
                 ${config-powermanagement_SRCS} 
                 ${UIS} 
                 ${MOCS} 
                 ${DESKTOP_FILES}
                 ${QM_FILES}
               )

target_link_libraries ( lxqt-config-powermanagement  
                        ${RAZOR_LIBRARY}
                        ${QTXDG_LIBRARY}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY}  
                        ${QT_QTDBUS_LIBRARY}
                      )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)

