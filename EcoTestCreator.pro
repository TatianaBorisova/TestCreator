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
    clientinfosaver.cpp \
    serversettingsview.cpp \
    testcreatorview.cpp \
    mainwindow.cpp

HEADERS  += \
    docfileprocessing.h \
    stringencryption.h \
    global.h \
    tcpserver.h \
    sqldatabase.h \
    clientinfosaver.h \
    serversettingsview.h \
    testcreatorview.h \
    mainwindow.h
