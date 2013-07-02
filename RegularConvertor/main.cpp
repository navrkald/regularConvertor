#include <QApplication>
#include "mainwindow.h"
#include <graphviz/gvc.h>
//#include <graphviz/cgraph.h>

static inline Agraph_t* _agopen(QString name)
{
    return agopen(const_cast<char *>(qPrintable(name)), Agdirected,0);
}

///// Add an alternative value parameter to the method for getting an object's attribute
//static inline QString _agget(void *object, QString attr, QString alt=QString())
//{
//    QString str=agget(object, const_cast<char *>(qPrintable(attr)));

//    if(str==QString())
//        return alt;
//    else
//        return str;
//}

///// Directly use agsafeset which always works, contrarily to agset
//static inline int _agset(void *object, QString attr, QString value)
//{
//    return agsafeset(object, const_cast<char *>(qPrintable(attr)),
//                     const_cast<char *>(qPrintable(value)),
//                     const_cast<char *>(qPrintable(value)));
//}


int main(int argc, char *argv[])
{
    Agraph_t* graph = _agopen("novy graph");
    //testing_graphicz(argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}




