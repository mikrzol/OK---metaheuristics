#include "../h/specimen.h"
#include <map>
#pragma once

using namespace std;

int generate_random(int size) {
    return rand() % size;
};

void mutate(Specimen& child, Graph& g) {
    // positions to mutate
    int position_1 = rand() % child.S.size();
    int position_2 = rand() % child.S.size();
    while(position_1 == position_2) {
        position_2 = generate_random(child.S.size());
    }
    // swap two nodes in child
    int temp = child.S[position_1];
    child.S[position_1] = child.S[position_2];
    child.S[position_2] = temp;

    // rebuild path and rescore it for the child
    child.find_path(g);
    child.grade_path(g);
}

void mutation(vector<Specimen>& children, Graph& g, int iteration, map<vector<int>, int>& census) {
    for(auto child : children) {
        auto child_S = census.find(child.S);
        if(child_S != census.end()) {
            int die_roll = rand() % ( (iteration + 1) * 10 / 5) ;
            if(!die_roll) {
                mutate(child, g);
            }
        } else {
            // fixed mutation rate
            // int die_roll = rand() % 99;
            // !!! need to decrease the chances of mutation with time !!!
            int die_roll = rand() % ( (iteration + 1) * 10);
            if(!die_roll) {
                mutate(child, g);
            }
        }
    }
};