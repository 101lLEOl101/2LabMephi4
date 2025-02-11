#include "Graph.h"

void test(){
    Graph g(5);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(3, 4);

    assert(g.hasEdge(0, 1));
    assert(g.hasEdge(0, 2));
    assert(g.hasEdge(1, 2));
    assert(g.hasEdge(3, 4));

    g.removeEdge(0, 1);
    g.removeEdge(3, 4);

    assert(!g.hasEdge(0, 1));
    assert(!g.hasEdge(3, 4));

    g.addVertex();
    g.addEdge(1, 5);
    assert(g.hasEdge(1, 5));

    g.removeVertex(2);
    assert(!g.hasEdge(1, 2));
    assert(!g.hasEdge(2, 3));

    g = Graph(5);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(3, 4);
    g.colorGraph();
}