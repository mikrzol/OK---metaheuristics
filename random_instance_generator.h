#include "graph.h"

using namespace std;

bool has_hit_edge_cap(const int& vertex, Graph&g, const int& max_edges) {
    // check if vertex Node has hit the edge cap already
        // find the node of name src
        auto vertex_it = g.node_map.find(vertex);
        auto vertex_node = vertex_it->second;
        // if src_node has NOT hit the edge cap yet
        if(vertex_node->num_of_edges < max_edges) {
            return 0;
        } else {
            return 1;
        }
};

void find_destination(Graph& g, const int& src, const int& weight, const int& V_number, const int& max_edges, const int& max_weight, int& tries) {
    //if(tries < 4) {
        // find the name of the vertex the edge will go to (random)
        unsigned int destination = rand() % V_number+1; // +1 because naming starts from 1
/*
// find the node named destination
auto dest_it = g.node_map.find(destination);
cout << "\tdest->second->name" << dest_it->second->name << "\tdest->second->num_of_edges = " << dest_it->second->num_of_edges << endl;
*/

        // check the degree of the destination vertex
        if(!has_hit_edge_cap(destination, g, max_edges)) {
            // add the edge based on the above parameters
            g.add_edge(src, destination, weight);
            return;
        } else {
            // find a new destination Node (return to destination determining step)
            tries++;
            return find_destination(g, src, weight, V_number, max_edges, max_weight, tries);
            return;
        }
    //}
    //else {
        //cout << "Too many tries" << endl;
        //return;
    //}
};

Graph generate_random_instance() {
    // initialise randomness
    srand(time(NULL));
    // !!! KEY PARAMETERS FOR INSTANCE GENERATION ARE BELOW
    // number of vertices
    unsigned int V_number = 100;
    // max number of Edges for each vertex
    unsigned int max_edges = 6;
    // max number of Arcs for each vertex
    unsigned int max_arcs = 3; // 3 not 2 because of how the modulo operation works (x % 3 gives 0, 1 or 2)
    // max weight for Edge/Arc
    unsigned int max_weight = 100;

    // 1. create a new Graph object
    // 2. create V_number vertices in the Graph
    Graph g = Graph(V_number);

    // 3. add the Edges and Arcs to the Graph
    // loop over all of the nodes in the graph
    for(int src = 1; src < V_number+1; src++) {
        // ================================ EDGES PART ================================
        // check if src Node has hit the edge cap already
        // if src_node has NOT hit the edge cap yet
        if(!has_hit_edge_cap(src, g, max_edges)) {
            // randomly determine the number of edges for this vertex
            unsigned int v_degree_edges = rand() % max_edges +1; // +1 because deg(v,e) = [1,6]
            // create this amount of edges to random other nodes
            for(int e = 0; e < v_degree_edges; e++) {
                if(!has_hit_edge_cap(src, g, max_edges)) {
                    // randomly generate the weight of the edge
                    unsigned int weight = rand() % max_weight +1; // +1 because wi = [1,100]
                    // hold the number of tries
                    int tries = 0;
                    find_destination(g, src, weight, V_number, max_edges, max_weight, tries);
                } else {
                    break;
                }
            }
        }

        // ================================ ARCS PART ================================
        // randomly determine the number of arcs for this vertex
        unsigned int v_degree_arcs = rand() % max_arcs; // +0 becuase deg(v,a) = [0,2]
        // create this amount of arcs to random other nodes
        for(int a = 0; a < v_degree_arcs; a++) {
            // find the name of the vertex the arc will go to (random)
            unsigned int destination = rand() % V_number+1; // +1 because naming starts from 1
            // randomly determine the weight of the arc
            unsigned int weight = rand() % max_weight +1; // +1 because wi = [1,100]
            // add the arc based on the above parameters
            g.add_arc(src, destination, weight);
        }
    }
    return g;
};