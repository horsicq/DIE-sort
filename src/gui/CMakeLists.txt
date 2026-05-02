include(../../dep/build_tools/cmake/cpp_standart_setup.cmake)

add_definitions(-DUSE_DEX)
add_definitions(-DUSE_PDF)
add_definitions(-DUSE_ARCHIVE)
add_definitions(-DUSE_YARA)
add_definitions(-DUSE_DIE)
add_definitions(-DUSE_XSIMD)

# Include xscanwidgets first so its xscanengine dependency is marked defined
# before die_script.cmake runs — prevents duplicate sources.
include(${CMAKE_CURRENT_LIST_DIR}/../../dep/XScanEngine/xscanwidgets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../dep/die_script/die_script.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../dep/XStyles/xstyles.cmake)
if(NOT DEFINED XYARA_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../../dep/XYara/xyara.cmake)
endif()

if(UNIX AND NOT APPLE)
    include(GNUInstallDirs)
endif()

set(DIE_SORT_GUI_SOURCES
    ${XSCANWIDGETS_SOURCES}
    ${DIE_SCRIPT_SOURCES}
    ${XSTYLES_SOURCES}
    ${XYARA_SOURCES}
    main_gui.cpp
    guimainwindow.cpp
    guimainwindow.h
    guimainwindow.ui
    "${CMAKE_CURRENT_LIST_DIR}/../../res/resources_gui.qrc"
)

if(WIN32 AND EXISTS "${X_WINDOWS_RESOURCE_ICON_RC}")
    list(APPEND DIE_SORT_GUI_SOURCES "${X_WINDOWS_RESOURCE_ICON_RC}")
endif()

if(APPLE AND EXISTS "${X_MACOS_ICON}")
    set_source_files_properties("${X_MACOS_ICON}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    list(APPEND DIE_SORT_GUI_SOURCES "${X_MACOS_ICON}")
endif()

add_executable(DIESort WIN32
    ${DIE_SORT_GUI_SOURCES}
)

set_target_properties(DIESort PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE    TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "${MACOSX_BUNDLE_GUI_IDENTIFIER}"
    OUTPUT_NAME "die-sort"
)

target_include_directories(DIESort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(DIESort PRIVATE
    Qt${DIE_SORT_QT_MAJOR}::Widgets
    Qt${DIE_SORT_QT_MAJOR}::Concurrent
    Qt${DIE_SORT_QT_MAJOR}::Sql
    bzip2
    lzma
    ppmd
    zlib
    capstone
    yara
    xsimd
)

if(DIE_SORT_QT_MAJOR EQUAL 5)
    target_link_libraries(DIESort PRIVATE Qt5::Script)
elseif(DIE_SORT_QT_MAJOR GREATER_EQUAL 6)
    target_link_libraries(DIESort PRIVATE Qt${DIE_SORT_QT_MAJOR}::Qml)
endif()

if(WIN32)
    target_link_libraries(DIESort PRIVATE Wintrust Crypt32)
endif()

if(TARGET diesort_translations)
    add_dependencies(DIESort diesort_translations)
endif()

if(UNIX AND NOT APPLE)
    set(DIE_SORT_RUNTIME_DESTINATION "${CMAKE_INSTALL_BINDIR}")
    set(DIE_SORT_DOC_DESTINATION     "${CMAKE_INSTALL_DOCDIR}/diesort")
    set(DIE_SORT_DESKTOP_FILE        "${CMAKE_CURRENT_BINARY_DIR}/die-sort.desktop")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/../../res/linux/die-sort.desktop.in"
        "${DIE_SORT_DESKTOP_FILE}"
        @ONLY
    )
else()
    set(DIE_SORT_RUNTIME_DESTINATION ".")
    set(DIE_SORT_DOC_DESTINATION     ".")
endif()

install(TARGETS DIESort
    RUNTIME DESTINATION ${DIE_SORT_RUNTIME_DESTINATION}
    BUNDLE  DESTINATION .
)

if(UNIX AND NOT APPLE)
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../README.md"
        DESTINATION ${DIE_SORT_DOC_DESTINATION}
        OPTIONAL
    )

    install(FILES "${DIE_SORT_DESKTOP_FILE}"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../res/linux/hicolor/"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor")
else()
    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE"
        DESTINATION .
        RENAME LICENSE.txt
        OPTIONAL
    )

    install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../README.md"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../res/license.txt"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../res/readme.txt"
        "${X_WINDOWS_ICON}"
        DESTINATION .
        OPTIONAL
    )

    install(CODE [[file(WRITE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/portable" "")]])
endif()

if(APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/macdeployqt.cmake)
endif()

if(WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/deploy_qt_windows.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/../../dep/build_tools/cmake/deploy_msvc.cmake)
endif()
