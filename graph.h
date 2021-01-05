#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <stdlib.h>
#include <time.h>
#include <utility>
#pragma once

using namespace std;

// Node struct
struct Node {
    int name;
};

// Edge or Arc struct
struct Edge_Or_Arc {
    // source is in the Graph neighborhood_list as the first parameter
    // node which the edge/ arc goes to
    Node* destination;
    // weight of the edge/arc
    int weight;
    // multiplicity if it's needed
    int multiplicity;
    // 1 if edge, 0 if arc
    bool is_edge;

    // constructor for multiplicity initialisation
    Edge_Or_Arc() : multiplicity(0) {};
};

// Graph struct
struct Graph {
    // a collection of Nodes
    // map because of the O(1) retrieval
    // map<name, address>
    unordered_map<int, Node *> node_map; 

    // a collection of edges or arcs
    // map<source Node*, a collection of edges/arcs>
    unordered_map<Node *, vector<Edge_Or_Arc *>> neighborhood_map;

    // methods
    // create a Node
    void add_node(int name);
    // na razie - konkretne wartości wagi
    // add an edge to the Graph
    void add_edge(int src, int dest, int weight);
    // add an arc to the Graph
    void add_arc(int src, int dest, int weight);

    // print the graph
    void print_graph();

    // naive graph traversal
    pair<vector<Node*>, int> traverse_graph_naive();

    // basic graph traversal
    void DFS(Node* start);

    // recursive part of DFS
    void DFS_recur(Node* v, bool visited[]);

    // constructor just to have access to the destructor
    Graph(){};

    // destructor
    ~Graph() {
        for(auto node : neighborhood_map) {
            for(auto edge_or_arc : node.second) {
                delete edge_or_arc;
            }
            delete node.first;
        }
        cout << "cleanup done" << endl;
    }
};

void Graph::add_node(int name) {
    // create a new node
    Node *new_node = new Node;
    // give it the specified name
    new_node->name = name;
    // put the address of the new node into the map
    this->node_map[name] = new_node;
};

void Graph::add_edge(int src, int dest, int weight) {
    // 1) SRC TO DEST SECTION 
        // create a new Edge_Or_Arc object on the heap
        Edge_Or_Arc* new_edge = new Edge_Or_Arc;

        // find the desired nodes in the nodes_map
        auto src_it = this->node_map.find(src);
        auto dest_it = this->node_map.find(dest);

        // assign specified values to the edge
        new_edge->destination = dest_it->second;
        new_edge->is_edge = 1;
        new_edge->multiplicity++;
        new_edge->weight = weight;

        // !!! NA RAZIE - PO PROSTU WEPCHNIJ NA KONIEC (MULTIPLICITY NIE MA ZNACZENIA, BO CHYBA NIE JEST ISTOTNE)
        // put the new connection into the neighborhood map
        this->neighborhood_map[src_it->second].push_back(new_edge);

    // 2) DEST TO SRC SECTION
        // create a new Edge_Or_Arc object on the heap
        Edge_Or_Arc* new_edge2 = new Edge_Or_Arc;

        // assign specified values to the edge
        new_edge2->destination = src_it->second;
        new_edge2->is_edge = 1;
        new_edge2->multiplicity++;
        new_edge2->weight = weight;

        // !!! NA RAZIE - PO PROSTU WEPCHNIJ NA KONIEC (MULTIPLICITY NIE MA ZNACZENIA, BO CHYBA NIE JEST ISTOTNE)
        // put the new connection into the neighborhood map
        this->neighborhood_map[dest_it->second].push_back(new_edge2);
};

void Graph::add_arc(int src, int dest, int weight) {
    // create a new Edge_Or_Arc object on the heap
    Edge_Or_Arc* new_edge = new Edge_Or_Arc;

    // find the desired nodes in the nodes_map
    auto src_it = this->node_map.find(src);
    auto dest_it = this->node_map.find(dest);

    // assign specified values to the edge
    new_edge->destination = dest_it->second;
    new_edge->is_edge = 0;
    new_edge->multiplicity++;
    new_edge->weight = weight;

    // !!! NA RAZIE - PO PROSTU WEPCHNIJ NA KONIEC (MULTIPLICITY NIE MA ZNACZENIA, BO CHYBA NIE JEST ISTOTNE)
    // put the new connection into the neighborhood map
    this->neighborhood_map[src_it->second].push_back(new_edge);
};

