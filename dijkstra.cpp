#include <iostream>
#include <vector>
#include "./h/graph.h"
#include "./h/pq.h"

using namespace std;
void dijkstra(Graph& g, int source, int target) {
    vector<bool> visited(g.node_map.size());
    vector<int> distance(g.node_map.size());
    vector<Node *> prev(g.node_map.size());

    distance[source-1] = 0;

    P_QUEUE pq;

    for(auto el : g.node_map) {
        if(el.first != source) {
            distance[el.first-1] = INF;
            prev[el.first-1] = NULL;
        }
        pq.insert(new Vertex(el.first, distance[el.first-1]));
    }

    while(pq.nodes.size()) {
        auto u = pq.pop();

        if(u->name == target) {
            break;
        }

        // get to the neighbors of u
        auto u_node = g.node_map.find(u->name);
        auto u_neigh = g.neighborhood_map.find(u_node->second);
        // for each neighbor of u
        for(auto el : u_neigh->second) {
            int alt_cost = distance[u->name-1] + el->weight;
            if(alt_cost < distance[el->destination->name-1]) {
                distance[el->destination->name-1] = alt_cost;
                prev[el->destination->name-1] = u_node->second;
                pq.decrease_priority(el->destination->name, alt_cost);
            }
        }
    }

    cout << "DISTANCE FROM " << source << endl;
    int counter = 1;
    for(auto el : distance) {
        cout << counter << "(" << el << ") ";
        counter++;
    }
    cout << endl;

    cout << "PREVIOUS: " << endl;
    counter = 1;
    for(auto el : prev) {
        if(el) {
            cout << counter << "(" << el->name << ")\t"; 
        } else {
            cout << counter << "(" << 0 << ")\t";
        }
        counter++;
    }
    cout << endl;
};

void dijkstra(Graph& g, int source) {
    vector<bool> visited(g.node_map.size());
    vector<int> distance(g.node_map.size());
    vector<Node *> prev(g.node_map.size());

    distance[source-1] = 0;

    P_QUEUE pq;

    for(auto el : g.node_map) {
        if(el.first != source) {
            distance[el.first-1] = INF;
            prev[el.first-1] = NULL;
        }
        pq.insert(new Vertex(el.first, distance[el.first-1]));
    }

    while(pq.nodes.size()) {
        auto u = pq.pop();

        // get to the neighbors of u
        auto u_node = g.node_map.find(u->name);
        auto u_neigh = g.neighborhood_map.find(u_node->second);
        // for each neighbor of u
        for(auto el : u_neigh->second) {
            int alt_cost = distance[u->name-1] + el->weight;
            if(alt_cost < distance[el->destination->name-1]) {
                distance[el->destination->name-1] = alt_cost;
                prev[el->destination->name-1] = u_node->second;
                pq.decrease_priority(el->destination->name, alt_cost);
            }
        }
    }

    cout << "DISTANCE FROM " << source << endl;
    int counter = 1;
    for(auto el : distance) {
        cout << counter << "(" << el << ") ";
        counter++;
    }
    cout << endl;
};

int main() {
    Graph g = load_graph_from_file();

    dijkstra(g, 1);

/*
    P_QUEUE pq;

    for(auto el : g.node_map) {
        pq.insert(new Vertex(el.first, 10*el.first));
    }

    cout << "NODES in ascending order (by name): " << endl;
    while(pq.nodes.size()) {
        auto p = pq.pop();
        cout << p->name << "(" << p->distance << ")" << " ";
    }
    cout << endl;
*/
    

    return 1;
}