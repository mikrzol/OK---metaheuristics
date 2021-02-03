#include "../h/graph.h"

#pragma once

using namespace std;

pair<vector<Node*>,vector<Node*>> crossover(vector<Node*> parent_1, vector<Node*> parent_2) {
    // create temp vector just for swapping the info between the parents
    vector<Node*> temp;

    // compare the sizes of the parents to determine the smaller one
    int smaller_size = (parent_1.size() < parent_2.size()) ? parent_1.size() : parent_2.size();

    // get a random size of the cut (ranging from 1 to smaller_size inclusive)
    int cut_size = rand() % smaller_size + 1;

    // !!! SUPER NAIVE - TEMPORARY SOLUTION !!! 
    // put 10 first elements from parent_1 to temp
    for(int i = 0; i < 10; i++) {
        temp.push_back(parent_1[i]);
    }
    // change the first 10 items in parent_1 to the items from parent_2
    for(int i = 0; i < 10; i++) {
        parent_1[i] = parent_2[i];
    }
    // change the first 10 items in parent_2 to the items from temp
    for(int i = 0; i < 10; i++) {
        parent_2[i] = temp[i];
    }
    pair<vector<Node*>, vector<Node*>> children = make_pair(parent_1, parent_2);
    return children;
}