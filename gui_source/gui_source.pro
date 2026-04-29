QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = die-sort
TEMPLATE = app

XCONFIG += use_dex
XCONFIG += use_pdf
XCONFIG += use_archive
XCONFIG += use_yara
XCONFIG += use_die
XCONFIG += use_xsimd

SOURCES += \
        guimainwindow.cpp \
        main_gui.cpp

HEADERS += \
        guimainwindow.h \
        ../global.h

FORMS += \
        guimainwindow.ui

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include(../die_script/die_script.pri)
}

!contains(XCONFIG, xscanwidgets) {
    XCONFIG += xscanwidgets
    include(../XScanEngine/xscanwidgets.pri)
}

include(../build.pri)

win32 {
    RC_ICONS = ../icons/main.ico
}

macx {
    ICON = ../icons/main.icns
}

DISTFILES += \
    ../build_win32.bat
