#include <iostream>
#include "./h/specimen.h"
#include "./h/random_instance_generator.h"
#include "./metaheuristics/GA_v2.h"
#include <windows.h>

using namespace std;

int main(int argc, char* argv[]) {
    // initialise randomness
    srand(time(NULL));

    Graph g = load_graph_from_file();

    // vector of results Specimen - population
    vector<Specimen> population;

    // ask for the wanted amount of Specimen
    int population_size = stoi(argv[1]);
    cout << "RUNNING PROGRAM WITH POPULATION SIZE " << population_size << endl;
/*
    cout << "What population size do you want? (int only!)" << endl;
    cin >> population_size;
    cin.clear();
*/
    // create the desired amount of Specimen
    for(int i = 0; i < population_size; i++) {
        Specimen s;
        s.initialise_S(g);
        s.find_path(g);
        s.grade_path(g);
        population.push_back(s);
    }

    genetic_algorithm(population, g);

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
  
    // population cleanup
/*
    for(auto el : population) {
        delete el;
    }
*/
    
    // cout << endl << "zrobione" << endl;
    return 1;
};

