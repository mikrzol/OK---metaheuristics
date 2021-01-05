#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#pragma once

using namespace std;

struct Node {
    string name;
    vector<Node *> neighbors;
};

struct Graph {
    int size;
    int num_of_edges;
    //vector<Node *> nodeList;

    // nodeMap<node name, vector<neighbors names>>
    // nodeMap<vertex, edge list>
    // nodeMap<node name, unordered_map<neighbor name, edge multiplicity>>
    unordered_map<string, unordered_map<string, int>> nodeMap;

    bool can_be_adjoint;
    
    Graph() : size(0), num_of_edges(0), can_be_adjoint(1) {};

    //Graph(int V) : size(V), nodeList(V) {};

    void printSize(){
        cout << "The size of this graph is: " << size << endl;
    };

    void addVertex(string name) {

        // MAP IMPLEMENTATION - CONSTANT TIME
        auto it = nodeMap.find(name);
        if(it != nodeMap.end()) {
            return;
        }
        // otherwise add the vertex to the map
        nodeMap[name];
/*
        // VECTOR IMPLEMENTATION
        int i = 0;
        for(i; i < nodeList.size(); i++) {
            if(nodeList[i]->name == name) {
                return;
            }
        }
        Node *n = new Node;
        n->name = name;
        this->nodeList.push_back(n);
*/
    };

    void addEdge(string source, string destination) {

        // MAP IMPLEMENTATION
        auto src_it = nodeMap.find(source);
        auto dest_it = nodeMap.find(destination);

        // found both nodes
        if(src_it != nodeMap.end() && dest_it != nodeMap.end()) {
            // see whether there's already an edge
            auto neighbor_it = src_it->second.find(destination);
            // if this edge already exists
            if(neighbor_it != src_it->second.end()){
                this->can_be_adjoint = 0;
                neighbor_it->second += 1;
            } else {
                // otherwise create a new edge
                src_it->second[destination] = 1;
            }
        }
        // found only source
        else if(src_it != nodeMap.end() && dest_it == nodeMap.end()) {
            // add a new vertex to the map's first layer
            nodeMap[destination];
            // add a new edge
            src_it->second[destination] = 1;
            // this->size is unnecessary in the map implementation
            this->size++;
        }
        // found only destination
        else if(src_it == nodeMap.end() && dest_it != nodeMap.end()) {
            //add a new vertex to the map's first layer
            nodeMap[source];
            // add a new edge
            nodeMap[source][destination] = 1;
            // this->size is unnecessary in the map implementation
            this->size++;
        }
        // none of the nodes were found
        else {
            if(source == destination) {
                // bool can_be_adjoint = 0;
                nodeMap[source][destination] = 1;
                this->size++;
            }
            else {
                nodeMap[source][destination] = 1;
                nodeMap[destination];
                this->size += 2;
            }
        }

/*
        // VECTOR IMPLEMENTATION
        Node* temp_src = NULL, *temp_dest = NULL;
        int i = 0;
        for(i; i < nodeList.size(); i++) {
            //found source
            if(nodeList[i]->name == source) {
                temp_src = nodeList[i];
            }
            //found destination
            if(nodeList[i]->name == destination) {
                temp_dest = nodeList[i];
            }
        }
        // LAZY EVALUATION?
        // found both nodes
        if(temp_src && temp_dest) {
            temp_src->neighbors.push_back(temp_dest);
        } 
        // found only source
        else if(temp_src && !temp_dest) {
            temp_dest = new Node;
            temp_dest->name = destination;
            temp_src->neighbors.push_back(temp_dest);
            // add new vertex
            this->nodeList.push_back(temp_dest);
            this->size++;
        }
        // found only dest
        else if(temp_dest && !temp_src) {
            temp_src = new Node;
            temp_src->name = source;
            temp_src->neighbors.push_back(temp_dest);
            // add new vertex
            this->nodeList.push_back(temp_src);
            this->size++;
        }
        // none of the nodes found
        else {
            if(source == destination) {
                temp_src = new Node;
                temp_src->name = source;
                temp_src->neighbors.push_back(temp_src);
                // add new vertex
                this->nodeList.push_back(temp_src);
                this->size++;
            } else {
            temp_src = new Node;
            temp_src->name = source;
            temp_dest = new Node;
            temp_dest->name = destination;
            temp_src->neighbors.push_back(temp_dest);
            // add new vertex
            this->nodeList.push_back(temp_src);
            // add new vertex
            this->nodeList.push_back(temp_dest);
            this->size+=2;
            }
        }
*/
        this->num_of_edges++;
    };
    
    void printGraph() {
        cout << "This graph has " << nodeMap.size() << " vertices" << endl;
        cout << "This graph has " << num_of_edges << " edges" << endl;
        // MAP IMPLEMENTATION
        for(auto node : nodeMap) {
            cout << "\nVertex " << node.first << ":";
            for(auto neighbor : node.second){
                cout << "-> " << neighbor.first << "(x" << neighbor.second << ")";
            }
            cout << endl;
        }
/*
        VECTOR IMPLEMENTATION
        for (int d = 0; d < nodeList.size(); d++) {
            cout << "\nVertex " << nodeList[d]->name << ":";
            for(auto x : nodeList[d]->neighbors) {
                cout << "-> " << x->name;
            }
            cout << endl;
        }
*/
    }

    bool includes_vertex(string name) {
        auto it = nodeMap.find(name);

        if(it != nodeMap.end()) {
            cout << "This graph includes vertex " << it->first << endl;
            return 1;
        }
        cout << "This graph doesn't include vertex " << name << endl;
        return 0;
    }
/*
        // VECTOR IMPLEMENTATION
        for(auto x : nodeList) {
            if(x->name == name) {
                cout << "This graph includes vertex " << name << endl;
                return 1;
            }
        }
        cout << "This graph doesn't include vertex " << name << endl;
        return 0;
    }
*/
    
};