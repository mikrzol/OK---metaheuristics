#include "../h/specimen.h"

#pragma once

using namespace std;

vector<unordered_map<int, bool>> swap_between(Specimen* p1, Specimen* p2, int larger, int smaller) {
    // create a map for the parents
    unordered_map<int, bool> p1_map;
    unordered_map<int, bool> p2_map;

    // get temp vector
    vector<int> temp;
    for(int i = smaller; i < larger; i++) {
        temp.push_back(p1->S[i]);
        p1_map[i] = 1;
    }

    // swap the elements
    int c = 0;
    for(int i = smaller; i < larger; i++) {
        p1->S[i] = p2->S[i];
        p2_map[i] = 1;
        p2->S[i] = temp[c];
        c++;
    }

    // return the maps
    vector<unordered_map<int, bool>> maps;
    maps.push_back(p1_map);
    maps.push_back(p2_map);
    return maps;
}

// PMX crossover algorithm
vector<Specimen*> PMX_crossover(Specimen* parent_1, Specimen* parent_2) {
    // 1. choose 2 random cut points on parents
    int a = rand() % parent_1->S.size();
    int b = rand() % parent_1->S.size(); // doesn't matter from which parent - both S are of the same size
    // determine which is larger
    int larger, smaller;
    (a > b) ? larger = a, smaller = b : larger = b, smaller = a;

    // 2. exchange info between the cutting points across the parents
    auto maps = swap_between(parent_1, parent_2, larger, smaller);

    // 3. fill further bits (from original parents) that have no conflicts
    // use maps[x] to find if an el has already been used - then it's a conflict
    // keep track of the conflict idxes to return to them quickly
    vector<int> conflict_idxes;
}