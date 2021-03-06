#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <fstream> // ofstream, ifstream
#include <sstream> // isstream
#include "./pq.h"

using namespace std;

// Node struct
struct Node {
    int name;
    int num_of_edges;
    int num_of_arcs;

    // basic constructor
    Node() : num_of_edges(0), num_of_arcs(0) {};
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

    // distance map for all the nodes given the steps left (distance_map[mode][steps_left][starting_node][destination] = pair<path, shortest distance>)
    //unordered_map<unordered_map<unordered_map<int, vector<Node*> >, bool>, bool> distance_map;
    vector<vector<vector<vector<pair<vector<Node*>, int>>>>> distance_matrix;

    // the amount of steps using either arcs or edges to take to not get penalty
    int penalty_frame;

    // methods
    // create a Node
    void add_node(int name);
    // na razie - konkretne wartości wagi
    // add an edge to the Graph
    void add_edge(int src, int dest, int weight);
    void add_edge_from_file(int src, int dest, int weight);
    // add an arc to the Graph
    void add_arc(int src, int dest, int weight);

    // print the graph
    void print_graph();

    // save the graph to file
    void save_graph_to_file();

    // naive graph traversal
    pair<vector<Node*>, int> traverse_graph_naive();

    // grade the traversal using the penalties logic
    int grade_traversal(vector<Node*>);

    // basic graph traversal
    void DFS(Node* start);

    // recursive part of DFS
    void DFS_recur(Node* v, vector<bool>& visited);

    // constructor just to have access to the destructor
    // !!! USE THE OTHER PARAMETER - PENALTY FRAME !!!
    Graph(): penalty_frame(5) {};

    // constructor for constructing Graph of a given size
    // !!! KEY PARAMETER - PENALTY FRAME !!!
    Graph(int size) : penalty_frame(9) {
        for(int i = 0; i < size; i++) {
            this->add_node(i+1);
        }
        // initialising distance_matrix
        this->distance_matrix = vector<vector<vector<vector<pair<vector<Node*>, int>>>>>(2);
        for(int mode = 0; mode < this->distance_matrix.size(); mode++) {
            this->distance_matrix[mode] = vector<vector<vector<pair<vector<Node*>, int>>>>(this->penalty_frame);
            for(int steps_left = 0; steps_left < this->distance_matrix[mode].size(); steps_left++) {
                this->distance_matrix[mode][steps_left] = vector<vector<pair<vector<Node*>, int>>>(size);
                for(int start = 0; start < size; start++) {
                    this->distance_matrix[mode][steps_left][start] = vector<pair<vector<Node*>, int>>(size);
                    for(int dest = 0; dest < size; dest++) {
                        this->distance_matrix[mode][steps_left][start][0] = make_pair<>(vector<Node*>(0), 0);
                    }
                }
            }
        }
    };

    // destructor
    ~Graph() {
        for(auto node : neighborhood_map) {
            for(auto edge_or_arc : node.second) {
                delete edge_or_arc;
            }
            delete node.first;
        }
        // cout << "cleanup done" << endl;
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

        // increment the edge counts for the src and dest
        src_it->second->num_of_edges++;
        dest_it->second->num_of_edges++;
};

void Graph::add_edge_from_file(int src, int dest, int weight) {
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

    // increment the amount of edges in src node
    src_it->second->num_of_edges++;
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

    // increment the amount of arcs in src node
    src_it->second->num_of_arcs++;
};

int Graph::grade_traversal(vector<Node*> path) {
    // !!! KEY PARAMETER - amount of Edges or Arcs needed to avoid penalty
    int penalty_frame = this->penalty_frame;
    // final cost will be stored here
    int cost = 0;
    // loop over all but the last one nodes in the path ( -1 because the last connection will be at that point)
    for(int i = 0; i < path.size() -1; i++) {
        // find the src and dest Nodes in neighborhood_map
        auto src_it = this->neighborhood_map.find(path[i]);
        auto dest_it = this->neighborhood_map.find(path[i+1]);
        // find the Edge_Or_Arc that connects the two nodes
        // loop over the vector of neighbors of the src Node
        for(auto neighbor : src_it->second) {
            // find the correct Edge_Or_Arc
            if(neighbor->destination == dest_it->first) {
                auto connection = neighbor;
                // determine whether an Edge or Arc avoids the penalty
                int mod = i % (penalty_frame*2) + 1;
                // first 5 - Edges avoid penalty
                if(mod < penalty_frame+1) {
                    // EDGE MODE
                    if(connection->is_edge) {
                        // simply add the weight of the edge to cost
                        cost += connection->weight;
                    }
                    // else if the connection is not an edge
                    else {
                        // apply penalty for this connection
                        cost += connection->weight * penalty_frame;
                    }
                }
                // second 5 - Arcs avoid penalty
                else {
                    // ARC MODE
                    if(!connection->is_edge) {
                        // simply add the weight of the arc to cost
                        cost += connection->weight;
                    }
                    // else if the connection is NOT an arc
                    else {
                        // apply penalty for this connection
                        cost += connection->weight * penalty_frame;
                    }
                }
                // no need to keep looping over the neighbors vector
                break;
            }
        }
    }
    // finally, return the cost
    return cost;
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
    
    // pick a random node to start from
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

        // go to the randomly chosen neighbor
        current_node = curr_neighbors_list[random_edge]->destination;
    }

