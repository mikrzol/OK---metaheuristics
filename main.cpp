#include <iostream>
#include "./h/graph.h"
#include "./h/random_instance_generator.h"
#include "./metaheuristics/GA.h"
#include <windows.h>

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
    // initialise randomness
    srand(time(NULL));

    Graph g = load_graph_from_file();
    //g.print_graph();

    // vector of results
    vector<pair<vector<Node*>, int>> results_population;

    int population_size;
    cout << "What population size do you want? (int only!)" << endl;
    cin >> population_size;
    cin.clear();

    // create a population of results
    for(int i = 0; i < population_size; i++) {
        pair<vector<Node*>, int> result = g.traverse_graph_naive();
        cout << "result cost = " << result.second << endl;
        results_population.push_back(result);
    }
    cout << "Population size = " << results_population.size() << endl;
    
    genetic_algorithm(results_population);

/*
    // create a new graph
    Graph g = generate_random_instance();

    // naively traverse the graph
    pair<vector<Node*>, int> graded_path = g.traverse_graph_naive();

    string input;
    cout << "Save graph to file? (yes/ no)" << endl;
    cin >> input;
    cin.clear();

    if(input == "yes") {
        g.save_graph_to_file();
    }
*/

/*
    // add 9 nodes to the graph
    for(int i = 1; i < 10; i++) {
        g.add_node(i);
    }

    // add example edges to the graph
    add_example_edges(g);

    // print the graph
    //g.print_graph();

    
*/
    
    cout << endl << "zrobione" << endl;
    return 1;
};

