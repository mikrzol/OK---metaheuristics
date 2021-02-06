#include "../h/specimen.h"
#include <algorithm>

#pragma once

using namespace std;

Specimen* selection_tournament(vector<Specimen*>& contestants, int tournament_size) {
    Specimen* champion = NULL;
    for(int i = 0; i < tournament_size; i++) {
        // get a random index from 0 to contestants.size()
        int number = rand() % contestants.size();
        // find the contestant at the index number
        Specimen* contestant = contestants[number];

        // carry out the tournament
        if(champion == NULL || contestant->score < champion->score) {
            champion = contestant;
        }
    }
    return champion;
};