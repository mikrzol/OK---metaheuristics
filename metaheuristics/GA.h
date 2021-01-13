#include "mutation.h"
#include "crossover.h"
#include "selection.h"
#include "../graph.h"

#pragma once
using namespace std;

void genetic_algorithm(vector<pair<vector<Node*>, int>>& parents) {
    // the size of the population
    int population_size = parents.size();
    // parent population created in main()
    cout << "==============================================================" << endl;
    cout << "PRINTING PARENTS :" << endl;
        for(auto parent : parents) {
            for(auto el : parent.first) {
                cout << el->name << " ";
            }
            cout << endl << endl;
        }
    cout << "==============================================================" << endl;

    // while (not done - give it some time? x amount of iterations?) {
    for(int iteration = 0; iteration < 100; iteration++) {
        // create empty children population
        vector<vector<Node*>> children;
        // !!! KEY PARAMETER - THE SIZE OF CHILDREN VECTOR
        while(children.size() != parents.size()) {
            // ==============================SELECTION==============================
            // !!! KEY PARAMETER - THE NUMBER OF CONTESTANTS IN THE TOURNAMENT
            int tournament_size = 2;
            vector<Node*> parent_1 = selection_tournament(parents, tournament_size);
            vector<Node*> parent_2 = selection_tournament(parents, tournament_size);

            // ==============================CROSSOVER==============================
            pair<vector<Node*>, vector<Node*>> children_pair = crossover(parent_1, parent_2);
            vector<Node*> child_1, child_2;
            child_1 = children_pair.first;
            child_2 = children_pair.second;

            // ==============================MUTATION==============================
            // FOR NOW - DO NOTHING 

            // finally, push the children into the children vector
            children.push_back(child_1);
            children.push_back(child_2);
        }
        // replace parents with children
        for(int c = 0; c < parents.size(); c++) {
            parents[c].first = children[c];
        }
    }

    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "PRINTING CHILDREN :" << endl;
        for(auto parent : parents) {
            for(auto el : parent.first) {
                cout << el->name << " ";
            }
            cout << endl << endl;
        }
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
/*
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "PRINTING CHILDREN :" << endl;
        for(auto child : children) {
            for(auto el : child) {
                cout << el->name << " ";
            }
            cout << endl << endl;
        }
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
*/
    // } (end while(not done))
};