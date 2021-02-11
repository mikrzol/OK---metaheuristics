// #include "mutation_v2.h"
#include "crossover_v2.h"
#include "selection_v2.h"
#include "../h/specimen.h"
#include <climits>

#pragma once
using namespace std;

void print_best_score(const vector<Specimen>& children) {
    int best_score = INT_MAX;
    for(auto el : children) {
        if(el.score < best_score) {
            best_score = el.score;
        }
    }
    cout << best_score << endl;
};

void genetic_algorithm(vector<Specimen>& parents, Graph& g) {
    // the size of the population
    int population_size = parents.size();
    // parent population created in main()
    ofstream out_file;
    out_file.open("./a.txt");

    out_file << "==============================================================" << endl;
    out_file << "PARENTS BEFORE GA:" << endl;
        for(auto parent : parents) {
            for(auto el : parent.S) {
                out_file << el << " ";
            }
            out_file << "\nPATH:" << endl;
            for(auto el : parent.path) {
                out_file << el->name << " ";
            }
            out_file << endl << "SCORE = " << parent.score;
            out_file << endl << endl;
        }
    out_file << "==============================================================" << endl;

/* CONSOLE PARENT PRINTING
    cout << "==============================================================" << endl;
    cout << "PRINTING PARENTS (BEFORE GA):" << endl;
        for(auto parent : parents) {
            for(auto el : parent->S) {
                cout << el << " ";
            }
            cout << "\nPATH:" << endl;
            for(auto el : parent->path) {
                cout << el->name << " ";
            }
            cout << endl << "SCORE = " << parent->score;
            cout << endl << endl;
        }
    cout << "==============================================================" << endl;
*/

    // while (not done - give it some time? x amount of iterations?) {
    for(int iteration = 0; iteration < 100; iteration++) {
        // create empty children population
        vector<Specimen> children;
        // !!! KEY PARAMETER - THE SIZE OF CHILDREN VECTOR
        while(children.size() != parents.size()) {
            // ==============================SELECTION==============================
            // !!! KEY PARAMETER - THE NUMBER OF CONTESTANTS IN THE TOURNAMENT
            int tournament_size = 2;
            Specimen* parent_1 = selection_tournament(parents, tournament_size);
            Specimen* parent_2 = selection_tournament(parents, tournament_size);

            // ==============================CROSSOVER==============================
            vector<Specimen> children_pair = PMX_crossover(*parent_1, *parent_2, g);

            // ==============================MUTATION==============================
            // mutation(children_pair, g, iteration);

            // finally, push the children into the children vector
            children.push_back(children_pair[0]);
            children.push_back(children_pair[1]);
        }
        print_best_score(children);
        // replace parents with children
        parents = children;
    } // (end for iteration loop)

    out_file << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    out_file << "PRINTING CHILDREN (AFTER GA):" << endl;
        for(auto parent : parents) {
            for(auto el : parent.S) {
                out_file << el << " ";
            }
            out_file << "\nPATH:" << endl;
            for(auto el : parent.path) {
                out_file << el->name << " ";
            }
            out_file << "\nSCORE = " << parent.score;
            out_file << endl << endl;
        }
    out_file << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

    out_file.close();

/* CONSOLE CHILDREN PRINTING
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "PRINTING CHILDREN (AFTER GA):" << endl;
        for(auto parent : parents) {
            unordered_map<int, bool> mapka;
            for(auto el : parent->S) {
                cout << el << " ";
                auto map_it = mapka.find(el);
                if(map_it == mapka.end()) {
                    mapka[el] = 1;
                } else {
                    cout << "FOUND DUPLICATE EL: " << el << endl;
                }
            }
            cout << "\nPATH:" << endl;
            for(auto el : parent->path) {
                cout << el->name << " ";
            }
            cout << "\nSCORE = " << parent->score;
            cout << endl << endl;
        }
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
*/
    // } (end while(not done))
};