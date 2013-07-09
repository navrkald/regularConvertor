#-------------------------------------------------
#
# Project created by QtCreator 2012-12-27T18:46:24
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RegularConvertor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    finite_machine/diagramscene.cpp \
    finite_machine/statenode.cpp \
    finite_machine/arrow.cpp \
    finite_machine/finiteautomata.cpp \
    finite_machine/computationalrules.cpp \
    finite_machine/fa_widget.cpp \
    finite_machine/multiselectcompleter.cpp \
    finite_machine/editruledialog.cpp \
    finite_machine/graphviz.cpp \
    finite_machine/symbolsinputdialog.cpp

HEADERS  += mainwindow.h \
    finite_machine/diagramscene.h \
    finite_machine/statenode.h \
    finite_machine/arrow.h \
    finite_machine/finiteautomata.h \
    finite_machine/computationalrules.h \
    finite_machine/fa_widget.h \
    finite_machine/multiselectcompleter.h \
    finite_machine/editruledialog.h \
    finite_machine/graphviz.h \
    finite_machine/symbolsinputdialog.h

FORMS    += mainwindow.ui \
    finite_machine/fa_widget.ui \
    finite_machine/editruledialog.ui \
    finite_machine/symbolsinputdialog.ui

CONFIG += static

QMAKE_CFLAGS_DEBUG += -I/usr/local/include/graphviz -Wall -g -O2
QMAKE_LFLAGS += -L/usr/local/lib -lgvc -lcgraph -lcdt
INCLUDEPATH += /usr/include/graphviz
