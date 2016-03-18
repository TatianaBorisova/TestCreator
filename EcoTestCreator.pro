#-------------------------------------------------
#
# Project created by QtCreator 2016-01-18T22:55:09
#
#-------------------------------------------------

QT       += core gui widgets axcontainer network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EcoTestCreator
TEMPLATE = app


SOURCES += main.cpp\
    docfileprocessing.cpp \
    stringencryption.cpp \
    tcpserver.cpp \
    sqldatabase.cpp \
    mainwindow.cpp \
    views/testcreatorbaseview.cpp \
    views/mainwindowtab.cpp \
    views/questioncreatortabview.cpp \
    views/settingstabview.cpp \
    views/testdbview.cpp \
    views/resultdbview.cpp \
    subviews/testeditorsubview.cpp \
    subviews/questioneditorsubview.cpp \
    subviews/settingsubview.cpp \
    subviews/questionslistsubview.cpp \
    subviews/dbtestviewer.cpp \
    subviews/resultdbsubview.cpp \
    clientdatamanager.cpp

HEADERS  += \
    docfileprocessing.h \
    stringencryption.h \
    global.h \
    tcpserver.h \
    sqldatabase.h \
    mainwindow.h \
    views/testcreatorbaseview.h \
    views/mainwindowtab.h \
    views/questioncreatortabview.h \
    views/settingstabview.h \
    views/testdbview.h \
    views/resultdbview.h \
    subviews/testeditorsubview.h \
    subviews/questioneditorsubview.h \
    subviews/settingsubview.h \
    subviews/questionslistsubview.h \
    subviews/dbtestviewer.h \
    subviews/resultdbsubview.h \
    clientdatamanager.h

RESOURCES += \
    res.qrc
