#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T10:32:22
#
#-------------------------------------------------

QT       += core gui charts serialbus serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tabai
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    digitkeyboard.cpp \
    plotoptions.cpp \
    plotter.cpp \
    lumelp18.cpp \
    dataminer.cpp

HEADERS  += mainwindow.h \
    digitkeyboard.h \
    plotoptions.h \
    plotter.h \
    lumelp18.h \
    dataminer.h

FORMS    += mainwindow.ui \
    digitkeyboard.ui \
    plotoptions.ui \
    plotter.ui

RESOURCES += \
    res.qrc

DISTFILES +=

