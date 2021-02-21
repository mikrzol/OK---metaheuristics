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

Graph generate_random_instance(int size) {
    // !!! KEY PARAMETERS FOR INSTANCE GENERATION ARE BELOW
    // number of vertices
    unsigned int V_number = size;
    // max number of Edges for each vertex (default - 6)
    unsigned int max_edges = 6;
    // max number of Arcs for each vertex (default - 2 +1)
    unsigned int max_arcs = 3; // 3 not 2 because of how the modulo operation works (x % 3 gives 0, 1 or 2)
    // max weight for Edge/Arc
    unsigned int max_weight = 200;

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
            // with dynamically decreased probability of having bigger amount of edges
            int sum = 0;
            for(int s = 1; s <= max_edges; s++) {
                sum += s;
            }
            unsigned int roll = rand() % sum +1; // +1 because deg(v,e) = [1,6]
            int step = 1;
            for(int part_sum = 1; part_sum <= sum; part_sum += step) {
                if(roll <= part_sum) {
                    break;
                }
                step++;
            }
            // need to "flip" the v_degree_edges, so the probability goes down, not up as the numer increases
            vector<int> flipper;
            for(int p = 0; p < max_edges; p++) {
                flipper.push_back(p+1);
            }
            // finally determine the v_degree_edges
            unsigned int v_degree_edges = flipper[flipper.size()-step];
/*
if(src == 1) {
    cout << "ROLL = " << roll << "\tSTEP = " << step << endl;
    for(auto elem : flipper) {
        cout << elem << " ";
    } 
    cout << endl;
    cout << "v_degree_edges = " << v_degree_edges << endl;
}
*/
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

    // print graph
/*
    for(auto el : g.node_map) {
        cout << el.first << " ";
    }
    cout << endl;
*/
    //g.DFS(g.node_map[0]);
    int start = 1;
    auto distances = dijkstra_global(g, start);

    for(auto el : distances) {
        if(el > max_weight && el+1 != start) {
            cout << "!!! FOUND AN ELEMENT WITH DISTANCE 0 !!!" << endl;
            for(int i = 0; i < distances.size(); i++) {
                cout << i+1 << ": " << distances[i] << endl;
            }
        }
    }

    // adjust the graph so it's guaranteed to be connected
    for(int el = 0; el < distances.size(); el++) {
        // if a distance from the start to a given vertex is 0, it needs to be connected to the graph
        if(distances[el] > max_weight && el+1 != start) {
            // find a candidate for connection - the first node that has not hit the edge cap 
            for(auto node : g.node_map) {
                if(!has_hit_edge_cap(node.first, g, max_edges)) {
                    // if the el+1 vertex has NOT hit the edge cap yet
                    if(!has_hit_edge_cap(el+1, g, max_edges)) {
                        // add a new edge to node.first
                        int weight = rand() % max_weight +1;
                        g.add_edge(el+1, node.first, weight);
                        // need to re-run the dijkstra_global because adding one connection might've solved the problem (connected multiple other vertices through this one)
                        distances = dijkstra_global(g, start);
                    } else {
                        // change one connection from el+1 to node.first
                        // find the el+1 node in node_map 
                        auto el_it = g.node_map.find(el+1);
                        // find its neighbors
                        auto el_neighs = g.neighborhood_map.find(el_it->second);
                        // find the first edge to change its destination to node.second
                        for(int el_i = 0; el_i < el_neighs->second.size(); el_i++){
                            if(el_neighs->second[el_i]->is_edge) {
                                // finally change the connection - can keep the weight
                                el_neighs->second[el_i]->destination = node.second;
                                // re-run dijkstra_global
                                distances = dijkstra_global(g, start);
                                // break out of the loop so it doesn't update other connections
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    for(auto el : distances) {
        if(el > max_weight && el+1 != start) {
            cout << "!!! FOUND AN ELEMENT WITH DISTANCE 0 !!!" << endl;
            for(int i = 0; i < distances.size(); i++) {
                cout << i+1 << ": " << distances[i] << endl;
            }
        }
    }

    return g;
};