    // calculate path's cost
    int cost = this->grade_traversal(path);

/* printing:
    cout << "Naive graph traversal path:" << endl;
    for(auto node : path) {
        cout << node->name << " ";
    }
    cout << endl;
    cout << "Path length = " << path.size() << endl;
    cout << "Path cost = " << cost << endl;
*/

    pair<vector<Node*>, int> graded_path;
    graded_path = make_pair(path, cost);

    return graded_path;
};

void Graph::DFS(Node* start){
    // the visited array will be the size of the graph. A node will be marked on position corresponding to its name-1
    int size = this->node_map.size();
    vector<bool> visited = vector<bool>(size);
    for(int i = 0; i < size; i++) {
        visited[i] = 0;
    }


    // call the recursive part of the method
    this->DFS_recur(start, visited);
cout << "JESTEM TUTAJ" << endl;

    int amount_visited = 0;
    for(auto el : visited) {
        if(el == 1) {
            amount_visited++;
        } 
    }
    cout << "VISITED " << amount_visited << " OUT OF " << visited.size() << " NODES" << endl;
};

void Graph::DFS_recur(Node* vertex, vector<bool>& visited) {
    // mark the current node as visited and print it
    cout << vertex->name << " " << endl;
    visited[vertex->name-1] = 1;

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

        cout << "\tnum of edges = " << node.second->num_of_edges << "\t num of arcs = " << node.second->num_of_arcs;

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

void Graph::save_graph_to_file() {
    ofstream data;
    data.open("./txt/saved_graph.txt");
    // first line in saved file will have the size of the graph for graph construction
    data << this->node_map.size() << "\n";

    for(auto node : node_map) {
        data << node.first << "\t";
        auto node_it = neighborhood_map.find(node.second);
        if(node_it != neighborhood_map.end()) {
            for(auto neighbor : node_it->second) {
                data << neighbor->destination->name << "#";
                if(neighbor->is_edge) {
                    data << "E" << "#";
                } else {
                    data << "A" << "#";
                }
                data << neighbor->weight;
                data << "\t";
            }
            data << "\n";
        }
    }
};

Graph load_graph_from_file() {
    ifstream data;
    // !!! IMPORTANT - USE THE FULL PATH TO WORKING_INSTANCE.txt FILE !!!
    data.open("./txt/WORKING_INSTANCE.txt");
    string line;
    // first line contains the size of the graph
    int size;
    getline(data, line);
    size = stoi(line);
    // create a graph based on this size
    Graph g = Graph(size);

    while(getline(data, line)) {
        string text(line);
        istringstream iss(text);
        string segment;
        vector<string> seglist;
        while(getline(iss, segment, '\t')) {
            seglist.push_back(segment);
        }
        // name of the src node is in seglist[0]
        string node_name = seglist[0];
        // info on all the connections is in seglist[1:]
        for(int i = 1; i < seglist.size(); i++) {
            // string parsing based on on a delimiter
            string delimiter = "#";
            size_t pos = 0;
            vector<string> tokens;
            while((pos = seglist[i].find(delimiter)) != string::npos) {
                string t = seglist[i].substr(0, pos);
                tokens.push_back(t);
                seglist[i].erase(0, pos + delimiter.length());
            }
            tokens.push_back(seglist[i]);

            string dest = tokens[0], type = tokens[1], weight = tokens[2];
            
            // create conncetions based on info stored in tokens
            if(type == "E"){
                g.add_edge_from_file(stoi(node_name), stoi(dest), stoi(weight));
            } else if (type == "A"){
                g.add_arc(stoi(node_name), stoi(dest), stoi(weight));
            }
            
            // printing
            /*
            for(auto t : tokens) {
                cout << t << " ";
            }
            cout << "\t";
            */
        }
    }
    return g;
};

// DIJKSTRA PART

struct Shortest_Path {
    vector<int> distance;
    vector<Node *> prev;

    vector<Node*> get_shortest_path(Graph& g, int& source, int& target) {
        vector<Node*> sp;
        int curr = target;

        auto target_it = g.node_map.find(target);
        sp.push_back(target_it->second);

        while(curr != source) {
            sp.push_back(prev[curr-1]);
            curr = this->prev[curr-1]->name;
        }
        //sp.push_back(prev[curr-1]);

        vector<Node*> asp;
        for(int i = sp.size()-2; i > -1; i--) {
            asp.push_back(sp[i]);
        }

        return asp;
    }

};

pair<vector<Node*>, int> dijkstra(Graph& g, int source, int target, const int& steps_left, const bool& mode) {
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
        pq.insert(Vertex(el.first, distance[el.first-1]));
    }
    // assign steps_left and mode to the Vertex at the top of the PQ
    pq.top()->steps_left = steps_left;
    pq.top()->mode = mode;

    while(pq.nodes.size()) {
        auto u = pq.pop();

        if(u.name == target) {
            break;
        }

        // get to the neighbors of u
        auto u_node = g.node_map.find(u.name);
        auto u_neigh = g.neighborhood_map.find(u_node->second);
        // for each neighbor of u
        for(auto el : u_neigh->second) {
            int alt_cost;
            // 0 - edge mode; 1 - arc mode
            if(u.mode == 0) {
                if(el->is_edge) {
                    alt_cost = distance[u.name-1] + el->weight;
                } else {
                    alt_cost = distance[u.name-1] + (el->weight * g.penalty_frame);
                }
            } else {
                if(el->is_edge) {
                    alt_cost = distance[u.name-1] + (el->weight * g.penalty_frame);
                } else {
                    alt_cost = distance[u.name-1] + el->weight;
                }
            }
            if(alt_cost < distance[el->destination->name-1]) {
                distance[el->destination->name-1] = alt_cost;
                prev[el->destination->name-1] = u_node->second;
                pq.decrease_priority(el->destination->name, alt_cost, u.steps_left, u.mode, g.penalty_frame);
            }
        }
    }

    // return the shorest path distance and prev vectors
    Shortest_Path sp = Shortest_Path();
    sp.distance = distance;
    sp.prev = prev;

    auto to_return = make_pair<>(sp.get_shortest_path(g, source, target), distance[target-1]);
    return to_return;
    
    //return sp.get_shortest_path(g, source, target);; 
};

vector<int> dijkstra_global(Graph& g, int source) {
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
        pq.insert(Vertex(el.first, distance[el.first-1]));
    }

    while(pq.nodes.size()) {
        auto u = pq.pop();

        // get to the neighbors of u
        auto u_node = g.node_map.find(u.name);
        auto u_neigh = g.neighborhood_map.find(u_node->second);
        // for each neighbor of u
        for(auto el : u_neigh->second) {
            int alt_cost = distance[u.name-1] + el->weight;
            if(alt_cost < distance[el->destination->name-1]) {
                distance[el->destination->name-1] = alt_cost;
                prev[el->destination->name-1] = u_node->second;
                pq.decrease_priority(el->destination->name, alt_cost, u.steps_left, u.mode, g.penalty_frame);
            }
        }
    }
/*
    for(int i = 0; i < distance.size(); i++) {
        cout << i+1 << ": " << distance[i] << endl;
    }
    cout << endl;
*/
    return distance;
};