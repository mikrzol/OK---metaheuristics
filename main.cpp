#include <iostream>
#include "graph.h"

using namespace std;

void add_example_edges(Graph& g) {
    // 1
        g.add_edge(1, 2, 2);
        g.add_edge(1,4,3);
    // 2
        //g.add_edge(2,1,2);
        g.add_arc(2,3,4);
        g.add_arc(2,4,2);
        g.add_edge(2,8,1);
    // 3
        g.add_edge(3,6,5);
    // 4
        g.add_arc(4,9,5);
    // 5
        g.add_edge(5,6,4);
        g.add_arc(5,7,5);
    // 6
        g.add_edge(6,7,3);
    // 7
        g.add_edge(7,8,3);
        g.add_arc(7,9,2);
    // 8
        g.add_edge(8,9,2);
};

int main() {
    // create a new graph
    Graph g = Graph();

    // add 9 nodes to the graph
    for(int i = 1; i < 10; i++) {
        g.add_node(i);
    }

    // add example edges to the graph
    add_example_edges(g);

    // print the graph
    //g.print_graph();

    // naively traverse the graph
    pair<vector<Node*>, int> graded_path = g.traverse_graph_naive();

    cout << endl << "zrobione" << endl;
    return 1;
};

