#pragma once
#include "mutation_v2.h"
#include "crossover_v2.h"
#include "selection_v2.h"
#include "../h/specimen.h"
#include <climits>
#include <map>
#include "GA_extra_methods.h"

using namespace std;

void genetic_algorithm(vector<Specimen>& parents, Graph& g) {
    // the size of the population
    int population_size = parents.size();
    // parent population created in main()
    ofstream out_file;
    // !!! IMPORTANT - USE FULL PATH TO THE LOG.TXT FILE !!!
    out_file.open("E:\\OK\\log.txt");

    print_parents_to_file(parents, out_file);

    // !!! KEY PARAMETER - NUMBER OF ITERATIONS FOR GA !!!
    for(int iteration = 0; iteration < 500; iteration++) {
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
};