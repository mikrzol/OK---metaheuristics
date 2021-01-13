#include "../graph.h"
#include <algorithm>

#pragma once

using namespace std;

vector<Node*> selection_tournament(vector<pair<vector<Node*>, int>>& contestants, int tournament_size) {
    pair<vector<Node*>, int> champion;
    for(int i = 0; i < tournament_size; i++) {
        // get a random index from 0 to contestants.size()
        int number = rand() % contestants.size();
        // find the contestant at the index number
        pair<vector<Node*>, int> contestant = contestants[number];

        // carry out the tournament
        if(champion.first.size() == 0 || contestant.second < champion.second) {
            champion = contestant;
        }
    }
    return champion.first;
};