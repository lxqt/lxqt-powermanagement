project(lxqt-config-powermanagement)

set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus Qt5::Svg)

set(config-powermanagement_MOCS
    mainwindow.h
    lidwatchersettings.h
    batterywatchersettings.h
    idlenesswatchersettings.h
    powermanagementsettings.h
)

set(config-powermanagement_SRCS
    main.cpp
    mainwindow.cpp
    lidwatchersettings.cpp
    batterywatchersettings.cpp
    idlenesswatchersettings.cpp
    powermanagementsettings.cpp
    helpers.cpp
    ../src/iconproducer.cpp
    ../src/battery.cpp
)

set(config-powermanagement_UIS
    lidwatchersettings.ui
    batterywatchersettings.ui
    idlenesswatchersettings.ui
)

set(config-powermanagement_RSCS
    config-powermanagement.qrc
)

set(ICON_FILES
    icons/laptop-lid.svg
)

set(DESKTOP_FILES_IN
    lxqt-config-powermanagement.desktop.in
)

# Translations **********************************

lxqt_translate_ts(QM_FILES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    SOURCES
        ${config-powermanagement_SRCS}
        ${config-powermanagement_UIS}
        ${config-powermanagement_MOCS}
    INSTALL_DIR
        ${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}
)

lxqt_app_translation_loader(config-powermanagement_QM_LOADER ${PROJECT_NAME})
lxqt_translate_desktop(DESKTOP_FILES SOURCES ${DESKTOP_FILES_IN})


qt5_add_resources(RSCS ${config-powermanagement_RSCS})
qt5_wrap_ui(UIS ${config-powermanagement_UIS})

add_executable(${PROJECT_NAME}
    ${config-powermanagement_SRCS}
    ${UIS}
    ${DESKTOP_FILES}
    ${QM_FILES}
    ${config-powermanagement_QM_LOADER}
)

target_link_libraries(lxqt-config-powermanagement
    ${LXQT_LIBRARIES}
    ${QTXDG_LIBRARIES}
    ${QTX_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${DESKTOP_FILES} DESTINATION share/applications)
install(FILES ${ICON_FILES} DESTINATION share/icons/hicolor/scalable/devices)