pair<vector<Node*>, int> Graph::traverse_graph_naive() {
    // store the graph size
    int graph_size = this->node_map.size();

    // create a pointer used to traverse the graph
    Node* current_node;

    // create a map that contains info on which nodes have already been visited
    unordered_map<Node*, bool> visited;

    // create a vector to store the visited nodes in the order they were visited
    vector<Node*> path;

    // store points for the path used
    int points = 0;
    
    // pick a random node to start from
    srand(time(NULL));
    int random_start = rand() % this->node_map.size() + 1; // +1 because naming starts from 1
    // find the node of the random_node name
    auto random_it = this->node_map.find(random_start);
    current_node = random_it->second;

    while(visited.size() != graph_size) {
        // add current node to path
        path.push_back(current_node);
        // check if current node has already been visited
        auto curr_vis_it = visited.find(current_node);
        // if the node has NOT been visited yet
        if(curr_vis_it == visited.end()) {
            // mark it as visited
            visited[current_node] = 1;
        }

        // find a random neighbor to go to - each vertex has at least one + the chances of getting an Edge or an Arc are equal
        // find current_node's neighbors list
        auto neighbors_it = this->neighborhood_map.find(current_node);
        auto curr_neighbors_list = neighbors_it->second;
        // get the index of the random Edge_Or_Arc
        int random_edge = rand() % curr_neighbors_list.size();

        // add the weight of the edge to points
        points += curr_neighbors_list[random_edge]->weight;

        // go to the randomly chosen neighbor
        current_node = curr_neighbors_list[random_edge]->destination;
    }

    cout << "Naive graph traversal path:" << endl;
    for(auto node : path) {
        cout << node->name << " ";
    }
    cout << endl;
    cout << "Path cost = " << points << endl;

    pair<vector<Node*>, int> graded_path;
    graded_path = make_pair(path, points);

    return graded_path;
};

void Graph::DFS(Node* start){
    // the visited array will be the size of the graph. A node will be marked on position corresponding to its name-1
    int size = this->node_map.size();
    bool *visited = new bool[size];
    for(int i = 0; i < size; i++) {
        visited[i] = 0;
    }

    // call the recursive part of the method
    this->DFS_recur(start, visited);
};

void Graph::DFS_recur(Node* vertex, bool visited[]) {
    // mark the current node as visited and print it
    visited[vertex->name-1] = 1;
    cout << vertex->name << " " << endl;

    // recur for all the vertices adjacent to this vertex
    // find the node on the neighborhood_mp
    auto v_it = this->neighborhood_map.find(vertex);
    // if it's been found in the neighborhood_map
    if(v_it != this->neighborhood_map.end()) {
        // loop over all of its neighbors
        for(auto neighbor : v_it->second) {
            // if the neighbor has not been visited yet
            if(!visited[neighbor->destination->name-1]) {
                DFS_recur(neighbor->destination, visited);
            }
        }
    }
};

void Graph::print_graph(){
    for(auto node : node_map) {
        // print the name of the Node
        cout << "Node name: " << node.first;

        // print the neighbors beginning
        cout <<"\tNeighbors: ";

        // find the node on the neighborhood_map
        auto node_it = neighborhood_map.find(node.second);
        // if there are any neighbors
        if(node_it != neighborhood_map.end()) {
            // loop over all the node's neighbors
            for(auto neighbor: node_it->second) {
                // and print them
                cout << neighbor->destination->name;
                // print the info whether it's an edge or an arc
                if(neighbor->is_edge) {
                    cout << "(E)";
                } else {
                    cout << "(A)";
                }
                cout << " ";
            }
            cout << endl;
        }
    }
};
