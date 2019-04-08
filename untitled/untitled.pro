#-------------------------------------------------
#
# Project created by QtCreator 2019-02-25T00:57:31
#
#-------------------------------------------------

QT       += core gui serialport testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
