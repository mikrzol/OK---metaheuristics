#include "../h/specimen.h"
#include <algorithm>
#include <climits>
#include <map>

#pragma once

using namespace std;

Specimen* selection_tournament(vector<Specimen>& contestants, int tournament_size, map<vector<int>, int>& census) {
    Specimen* champion = NULL;
    float champion_score = INT_MAX;
    for(int i = 0; i < tournament_size; i++) {
        // get a random index from 0 to contestants.size()
        int number = rand() % contestants.size();
        // find the contestant at the index number
        Specimen* contestant = &contestants[number];

        // carry out the tournament
        auto c_it = census.find(contestant->S);
        // !!! KEY PARAMETER - PENALTY FOR SAME PARENTS (0.01) !!!
        float contestant_score = (1.0 + c_it->second * 0.01) * contestant->score;
        if(champion == NULL || contestant->score < champion_score) {
            champion = contestant;
            champion_score = contestant_score;
        }
    }
    return champion;
};