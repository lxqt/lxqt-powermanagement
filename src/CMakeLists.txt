project(lxqt-powermanagement)

set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus Qt5::Svg Qt5::X11Extras)

set(lxqt-powermanagement_MOCS
    watcher.h
    battery.h
    batterywatcher.h
    batteryinfoframe.h
    batteryinfodialog.h
    idlenesswatcher.h
    lid.h
    lidwatcher.h
    powermanagementd.h
    trayicon.h
    iconproducer.h
    ../config/powermanagementsettings.h
)

set(lxqt-powermanagement_SRCS
    watcher.cpp
    battery.cpp
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
    x11helper.cpp
    ../config/powermanagementsettings.cpp
)

set(lxqt-powermanagement_UIS
    batteryinfoframe.ui
    batteryinfodialog.ui
)

set(DESKTOP_FILES_IN
    lxqt-powermanagement.desktop.in
)

qt5_wrap_ui( UIS ${lxqt-powermanagement_UIS})
qt5_add_dbus_adaptor(lxqt-powermanagement_SRCS
    org.freedesktop.ScreenSaver.xml
    idlenesswatcher.h
    IdlenessWatcher
)

lxqt_translate_ts(QM_FILES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    SOURCES
        ${lxqt-powermanagement_SRCS}
        ${lxqt-powermanagement_UIS}
        ${lxqt-powermanagement_MOCS}
    INSTALL_DIR
        ${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}
)

lxqt_app_translation_loader(lxqt-powermanagement_QM_LOADER ${PROJECT_NAME})

# XCB stuff (TODO: move into findXCB)
find_package(PkgConfig)

pkg_check_modules(PKG_XCB xcb)
find_path(XCB_INCLUDE_DIR xcb/xcb.h
    ${PKG_XCB_INCLUDE_DIRS}
)
find_library(XCB_LIBRARY NAMES xcb
    PATHS
    ${PKG_XCB_LIBRARY_DIRS}
)

pkg_check_modules(PKG_X11_XCB x11-xcb)
find_path(X11_XCB_INCLUDE_DIR X11/Xlib-xcb.h
    ${PKG_X11_XCB_INCLUDE_DIRS}
)
find_library(X11_XCB_LIBRARY NAMES X11-xcb
    PATHS
    ${PKG_X11_XCB_LIBRARY_DIRS}
)

pkg_check_modules(PKG_XCB_SCREENSAVER xcb-screensaver)
find_path(XCB_SCREENSAVER_INCLUDE_DIR xcb/screensaver.h
    ${PKG_XCB_SCREENSAVER_INCLUDE_DIRS}
)
find_library(XCB_SCREENSAVER_LIBRARY NAMES xcb-screensaver
    PATHS
    ${PKG_XCB_SCREENSAVER_LIBRARY_DIRS}
)

pkg_check_modules(PKG_XCB_DPMS xcb-dpms)
find_path(XCB_DPMS_INCLUDE_DIR xcb/dpms.h
    ${PKG_XCB_SCREENSAVER_INCLUDE_DIRS}
)
find_library(XCB_DPMS_LIBRARY NAMES xcb-dpms
    PATHS
    ${PKG_XCB_DPMS_LIBRARY_DIRS}
)


include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${XCB_INCLUDE_DIR}
    ${X11_XCB_INCLUDE_DIR}
    ${QTXDG_INCLUDE_DIRS}
    ${XCB_SCREENSAVER_INCLUDE_DIR}
    ${XCB_DPMS_INCLUDE_DIR}
)

add_executable(lxqt-powermanagement
    ${lxqt-powermanagement_SRCS}
    ${UIS}
    ${RSCS}
    ${QM_FILES}
    ${lxqt-powermanagement_QM_LOADER}
    ${DESKTOP_FILES}
)

target_link_libraries(lxqt-powermanagement
    ${LXQT_LIBRARIES}
    ${QTXDG_LIBRARY}
    ${QTX_LIBRARIES}
    ${XCB_LIBRARY}
    ${XCB_SCREENSAVER_LIBRARY}
    ${XCB_DPMS_LIBRARY}
    ${X11_XCB_LIBRARY}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES ${DESKTOP_FILES} DESTINATION share/applications)
