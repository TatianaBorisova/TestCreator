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
    mainwindow.cpp \
    docfileprocessing.cpp \
    stringencryption.cpp \
    serverview.cpp \
    tcpserver.cpp \
    sqldatabase.cpp

HEADERS  += \
    mainwindow.h \
    docfileprocessing.h \
    stringencryption.h \
    global.h \
    serverview.h \
    tcpserver.h \
    sqldatabase.h
