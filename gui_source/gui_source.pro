QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = die-sort
TEMPLATE = app

SOURCES += \
        guimainwindow.cpp \
        main_gui.cpp \
        dialogscanprogress.cpp \
        scanprogress.cpp

HEADERS += \
        guimainwindow.h \
        ../global.h \
        dialogscanprogress.h \
        scanprogress.h

FORMS += \
        guimainwindow.ui \
        dialogscanprogress.ui

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include(../die_script/die_script.pri)
}

!contains(XCONFIG, xcapstone) {
    XCONFIG += xcapstone
    include(../XCapstone/xcapstone.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include(../FormatDialogs/xdialogprocess.pri)
}

include(../build.pri)

win32 {
    RC_ICONS = ../icons/main.ico
}

macx {
    ICON = ../icons/main.icns
}
