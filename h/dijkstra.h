#pragma once
#include <iostream>
#include <vector>
#include "./graph.h"
#include "./pq.h"

using namespace std;

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
        sp.push_back(prev[curr-1]);

        vector<Node*> asp;
        for(int i = sp.size()-2; i > -1; i--) {
            asp.push_back(sp[i]);
        }

        return asp;
    }

};

Shortest_Path dijkstra(Graph& g, int source, int target) {
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

    // return the shorest path distance and prev vectors
    Shortest_Path sp = Shortest_Path();
    sp.distance = distance;
    sp.prev = prev;
    return sp; 
};

void dijkstra_global(Graph& g, int source) {
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
};