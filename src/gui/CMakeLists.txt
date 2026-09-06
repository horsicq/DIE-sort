include(../../dep/build_tools/cmake/cpp_standart_setup.cmake)

# Adds the xjs* sources to XPDF and defines USE_PDFJSEMUL (see xpdf.cmake)
option(WITH_PDFJSEMUL "Use PDF JavaScript emulator" ON)
add_definitions(-DUSE_YARA)
if(USE_DIE)
    add_definitions(-DUSE_DIE)
endif()
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

# Everything except main_gui.cpp goes into an OBJECT library so the smoke-test binary in
# tests/ reuses the same ~570 compiled objects and only pays for its own link.
set(DIE_SORT_GUI_OBJECT_SOURCES ${DIE_SORT_GUI_SOURCES})
list(REMOVE_ITEM DIE_SORT_GUI_OBJECT_SOURCES main_gui.cpp)

# The Windows .rc carries its own manifest, which would collide with the one the linker
# generates for the test binary, and the icon/version block only belongs to the app anyway.
set(DIE_SORT_APP_ONLY_SOURCES)

if(WIN32 AND EXISTS "${X_WINDOWS_RESOURCE_ICON_RC}")
    list(REMOVE_ITEM DIE_SORT_GUI_OBJECT_SOURCES "${X_WINDOWS_RESOURCE_ICON_RC}")
    list(APPEND DIE_SORT_APP_ONLY_SOURCES "${X_WINDOWS_RESOURCE_ICON_RC}")
endif()

add_library(diesort_gui_objects OBJECT ${DIE_SORT_GUI_OBJECT_SOURCES})

# PUBLIC so that both the application and the smoke-test binary inherit the include paths
# and link the same set of libraries by linking this one target. The *.cmake includes above
# add their headers with directory-scoped include_directories(), which a sibling directory
# would not see, so they are re-exported here as a usage requirement.
get_property(DIE_SORT_GUI_INCLUDE_DIRS DIRECTORY PROPERTY INCLUDE_DIRECTORIES)

target_include_directories(diesort_gui_objects PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${DIE_SORT_GUI_INCLUDE_DIRS}
)

target_link_libraries(diesort_gui_objects PUBLIC
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
    target_link_libraries(diesort_gui_objects PUBLIC Qt5::Script)
elseif(DIE_SORT_QT_MAJOR GREATER_EQUAL 6)
    target_link_libraries(diesort_gui_objects PUBLIC Qt${DIE_SORT_QT_MAJOR}::Qml)
endif()

if(WIN32)
    target_link_libraries(diesort_gui_objects PUBLIC Wintrust Crypt32)
endif()

add_executable(DIESort WIN32
    main_gui.cpp
    ${DIE_SORT_APP_ONLY_SOURCES}
)

# The .ui files live outside this binary directory, so AUTOUIC writes them to a shared
# mirrored path rather than a per-target autogen dir. Leaving it on for both targets makes
# them claim the same outputs. DIESort compiles only main_gui.cpp, which includes no
# ui_*.h, no Q_OBJECT and no .qrc - all of that is in diesort_gui_objects.
set_target_properties(DIESort PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE    TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "${MACOSX_BUNDLE_GUI_IDENTIFIER}"
    OUTPUT_NAME "die-sort"
)

if(X_ENABLE_ASAN)
    if(MSVC)
        target_compile_options(DIESort PRIVATE /fsanitize=address)
    else()
        target_compile_options(DIESort PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(DIESort PRIVATE -fsanitize=address)
    endif()
endif()

add_custom_command(TARGET DIESort POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E touch $<TARGET_FILE_DIR:DIESort>/portable
)

# Linking the OBJECT library pulls in its objects and its usage requirements.
target_link_libraries(DIESort PRIVATE diesort_gui_objects)

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
