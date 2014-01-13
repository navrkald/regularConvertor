#include <QApplication>
#include "mainwindow.h"
#include <QDebug>
#include "reg_exp/regexp.h"
#include "reg_exp/regexpparser.h"
#include "algorithms/regexptofa.h"

#include "finite_machine/finiteautomata.h"
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
    FiniteAutomata A;
    A.alphabet.insert("a");
    A.alphabet.insert("b");
    A.alphabet.insert("c");
    A.alphabet.insert("d");
    A.states.insert("1");
    A.states.insert("2");
    A.states.insert("3");
    A.states.insert("4");
    A.states.insert("5");
    A.startState = "1";
    A.finalStates.insert("3");
    A.finalStates.insert("4");
    A.finalStates.insert("5");
    A.rules.insert(ComputationalRules("1","2","a"));
    A.rules.insert(ComputationalRules("2","4","b"));
    A.rules.insert(ComputationalRules("4","1","a"));
    A.rules.insert(ComputationalRules("3","5","a"));
    A.rules.insert(ComputationalRules("4","4","c"));
    A.rules.insert(ComputationalRules("5","3","d"));


    FiniteAutomata B;
    B.alphabet.insert("c");
    B.alphabet.insert("d");
    B.alphabet.insert("e");
    B.alphabet.insert("f");
    B.states.insert("4");
    B.states.insert("5");
    B.states.insert("6");
    B.states.insert("7");
    B.states.insert("8");
    B.states.insert("9");
    B.states.insert("10");
    B.startState="4";
    B.finalStates.insert("8");
    B.finalStates.insert("9");
    B.finalStates.insert("10");
    B.rules.insert(ComputationalRules("4","6","c"));
    B.rules.insert(ComputationalRules("6","5","d"));
    B.rules.insert(ComputationalRules("5","9","e"));
    B.rules.insert(ComputationalRules("4","4","e"));
    B.rules.insert(ComputationalRules("9","7","f"));
    B.rules.insert(ComputationalRules("7","10","c"));
    B.rules.insert(ComputationalRules("10","4","d"));
    B.rules.insert(ComputationalRules("10","8","d"));
    B.rules.insert(ComputationalRules("8","8","e"));
    B.rules.insert(ComputationalRules("8","4","f"));

    FiniteAutomata C = FiniteAutomata::concatenate(A,B);
    //FiniteAutomata C = A +B;
    //FiniteAutomata C = FiniteAutomata::iteration(A);

    RegExp re("((abcd)*+5+6)*");
    RegExpToFA re_to_fa(re);
    re_to_fa.computeSolution();

    FiniteAutomata correct_FA;
    correct_FA.alphabet = QSet<QString>() << "a" << "b" << "c" <<"d"<<"5"<<"6";

    //if (re_to_fa.re.rootNode->correct_FA == correct_FA)



    RegExpParser parser;
    parser.parse("(ab)*");
    MainWindow w;
    w.show();
    //RegExp::addConcOperator("ab((c)d)ef");

    return a.exec();
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



