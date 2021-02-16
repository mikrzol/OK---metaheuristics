#include "../h/specimen.h"

#pragma once

using namespace std;

tuple<vector<Specimen>, vector<unordered_map<int, int>>> swap_between(const Specimen& p1_orig, const Specimen& p2_orig, int larger, int smaller) {
    // create a map for the parents
    // map<name, index>
    unordered_map<int, int> p1_map;
    unordered_map<int, int> p2_map;

    auto p1 = p1_orig;
    auto p2 = p2_orig;

    // get temp vector
    int c = 0;
    vector<int> temp;
    for(int i = smaller; i < larger; i++) {
        temp.push_back(p1.S[i]);
        p2_map[p1.S[i]] = c+smaller;
        c++;
    }

    // swap the elements
    c = 0;
    for(int i = smaller; i < larger; i++) {
        p1.S[i] = p2.S[i];
        p1_map[p2.S[i]] = smaller+c;
        p2.S[i] = temp[c];
        c++;
    }

    // return the maps
    vector<unordered_map<int, int>> maps;
    maps.push_back(p1_map);
    maps.push_back(p2_map);
    vector<Specimen> s;
    s.push_back(p1);
    s.push_back(p2);
    auto pair = make_pair<>(s, maps);
    return pair;
}

int PMX_resolve_conflict(int conflict_int, Specimen& p1, Specimen& p2, int& larger, int& smaller, vector<std::unordered_map<int, int>> maps, int& which_map, int &conflict_idx) {
    for(int c = 0; c < p1.S.size(); c++) {
        if(c == conflict_idx) {
            continue;
        }
        if(p1.S[c] == conflict_int) {
            // get the int that's paired to the conflict one
            int paired_to_conflict = p2.S[c];
            // see if it doesn't cause conflicts
            // !!! NOT JUST IN MAP REGION !!!
            for(int node = 0; node < p1.S.size(); node++) {
                if(p1.S[node] == paired_to_conflict) {
                    paired_to_conflict = p2.S[c];
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

void PMX_conflict_solver(Specimen& curr_parent, Specimen& other_parent, int& larger, int& smaller, vector<std::unordered_map<int, int>> maps, int& which_map) {
    // keep track of the conflict idxes to return to them quickly
    vector<int> conflict_idxes;
    // find conflicting elements for curr_parent
    for(int i = 0; i < curr_parent.S.size(); i++) {
        // check just the parts outside the exchanged part
        if(i < smaller || i >= larger) {
            // look for the el in the map[which_map]
            auto el_it = maps[which_map].find(curr_parent.S[i]);
            // if an element is in the map, there's a conflict
            if(el_it != maps[which_map].end()){
                conflict_idxes.push_back(i);
            }
        }
    }

    // resolve the conflicts for curr_parent
    for(int i : conflict_idxes) {
        int conflict_int = curr_parent.S[i];
        // find the int that won't cause any problems
        int correct_int = PMX_resolve_conflict(conflict_int, curr_parent, other_parent, larger, smaller, maps, which_map, i);
        // finally, put the correct_int to S[i] position
        curr_parent.S[i] = correct_int;
    }
};

// PMX crossover algorithm
vector<Specimen> PMX_crossover(const Specimen& parent_1, const Specimen& parent_2, Graph& g) {
    // 1. choose 2 random cut points on parents
    //int a = 3;
    //int b = 6;
    int a = rand() % parent_1.S.size();
    int b = rand() % parent_1.S.size(); // doesn't matter from which parent - both S are of the same size
    // determine which is larger
    int larger, smaller;
    (a > b) ? larger = a, smaller = b : larger = b, smaller = a;

    // Specimen child_1 = Specimen(); 
    // Specimen child_2 = Specimen();

    // 2. exchange info between the cutting points across the parents
    auto children_and_maps = swap_between(parent_1, parent_2, larger, smaller);
    auto maps = get<1>(children_and_maps);
    auto children = get<0>(children_and_maps);
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
    // NO NEED TO DO THE UPPER PART - CONFLICTS ARE FOUND IN PMX_conflict_solver
    // use maps[x] to find which els have been crossed over - then it's a conflict
    
    int which_map = 0;
    // solve all conflicts for parent_1
    PMX_conflict_solver(children[0], children[1], larger, smaller, maps, which_map);
    // ! update which map to use !
    which_map = 1;
    // solve all conflicts for parent_2
    PMX_conflict_solver(children[1], children[0], larger, smaller, maps, which_map);

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
    children[0].find_path(g);
    children[0].grade_path(g);
    children[1].find_path(g);
    children[1].grade_path(g);

    return children;
};

bool OX_check(const Specimen& parent, const int& look_up_idx, const unordered_map<int, bool>& map) {
    // see if the element we're trying to put in is already in the map
    auto el_it = map.find(parent.S[look_up_idx]);
    if(el_it != map.end()) {
        return 0;
    } else {
        return 1;
    }
};

void OX_fill(const Specimen& parent, Specimen& child, int& look_up_idx, int& input_idx, const int& smaller, const int& size, const unordered_map<int, bool>& map) {
    while(input_idx != smaller) {

        while(!OX_check(parent, look_up_idx, map)){
            (look_up_idx+1 == size) ? look_up_idx = 0 : look_up_idx++;
        }
        // put in the elements at appriopriate idx
        child.S[input_idx] = parent.S[look_up_idx];

        // update input_idx
        (input_idx+1 == size) ? input_idx = 0 : input_idx++;
        // update look_up_idx
        (look_up_idx+1 == size) ? look_up_idx = 0 : look_up_idx++;
    }
};

void OX_randomise_b(int& b, const Specimen& parent_1) {
    b = rand() % parent_1.S.size();
};

vector<Specimen> OX_crossover(const Specimen& parent_1, const Specimen& parent_2, Graph& g) {
    // 1. choose 2 random cut points on parents
    int a = rand() % parent_1.S.size();
    int b = rand() % parent_1.S.size(); // doesn't matter from which parent - both S are of the same size
    //int a = 0;
    //int b = 3;
    // determine which is larger
    while(a == b) {
        OX_randomise_b(b, parent_1);
    }
    int larger, smaller;
    if(a > b) {
        larger = a;
        smaller = b;
    } else {
        larger = b;
        smaller = a;
    }

    // create children and initialize vectors of correct size in them
    Specimen child_1 = Specimen(); 
    child_1.S = vector<int>(parent_1.S.size());
    Specimen child_2 = Specimen();
    child_2.S = vector<int>(parent_2.S.size());

    // maps hold info on which elements were between the cutting points (for fast checking in step 3.)
    unordered_map<int, bool> map1;
    unordered_map<int, bool> map2;

    // 2. put elements in range from smaller to larger from parents to correct children (no exchange here)
    for(int i = smaller; i < larger; i++) {
        child_1.S[i] = parent_1.S[i];
        map1[parent_1.S[i]] = 1;
        child_2.S[i] = parent_2.S[i];
        map2[parent_2.S[i]] = 1;
    }

    // 3. starting from larger position, put non-conflicting elements from P2 to C1 and from P1 to C2
    int input_idx = larger;
    int look_up_idx = larger;
    int size = parent_1.S.size();
    // fill child_1
    OX_fill(parent_2, child_1, look_up_idx, input_idx, smaller, size, map1);

    // reset the ints
    input_idx = larger;
    look_up_idx = larger;
    // fill child_2
    OX_fill(parent_1, child_2, look_up_idx, input_idx, smaller, size, map2);

/*
cout << "LARGER = " << larger << endl;
cout << "SMALLER = " << smaller << endl;
cout << "PARENT_1:" << endl;
for(auto el : parent_1.S) {
    cout << el << " ";
}
cout << endl;
cout << "PARENT_2:" << endl;
for(auto el : parent_2.S) {
    cout << el << " ";
}
cout << endl;

cout << "CHILD_1:" << endl;
for(auto el : child_1.S) {
    cout << el << " ";
};
cout << endl;
cout << "CHILD_2:" << endl;
for(auto el : child_2.S) {
    cout << el << " ";
};
cout << endl;
*/

    // find path, grade it and return the children
    child_1.find_path(g);
    child_1.grade_path(g);
    child_2.find_path(g);
    child_2.grade_path(g);
    vector<Specimen> children;
    children.push_back(child_1);
    children.push_back(child_2);
    return children;
};