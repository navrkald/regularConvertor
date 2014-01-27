#-------------------------------------------------
#
# Project created by QtCreator 2012-12-27T18:46:24
#
#-------------------------------------------------

QT       += core gui widgets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maintest
TEMPLATE = app
INCLUDEPATH += ../RegularConvertor

SOURCES += maintest.cpp\
    finite_machine/finiteautomata_test.cpp\
    \
    ../RegularConvertor/finite_machine/finiteautomata.cpp\
    ../RegularConvertor/finite_machine/computationalrules.cpp\
    algorithms/regexptofa_test.cpp \
    ../RegularConvertor/algorithms/regexptofa.cpp \
    ../RegularConvertor/reg_exp/regexp.cpp \
    ../RegularConvertor/reg_exp/regexpparser.cpp \
    ../RegularConvertor/reg_exp/regexpnode.cpp \
    ../RegularConvertor/reg_exp/parserstack.cpp \
    ../RegularConvertor/reg_exp/charpos.cpp \
    ../RegularConvertor/algorithms/algorithm.cpp \
    algorithms/removeepsilon_test.cpp \
    ../RegularConvertor/algorithms/removeepsilon.cpp


HEADERS  += \
    finite_machine/finiteautomata_test.h\
    \
    ../RegularConvertor/finite_machine/finiteautomata.h\
    ../RegularConvertor/finite_machine/computationalrules.h\
    algorithms/regexptofa_test.h \
    ../RegularConvertor/algorithms/regexptofa.h \
    ../RegularConvertor/reg_exp/regexp.h \
    ../RegularConvertor/reg_exp/rules_defines.h \
    ../RegularConvertor/reg_exp/regexpparser.h \
    ../RegularConvertor/reg_exp/regexpnode.h \
    ../RegularConvertor/reg_exp/parserstack.h \
    ../RegularConvertor/reg_exp/charpos.h \
    ../RegularConvertor/algorithms/algorithm.h \
    algorithms/removeepsilon_test.h \
    ../RegularConvertor/algorithms/removeepsilon.h

FORMS    +=

CONFIG += static

RESOURCES +=

DEFINES += SRCDIR=\\\"$$PWD/\\\"
