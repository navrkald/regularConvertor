#-------------------------------------------------
#
# Project created by QtCreator 2012-12-27T18:46:24
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# "cs" je ISO 639 kód pro češtinu, "CZ" je ISO 3166 kód pro ČR
TRANSLATIONS = RegularConvertor_cs_CZ.ts
CODECFORTR = UTF-8

TARGET = RegularConvertor
TEMPLATE = app

#unix {
#    LIBS += -lGL
#    QT += testlib
#}

#win32 {
##    QT += testlib
#    LIBS += -lopengl32
#   QMAKE_LFLAGS += -shared
#}

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
    finite_machine/symbolsinputdialog.cpp \
    reg_exp/regexp.cpp \
    algorithms/algorithmwidget.cpp \
    algorithms/algorithm.cpp \
    algorithms/regexptofa.cpp \
    reg_exp/parserstack.cpp \
    reg_exp/regexpparser.cpp \
    reg_exp/regexpnode.cpp \
    reg_exp/charpos.cpp \
    reg_exp/regexptreemodel.cpp \
    reg_exp/regextextedit.cpp \
    reg_exp/regexpwidget.cpp \
    algorithms/removeepsilon.cpp \
    algorithms/fatodfa.cpp \
    algorithms/dfatominfa.cpp \
    set/set_of_sets.cpp \
    algorithms/htmldelegate.cpp \
    algorithms/algorithmview.cpp \
    algorithms/epsiloncloserwidget.cpp \
    mytooltipqmenu.cpp \
    finite_machine/pushdownautomata.cpp \
    finite_machine/pdacompotutationalrules.cpp \
    algorithms/algorithmcfgtopda.cpp \
    CFG/contextfreegrammar.cpp \
    PDA/pdawidget.cpp \
    CFG/cfgwidget.cpp \
    gui_algorithm_interface/cfgtopdaguiinterface.cpp \
    PDA/pdadiagramscene.cpp \
    PDA/pdaruledialog.cpp \
    PDA/pdaarrow.cpp \
    widgets/cfgtopdawidget.cpp \
    widgets/variableswidget.cpp

HEADERS  += mainwindow.h \
    finite_machine/diagramscene.h \
    finite_machine/statenode.h \
    finite_machine/arrow.h \
    finite_machine/finiteautomata.h \
    finite_machine/computationalrules.h \
    finite_machine/fa_widget.h \
    finite_machine/multiselectcompleter.h \
    finite_machine/editruledialog.h \
    finite_machine/symbolsinputdialog.h \
    reg_exp/regexp.h \
    algorithms/algorithmwidget.h \
    algorithms/algorithm.h \
    algorithms/regexptofa.h \
    reg_exp/parserstack.h \
    reg_exp/regexpparser.h \
    reg_exp/charpos.h \
    reg_exp/rules_defines.h \
    reg_exp/regexpnode.h \
    reg_exp/regexptreemodel.h \
    reg_exp/regextextedit.h \
    reg_exp/regexpwidget.h \
    algorithms/removeepsilon.h \
    algorithms/fatodfa.h \
    algorithms/dfatominfa.h \
    set/set_of_sets.h \
    algorithms/htmldelegate.h \
    algorithms/algorithmview.h \
    algorithms/epsiloncloserwidget.h \
    mytooltipqmenu.h \
    finite_machine/pushdownautomata.h \
    finite_machine/pdacompotutationalrules.h \
    algorithms/algorithmcfgtopda.h \
    CFG/contextfreegrammar.h \
    PDA/pdawidget.h \
    CFG/cfgwidget.h \
    gui_algorithm_interface/cfgtopdaguiinterface.h \
    error.h \
    PDA/pdadiagramscene.h \
    PDA/pdaruledialog.h \
    PDA/pdaarrow.h \
    widgets/centralwidgetinterface.h \
    widgets/cfgtopdawidget.h \
    widgets/variableswidget.h


FORMS    += mainwindow.ui \
    finite_machine/fa_widget.ui \
    finite_machine/editruledialog.ui \
    finite_machine/symbolsinputdialog.ui \
    algorithms/algorithmwidget.ui \
    reg_exp/regexpwidget.ui \
    CFG/cfgwidget.ui \
    PDA/pdaruledialog.ui \
    widgets/cfgtopdawidget.ui \
    widgets/variableswidget.ui

CONFIG += c++11 #static

# Gcc compiler flags
gcc:QMAKE_CXXFLAGS += -g -gdwarf-2

# Msvc compiler flags
msvc:QMAKE_CXXFLAGS +=

RESOURCES += \
    pictures.qrc \
    translations.qrc
