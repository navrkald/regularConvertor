#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{




    QApplication a(argc, argv);

//    Graphviz* graph = new Graphviz("GraphName",qApp->font(),50);

//    graph->addNode("Prvni uzel");
//    graph->addNode("Druhy uzel");
//    graph->addNode("Treti uzel");
//    graph->addNode("Ctvrty uzel");
//    graph->addNode("Paty uzel");

//    graph->setRootNode("Prvni uzel");

//    graph->addEdge("Prvni uzel","Druhy uzel","h1");
//    graph->addEdge("Prvni uzel","Druhy uzel","h2");
//    graph->addEdge("Druhy uzel","Treti uzel","h2");
//    graph->addEdge("Druhy uzel","Ctvrty uzel","h2");
//    graph->addEdge("Ctvrty uzel","Paty uzel","h2");
//    graph->applyLayout("dot");

//    QList<Graphviz::GVNode> nodes = graph->nodes();
//    Graphviz::GVNode node;

//    QString tmp = "Ahoj svete";
//    foreach(node,nodes)
//    {
//        qDebug() << node.name << " "<< node.str_centerPos;
//    }

    MainWindow w;
    w.show();

    return a.exec();
    return 0;
}

//#include <graphviz/cgraph.h>
//typedef struct mydata_s {Agrec_t hdr; int x,y,z;} mydata;

//main(int argc, char **argv)
//{
//    Agraph_t    *g,*h;
//    Agnode_t    *v;
//    Agedge_t    *e;
//    Agsym_t     *attr;
//    Dict_t      *d;
//    int         cnt;
//    mydata      *p;

//    if (g = agread(stdin,NIL(Agdisc_t*))) {
//          cnt = 0; attr = 0;
//          while (attr = agnxtattr(g, AGNODE, attr)) cnt++;
//          printf("The graph %s has %d attributes0",agnameof(g),cnt);

//          /* make the graph have a node color attribute, default is blue */
//        attr = agattr(g,AGNODE,"color","blue");

//        /* create a new graph of the same kind as g */
//        h = agopen("tmp",Agdirected,0);

//        /* this is a way of counting all the edges of the graph */
//        cnt = 0;
//        for (v = agfstnode(g); v; v = agnxtnode(g,v))
//            for (e = agfstout(g,v); e; e = agnxtout(g,e))
//                cnt++;

//        /* attach records to edges */
//        for (v = agfstnode(g); v; v = agnxtnode(g,v))
//            for (e = agfstout(g,v); e; = agnxtout(g,e)) {
//                p = (mydata*) agbindrec(g,e,"mydata",sizeof(mydata),TRUE);
//                p->x = 27;  /* meaningless data access example */
//                    ((mydata*)(AGDATA(e)))->y = 999; /* another example */
//        }
//    }
//}



