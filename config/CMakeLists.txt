set(QTX_LIBRARIES Qt6::Widgets Qt6::DBus Qt6::Svg)

set(SOURCES
    main.cpp
    mainwindow.cpp
    lidwatchersettings.cpp
    batterywatchersettings.cpp
    idlenesswatchersettings.cpp
    powermanagementsettings.cpp
    powerkeyssettings.cpp
    helpers.cpp
    ../src/iconproducer.cpp
)

set(UI_FILES
    lidwatchersettings.ui
    batterywatchersettings.ui
    idlenesswatchersettings.ui
    powerkeyssettings.ui
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
        ${SOURCES}
        ${UI_FILES}
    TEMPLATE
        lxqt-config-powermanagement
    INSTALL_DIR
        "${LXQT_TRANSLATIONS_DIR}/lxqt-config-powermanagement"
)

lxqt_app_translation_loader(config-powermanagement_QM_LOADER lxqt-config-powermanagement)
lxqt_translate_desktop(DESKTOP_FILES SOURCES ${DESKTOP_FILES_IN} USE_YAML)

add_executable(lxqt-config-powermanagement
    ${SOURCES}
    ${DESKTOP_FILES}
    ${QM_FILES}
    ${config-powermanagement_QM_LOADER}
)

target_link_libraries(lxqt-config-powermanagement
    lxqt
    ${QTX_LIBRARIES}
    KF6::Solid
)

install(TARGETS
    lxqt-config-powermanagement
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)
install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
    COMPONENT Runtime
)
install(FILES
    ${ICON_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/devices"
    COMPONENT Runtime
)
