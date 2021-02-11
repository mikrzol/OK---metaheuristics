#include "../h/specimen.h"

#pragma once

using namespace std;

vector<unordered_map<int, int>> swap_between(Specimen* p1, Specimen* p2, int larger, int smaller) {
    // create a map for the parents
    // map<name, index>
    unordered_map<int, int> p1_map;
    unordered_map<int, int> p2_map;

    // get temp vector
    int c = 0;
    vector<int> temp;
    for(int i = smaller; i < larger; i++) {
        temp.push_back(p1->S[i]);
        p2_map[p1->S[i]] = c+smaller;
        c++;
    }

    // swap the elements
    c = 0;
    for(int i = smaller; i < larger; i++) {
        p1->S[i] = p2->S[i];
        p1_map[p2->S[i]] = smaller+c;
        p2->S[i] = temp[c];
        c++;
    }

    // return the maps
    vector<unordered_map<int, int>> maps;
    maps.push_back(p1_map);
    maps.push_back(p2_map);
    return maps;
}

int PMX_resolve_conflict(int conflict_int, Specimen* p1, Specimen* p2, int& larger, int& smaller, vector<std::unordered_map<int, int>> maps, int& which_map, int &conflict_idx) {
    for(int c = 0; c < p1->S.size(); c++) {
        if(c == conflict_idx) {
            continue;
        }
        if(p1->S[c] == conflict_int) {
            // get the int that's paired to the conflict one
            int paired_to_conflict = p2->S[c];
            // see if it doesn't cause conflicts
            // !!! NOT JUST IN MAP REGION !!!
            for(int node = 0; node < p1->S.size(); node++) {
                if(p1->S[node] == paired_to_conflict) {
                    paired_to_conflict = p2->S[c];
                    return PMX_resolve_conflict(paired_to_conflict, p1, p2, larger, smaller, maps, which_map, conflict_idx);
                }
            }
            return paired_to_conflict;
/*
            auto p_to_conflict_it = maps[which_map].find(paired_to_conflict);
            // down - it causes conflict
            if(p_to_conflict_it != maps[which_map].end()) {
                return PMX_resolve_conflict(paired_to_conflict, p1, p2, larger, smaller, maps, which_map);
            } else {
                return paired_to_conflict;
            }
*/
        }
    }
};

void PMX_conflict_solver(Specimen* curr_parent, Specimen* other_parent, int& larger, int& smaller, vector<std::unordered_map<int, int>> maps, int& which_map) {
    // keep track of the conflict idxes to return to them quickly
    vector<int> conflict_idxes;
    // find conflicting elements for curr_parent
    for(int i = 0; i < curr_parent->S.size(); i++) {
        // check just the parts outside the exchanged part
        if(i < smaller || i >= larger) {
            // look for the el in the map[which_map]
            auto el_it = maps[which_map].find(curr_parent->S[i]);
            // if an element is in the map, there's a conflict
            if(el_it != maps[which_map].end()){
                conflict_idxes.push_back(i);
            }
        }
    }

    // resolve the conflicts for curr_parent
    for(int i : conflict_idxes) {
        int conflict_int = curr_parent->S[i];
        // find the int that won't cause any problems
        int correct_int = PMX_resolve_conflict(conflict_int, curr_parent, other_parent, larger, smaller, maps, which_map, i);
        // finally, put the correct_int to S[i] position
        curr_parent->S[i] = correct_int;
    }
};

// PMX crossover algorithm
vector<Specimen*> PMX_crossover(Specimen* parent_1, Specimen* parent_2, Graph& g) {
    // 1. choose 2 random cut points on parents
    //int a = 3;
    //int b = 6;
    int a = rand() % parent_1->S.size();
    int b = rand() % parent_1->S.size(); // doesn't matter from which parent - both S are of the same size
    // determine which is larger
    int larger, smaller;
    (a > b) ? larger = a, smaller = b : larger = b, smaller = a;

    // 2. exchange info between the cutting points across the parents
    auto maps = swap_between(parent_1, parent_2, larger, smaller);
/*
cout << "PARENTS AFTER SWAPPING:\n\tPARENT 1:\n\t";
for(auto el : parent_1->S) {
    cout << el << " ";
}
cout << "\n\tPARENT 2:\n\t";
for(auto el : parent_2->S) {
    cout << el << " ";
}
cout << endl;
*/

    // 3. fill further bits (from original parents) that have no conflicts
    // use maps[x] to find if an el has already been used - then it's a conflict
    
    int which_map = 0;
    // solve all conflicts for parent_1
    PMX_conflict_solver(parent_1, parent_2, larger, smaller, maps, which_map);
    // ! update which map to use !
    which_map = 1;
    // solve all conflicts for parent_2
    PMX_conflict_solver(parent_2, parent_1, larger, smaller, maps, which_map);

/*
cout << "PARENTS AFTER CONFLICT SOLVING:\n\tPARENT 1:\n\t";
for(auto el : parent_1->S) {
    cout << el << " ";
}
cout << "\n\tPARENT 2:\n\t";
for(auto el : parent_2->S) {
    cout << el << " ";
}
cout << endl;
*/

    // find new paths for the crossovered parents and grade the new traversal
    parent_1->find_path(g);
    parent_1->grade_path(g);
    parent_2->find_path(g);
    parent_2->grade_path(g);

    vector<Specimen*> children;
    children.push_back(parent_1);
    children.push_back(parent_2);
    return children;
}