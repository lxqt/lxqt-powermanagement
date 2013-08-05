project(lz-lid)

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR}
                      ${CMAKE_SOURCE_DIR}/libraries/
                    )

set ( lid_HDRS
      lid.h
      razorautosuspend.h
        )

set ( lid_SRCS
        main.cpp
        lid.cpp
        lid.cpp
        razorautosuspend.cpp
        )

set ( lid_MOCS
      lid.h
      razorautosuspend.h
    )

# TODO: remove actual file and its translations
set(DESKTOP_FILES_IN
#    lid.desktop.in
)

QT4_WRAP_CPP(MOCS ${lid_MOCS})

# Translations **********************************
include(RazorTranslate)
razor_translate_ts(QM_FILES
    SOURCES
        ${lid_HDRS}
        ${lid_SRCS}
        ${lid_UIS}
)

razor_translate_desktop(DESKTOP_FILES
    SOURCES
        ${DESKTOP_FILES_IN}
)
#************************************************

add_executable ( lz-lid ${lid_SRCS} ${UIS} ${RSCS} ${QM_FILES} ${MOCS} ${DESKTOP_FILES})
target_link_libraries ( lz-lid  
                        ${RAZORQT_LIBRARIES}
                        ${RAZORXDG_LIBRARIES}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTDBUS_LIBRARY} 
                        ${QT_QTSVG_LIBRARY})


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)
