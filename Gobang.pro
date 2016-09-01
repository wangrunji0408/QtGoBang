#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T14:05:42
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gobang
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gobangmanager.cpp \
    gridview.cpp \
    ipdialog.cpp \
    gobangmessager.cpp

HEADERS  += mainwindow.h \
    gobangmanager.h \
    gridview.h \
    ipdialog.h \
    gobangmessager.h

FORMS    += mainwindow.ui \
    ipdialog.ui

CONFIG += c++11

RESOURCES += \
    res.qrc
