project(lxqt-powermanagement)

set(QTX_LIBRARIES Qt6::Widgets Qt6::DBus Qt6::Svg)

set(SOURCES
    watcher.cpp
    batteryhelper.cpp
    batteryinfoframe.cpp
    batteryinfodialog.cpp
    batterywatcher.cpp
    idlenesswatcher.cpp
    lid.cpp
    lidwatcher.cpp
    main.cpp
    powermanagementd.cpp
    trayicon.cpp
    iconproducer.cpp
    powerbutton.cpp
    ../config/powermanagementsettings.cpp
)

set(UI_FILES
    batteryinfoframe.ui
    batteryinfodialog.ui
)

set(DESKTOP_FILES_IN
    lxqt-powermanagement.desktop.in
)

lxqt_translate_ts(QM_FILES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    SOURCES
        ${SOURCES}
        ${UI_FILES}
    INSTALL_DIR
        ${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}
)

lxqt_app_translation_loader(lxqt-powermanagement_QM_LOADER ${PROJECT_NAME})

include_directories(
    ${Qt6Gui_PRIVATE_INCLUDE_DIRS}
)

add_executable(lxqt-powermanagement
    ${SOURCES}
    ${RSCS}
    ${QM_FILES}
    ${lxqt-powermanagement_QM_LOADER}
    ${DESKTOP_FILES}
)

target_link_libraries(lxqt-powermanagement
    lxqt
    ${QTX_LIBRARIES}
    KF6::Solid
    KF6::IdleTime
    lxqt-globalkeys
    ${XCB_LIBRARIES}
)

install(TARGETS
    ${PROJECT_NAME}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)
install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
    COMPONENT Runtime
)
