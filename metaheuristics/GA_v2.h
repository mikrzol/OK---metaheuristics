#include "mutation_v2.h"
#include "crossover_v2.h"
#include "selection_v2.h"
#include "../h/specimen.h"
#include <climits>
#include <map>


#pragma once
using namespace std;

void print_average_score(const vector<Specimen>& children, int& iteration) {
    float score = 0.0;
    for(auto el : children) {
        score += el.score;
    }
    cout << iteration << ": " << score/children.size() << endl;
};

void print_best_score(const vector<Specimen>& children, int& iteration) {
    int best_score = INT_MAX;
    for(auto el : children) {
        if(el.score < best_score) {
            best_score = el.score;
        }
    }
    cout << iteration << ": " << best_score << endl;
};

void print_parents_to_file(const vector<Specimen>& parents, ofstream& out_file) {
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
};

void print_parents_to_console(const vector<Specimen>& parents) {
    cout << "==============================================================" << endl;
    cout << "PRINTING PARENTS (BEFORE GA):" << endl;
        for(auto parent : parents) {
            for(auto el : parent.S) {
                cout << el << " ";
            }
            cout << "\nPATH:" << endl;
            for(auto el : parent.path) {
                cout << el->name << " ";
            }
            cout << endl << "SCORE = " << parent.score;
            cout << endl << endl;
        }
    cout << "==============================================================" << endl;
};

void print_children_to_file(const vector<Specimen>& children, ofstream& out_file) {
    out_file << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    out_file << "CHILDREN (AFTER GA):" << endl;
        for(auto child : children) {
            for(auto el : child.S) {
                out_file << el << " ";
            }
            out_file << "\nPATH:" << endl;
            for(auto el : child.path) {
                out_file << el->name << " ";
            }
            out_file << "\nSCORE = " << child.score;
            out_file << endl << endl;
        }
    out_file << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
};

void print_children_to_console(const vector<Specimen>& children, ofstream& out_file) {
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "PRINTING CHILDREN (AFTER GA):" << endl;
        for(auto child : children) {
            unordered_map<int, bool> mapka;
            for(auto el : child.S) {
                cout << el << " ";
                auto map_it = mapka.find(el);
                if(map_it == mapka.end()) {
                    mapka[el] = 1;
                } else {
                    cout << "FOUND DUPLICATE EL: " << el << endl;
                }
            }
            cout << "\nPATH:" << endl;
            for(auto el : child.path) {
                cout << el->name << " ";
            }
            cout << "\nSCORE = " << child.score;
            cout << endl << endl;
        }
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}

void genetic_algorithm(vector<Specimen>& parents, Graph& g) {
    // the size of the population
    int population_size = parents.size();
    // parent population created in main()
    ofstream out_file;
    out_file.open("./test.txt");

    print_parents_to_file(parents, out_file);

    // while (not done - give it some time? x amount of iterations?) {
    for(int iteration = 0; iteration < 100; iteration++) {
        // create a census for this generation
        // specimen, amount
        map<vector<int>, int> census;
        for(auto parent : parents) {
            auto p_it = census.find(parent.S);
            if(p_it != census.end()) {
                p_it->second++;
            } else {
                census[parent.S] = 1;
            }
        }
        // create empty children population
        vector<Specimen> children;
        // !!! KEY PARAMETER - THE SIZE OF CHILDREN VECTOR
        while(children.size() != parents.size()) {
            // ==============================SELECTION==============================
            // !!! KEY PARAMETER - THE NUMBER OF CONTESTANTS IN THE TOURNAMENT
            int tournament_size = 2;
            Specimen* parent_1 = selection_tournament(parents, tournament_size, census);
            Specimen* parent_2 = selection_tournament(parents, tournament_size, census);

            // ==============================CROSSOVER==============================
            vector<Specimen> children_pair = PMX_crossover(*parent_1, *parent_2, g);

            //vector<Specimen> children_pair = OX_crossover(*parent_1, *parent_2, g);

            // ==============================MUTATION==============================
            mutation(children_pair, g, iteration, census);

            // finally, push the children into the children vector
            children.push_back(children_pair[0]);
            children.push_back(children_pair[1]);
        }

        print_average_score(children, iteration);
        //print_best_score(children, iteration);

        // replace parents with children
        parents = children;
    } // (end for iteration loop)

    print_children_to_file(parents, out_file);

    out_file.close();

    // } (end while(not done))
};