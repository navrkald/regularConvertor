#-------------------------------------------------
#
# Project created by QtCreator 2012-12-27T18:46:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RegularConvertor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diagramscene.cpp \
    statenode.cpp \
    arrow.cpp \
    myqgraphictextitem.cpp \
    finiteautomata.cpp \
    computationalrules.cpp

HEADERS  += mainwindow.h \
    diagramscene.h \
    statenode.h \
    arrow.h \
    myqgraphictextitem.h \
    finiteautomata.h \
    computationalrules.h

FORMS    += mainwindow.ui
