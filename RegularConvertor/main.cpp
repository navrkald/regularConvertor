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

    // kódování pro lokalizaci bude UTF-8
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    QTranslator t;

    /* načteme překlad podle aktuální locale ze souboru ve tvaru:
     *	název programu + podtržítko + jazyk_země
     * kde jazyk je dvoumístný kód jazyka (ISO 639) malými písmeny a
     * země je dvoumístný kód země (ISO 3166) velkými písmeny, např. cs_CZ
     */
    //t.load( app.applicationName() + "_" + QLocale::system().name() );

    if(t.load(":/translations/language/RegularConvertor_cs_CZ.qm"))
        qDebug() << "Překlad úspěšně načten.";
    else
        qDebug() << "Překlad nenačten!";


    // a přiřadíme jej k této instanci programu
    a.installTranslator(&t);


    return a.exec();
}



