#-------------------------------------------------
#
# Project created by QtCreator 2014-08-02T13:30:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dragpane
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    PaneWidget.cpp \
    PaneSubWidget.cpp

HEADERS  += mainwindow.h \
    PaneWidget.h \
    PaneMeta.h \
    PaneSubWidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressource.qrc

OTHER_FILES +=
