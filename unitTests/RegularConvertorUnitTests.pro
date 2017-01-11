#-------------------------------------------------
#
# Project created by QtCreator 2012-12-27T18:46:24
#
#-------------------------------------------------

QT       += core gui opengl testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maintest
TEMPLATE = app
INCLUDEPATH += ../RegularConvertor

SOURCES +=\
    finite_machine/finiteautomata_test.cpp\
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
    ../RegularConvertor/algorithms/removeepsilon.cpp \
    ../RegularConvertor/algorithms/fatodfa.cpp \
    main_test.cpp \
    algorithms/fatodfa_test.cpp \
    ../RegularConvertor/set/set_of_sets.cpp \
    ../RegularConvertor/algorithms/dfatominfa.cpp \
    algorithms/dfatominfa_test.cpp \
    ../RegularConvertor/reg_exp/regexptreemodel.cpp \
    ../RegularConvertor/finite_machine/fa_widget.cpp \
    ../RegularConvertor/reg_exp/regexpwidget.cpp \
    ../RegularConvertor/finite_machine/arrow.cpp \
    ../RegularConvertor/finite_machine/diagramscene.cpp \
    ../RegularConvertor/finite_machine/editruledialog.cpp \
    ../RegularConvertor/finite_machine/multiselectcompleter.cpp \
    ../RegularConvertor/finite_machine/statenode.cpp \
    ../RegularConvertor/finite_machine/symbolsinputdialog.cpp \
    ../RegularConvertor/algorithms/algorithmview.cpp \
    ../RegularConvertor/algorithms/algorithmwidget.cpp \
    ../RegularConvertor/algorithms/htmldelegate.cpp \
    ../RegularConvertor/reg_exp/regextextedit.cpp \
    ../RegularConvertor/algorithms/epsiloncloserwidget.cpp \
    CFG/cfg_test.cpp \
    ../RegularConvertor/CFG/contextfreegrammar.cpp


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
    ../RegularConvertor/algorithms/removeepsilon.h \
    ../RegularConvertor/algorithms/fatodfa.h \
    algorithms/fatodfa_test.h \
    ../RegularConvertor/set/set_of_sets.h \
    ../RegularConvertor/algorithms/dfatominfa.h \
    algorithms/dfatominfa_test.h \
    ../RegularConvertor/reg_exp/regexptreemodel.h \
    ../RegularConvertor/finite_machine/fa_widget.h \
    ../RegularConvertor/reg_exp/regexpwidget.h \
    ../RegularConvertor/finite_machine/arrow.h \
    ../RegularConvertor/finite_machine/diagramscene.h \
    ../RegularConvertor/finite_machine/editruledialog.h \
    ../RegularConvertor/finite_machine/multiselectcompleter.h \
    ../RegularConvertor/finite_machine/statenode.h \
    ../RegularConvertor/finite_machine/symbolsinputdialog.h \
    ../RegularConvertor/algorithms/algorithmview.h \
    ../RegularConvertor/algorithms/algorithmwidget.h \
    ../RegularConvertor/algorithms/htmldelegate.h \
    ../RegularConvertor/reg_exp/regextextedit.h \
    ../RegularConvertor/algorithms/epsiloncloserwidget.h \
    CFG/cfg_test.h \
    ../RegularConvertor/CFG/contextfreegrammar.h

FORMS    += \
    ../RegularConvertor/finite_machine/fa_widget.ui \
    ../RegularConvertor/reg_exp/regexpwidget.ui \
    ../RegularConvertor/finite_machine/editruledialog.ui \
    ../RegularConvertor/finite_machine/symbolsinputdialog.ui \
    ../RegularConvertor/algorithms/algorithmwidget.ui

CONFIG += += c++11 #static
QMAKE_CXXFLAGS += -g -gdwarf-2
RESOURCES +=

#DEFINES += SRCDIR=\\\"$$PWD/\\\"
