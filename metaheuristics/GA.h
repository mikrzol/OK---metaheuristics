#include "mutation.h"
#include "crossover.h"
#include "selection.h"
#include "../h/specimen.h"

#pragma once
using namespace std;

void genetic_algorithm(vector<Specimen*>& parents) {
    // the size of the population
    int population_size = parents.size();
    // parent population created in main()
    cout << "==============================================================" << endl;
    cout << "PRINTING PARENTS :" << endl;
        for(auto parent : parents) {
            for(auto el : parent->path) {
                cout << el->name << " ";
            }
            cout << endl << endl;
        }
    cout << "==============================================================" << endl;

    cout << "DONE FOR NOW" << endl;
    return;

    // while (not done - give it some time? x amount of iterations?) {
    for(int iteration = 0; iteration < 100; iteration++) {
        // create empty children population
        vector<Specimen*> children;
        // !!! KEY PARAMETER - THE SIZE OF CHILDREN VECTOR
        while(children.size() != parents.size()) {
            // ==============================SELECTION==============================
            // !!! KEY PARAMETER - THE NUMBER OF CONTESTANTS IN THE TOURNAMENT
            int tournament_size = 2;
            Specimen* parent_1 = selection_tournament(parents, tournament_size);
            Specimen* parent_2 = selection_tournament(parents, tournament_size);

            // ==============================CROSSOVER==============================
            vector<Specimen*> children_pair = PMX_crossover(parent_1, parent_2);
            

            // ==============================MUTATION==============================
            // FOR NOW - DO NOTHING 

            // finally, push the children into the children vector
            children.push_back(children_pair[0]);
            children.push_back(children_pair[1]);
        }
        // replace parents with children
        for(int c = 0; c < parents.size(); c++) {
            parents[c] = children[c];
        }
    }

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "PRINTING CHILDREN :" << endl;
        for(auto parent : parents) {
            for(auto el : parent->path) {
                cout << el->name << " ";
            }
            cout << endl << endl;
        }
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    // } (end while(not done))
};