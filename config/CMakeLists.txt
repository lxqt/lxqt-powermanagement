project(lxqt-config-powermanagement)

find_package(LXQT REQUIRED)
include(${LXQT_USE_FILE})

set ( config-powermanagement_MOCS
      mainwindow.h
      lidsettings.h
      powerlowsettings.h
      idlesettings.h
    )

set ( config-powermanagement_SRCS
      main.cpp
      mainwindow.cpp
      lidsettings.cpp
      powerlowsettings.cpp
      idlesettings.cpp
      common.cpp
    )

set ( config-powermanagement_UIS
      lidsettings.ui
      powerlowsettings.ui
      idlesettings.ui
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
                        ${LXQT_LIBRARIES}
                        ${QTXDG_LIBRARIES}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY}  
                        ${QT_QTDBUS_LIBRARY}
                      )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)

