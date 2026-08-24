if(TARGET Qt6::Core OR TARGET Qt6::Widgets)
    set(DIE_SORT_QT_MAJOR 6)
elseif(TARGET Qt5::Core OR TARGET Qt5::Widgets)
    set(DIE_SORT_QT_MAJOR 5)
else()
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
    set(DIE_SORT_QT_MAJOR ${QT_VERSION_MAJOR})
endif()

set(DIE_SORT_QT_COMPONENTS
    Widgets
    Concurrent
    Sql
    LinguistTools
)

if(DIE_SORT_QT_MAJOR EQUAL 5)
    list(APPEND DIE_SORT_QT_COMPONENTS Script)
    if(NOT TARGET Qt5::Widgets)
        find_package(Qt5 REQUIRED COMPONENTS ${DIE_SORT_QT_COMPONENTS})
    endif()
elseif(DIE_SORT_QT_MAJOR GREATER_EQUAL 6)
    list(APPEND DIE_SORT_QT_COMPONENTS Qml)
    find_package(Qt${DIE_SORT_QT_MAJOR} REQUIRED COMPONENTS ${DIE_SORT_QT_COMPONENTS})

    if(NOT TARGET Qt5::Widgets)
        add_library(Qt5::Widgets INTERFACE IMPORTED)
        set_property(TARGET Qt5::Widgets PROPERTY
            INTERFACE_LINK_LIBRARIES Qt${DIE_SORT_QT_MAJOR}::Widgets)
    endif()
endif()

file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/../release_version.txt" X_PROJECT_VERSION)
project(DIESort VERSION ${X_PROJECT_VERSION} LANGUAGES CXX)
option(USE_DIE "Enable Detect It Easy integration" ON)

if(NOT PROJECT_VERSION_PATCH)
    set(PROJECT_VERSION_PATCH 0)
endif()
if(NOT PROJECT_VERSION_TWEAK)
    set(PROJECT_VERSION_TWEAK 0)
endif()

set(X_COMPANYNAME       "ntinfo")
set(X_PROJECTNAME       "DIESort")
set(X_MAINTAINER        "horsicq@gmail.com")
set(X_DESCRIPTION       "DIE-sort is a DIE sort utility")
set(X_HOMEPAGE          "http://ntinfo.biz")
set(X_ORIGINAL_FILENAME "die-sort")

if(NOT DEFINED MACOSX_BUNDLE_GUI_IDENTIFIER OR "${MACOSX_BUNDLE_GUI_IDENTIFIER}" STREQUAL "")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.ntinfo.diesort")
endif()
set(BUNDLE_ID_OPTION "${MACOSX_BUNDLE_GUI_IDENTIFIER}")

set(X_RESOURCE_DIR                "${CMAKE_CURRENT_LIST_DIR}/../res")
set(X_WINDOWS_ICON                "${X_RESOURCE_DIR}/windows/main.ico")
set(X_WINDOWS_RESOURCE_RC_IN      "${X_RESOURCE_DIR}/windows/resource.rc.in")
set(X_WINDOWS_RESOURCE_ICON_RC_IN "${X_RESOURCE_DIR}/windows/resource_icon.rc.in")
set(X_WINDOWS_RESOURCE_RC         "${X_RESOURCE_DIR}/windows/resource.rc")
set(X_WINDOWS_RESOURCE_ICON_RC    "${X_RESOURCE_DIR}/windows/resource_icon.rc")
set(X_MACOS_ICON                  "${X_RESOURCE_DIR}/macos/main.icns")
set(X_MACOS_INFO_PLIST_IN         "${X_RESOURCE_DIR}/macos/Info.plist.in")
set(X_MACOS_INFO_PLIST            "${X_RESOURCE_DIR}/macos/Info.plist")

set(DIE_SORT_DEPLOY_INIT "${CMAKE_CURRENT_LIST_DIR}/../dep/build_tools/cmake/deploy_init.cmake")
if(EXISTS "${DIE_SORT_DEPLOY_INIT}"
   AND EXISTS "${X_WINDOWS_RESOURCE_RC_IN}"
   AND EXISTS "${X_WINDOWS_RESOURCE_ICON_RC_IN}"
   AND EXISTS "${X_MACOS_ICON}"
   AND EXISTS "${X_MACOS_INFO_PLIST_IN}")
    include("${DIE_SORT_DEPLOY_INIT}")
else()
    message(STATUS "Skipping deploy_init.cmake — packaging templates missing")
endif()

set(DIE_SORT_TRANSLATION_HELPER "${CMAKE_CURRENT_LIST_DIR}/../dep/build_tools/cmake/deploy_translation.cmake")
if(EXISTS "${DIE_SORT_TRANSLATION_HELPER}")
    include("${DIE_SORT_TRANSLATION_HELPER}")

    set(DIE_SORT_TS_FILES
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_ar.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_bn.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_de.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_es.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_fa.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_fr.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_he.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_id.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_it.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_ja.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_ko.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_pl.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_pt_BR.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_pt_PT.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_ru.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_sv.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_tr.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_uk.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_vi.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_sq.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_zh.ts"
        "${CMAKE_CURRENT_LIST_DIR}/../translations/die-sort_zh_TW.ts"
    )

    deploy_add_translations(
        TARGET_NAME diesort_translations
        ADD_TO_ALL
        SOURCE_DIRS "${CMAKE_CURRENT_LIST_DIR}"
        TS_FILES ${DIE_SORT_TS_FILES}
        INSTALL_DESTINATION lang
        WINDOWS_APPDATA_SUBDIR "${X_PROJECTNAME}"
    )
endif()

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../dep/XCapstone" "${CMAKE_CURRENT_BINARY_DIR}/XCapstone")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../dep/XArchive"  "${CMAKE_CURRENT_BINARY_DIR}/XArchive")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../dep/XYara"     "${CMAKE_CURRENT_BINARY_DIR}/XYara")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../dep/Formats/xsimd" "${CMAKE_CURRENT_BINARY_DIR}/XSIMD")

add_subdirectory(gui)
