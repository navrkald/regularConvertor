#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H
#include <graphviz/gvc.h>
#include <QtCore>
#include <QFont>
#include <QPainterPath>
#include <cgraph.h>

#define DONTCREATESAME 0

//tato trida implementuje automaticke rozmisteni uzlu grafu
class Graphviz
{    


public:

    /// A struct containing the information for a GVGraph's node
    struct GVNode
    {
        /// The unique identifier of the node in the graph
        QString name;

        /// The position of the center point of the node from the top-left corner
        QPoint centerPos;

        QString str_centerPos;

        /// The size of the node in pixels
        double height, width;
    };

    /// A struct containing the information for a GVGraph's edge
    struct GVEdge
    {
        /// The source and target nodes of the edge
        QString source;
        QString target;

        /// Path of the edge's line
        QPainterPath path;
    };

    /// Default DPI value used by dot (which uses points instead of pixels for coordinates)
    static const qreal DotDefaultDPI;

    /*!
     * \brief Construct a Graphviz graph object
     * \param name The name of the graph, must be unique in the application
     * \param font The font to use for the graph
     * \param node_size The size in pixels of each node
     */
    Graphviz(QString name, QFont font=QFont(), qreal node_size=50);
    ~Graphviz();

    /// Add and remove nodes
    void addNode(const QString& name);
    void addNodes(const QStringList& names);
    void removeNode(const QString& name);
    void clearNodes();

    /// Add and remove edges
    void addEdge(const QString &source, const QString &target, const QString edge_name);
    void removeEdge(const QString& source, const QString& target);

    /// Set the font to use in all the labels
    void setFont(QFont font);

    void removeEdge(const QPair<QString, QString> &key);
    void setRootNode(const QString &name);
    void applyLayout(QString layout_type);
    //QRectF boundingRect() const;
    QList<GVNode> nodes()const;
    QList<Graphviz::GVEdge> edges() const;

private:
    GVC_t *_context;
    Agraph_t *_graph;
    QFont _font;
    QMap<QString, Agnode_t*> _nodes;
    QMap<QPair<QString, QString>, Agedge_t*> _edges;

    /*
     *These methods are wrapped functions of graphviz lib.
    */
    Agraph_t*_agopen(QString name);
    //Get attribute from obj
    QString _agget(void *object, QString attr, QString alt)const;

    //Set attribute of obj
    int _agset(void *object, QString attr, QString value);
    //Agsym_t* _agnodeattr(void *g, QString name, QString value);
    //Agsym_t* _agedgeattr(graph_t* g,QString name,QString value);
    Agnode_t * _agnode(graph_t *graph, QString nodeName);
    int _gvLayout(GVC_t *gvc, graph_t *g, QString engine);
};

#endif // GRAPHVIZ_H
