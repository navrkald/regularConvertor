#include <QApplication>
#include "mainwindow.h"
#include <QDebug>
#include "reg_exp/regexp.h"
#include "reg_exp/regexpparser.h"
#include "algorithms/regexptofa.h"
#include "finite_machine/arrow.h"

#include "finite_machine/finiteautomata.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    return a.exec();
}



