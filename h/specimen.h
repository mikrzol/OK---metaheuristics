#pragma once
#include "graph.h"

using namespace std;

// Specimen struct
// this struct contains information about one generated specimen to use for GA
struct Specimen {
    // a permutation of traversal through all the vertices of the Graph (will be changed in GA crossover/mutation) - unique elements, order matters (is crucial)
    vector<int> S;
    // actual path of the traversal "under the mask" - based on dijkstra-ing between the nodes in S in order
    vector<Node*> path;
    // cost of the path Graph traversal
    int score;

    // METHODS
    void initialise_S(Graph& g);
    // shuffle S to randomise it
    void shuffle_S();
    // create path from S
    void find_path(Graph& g);
    // swap elements in S
    void swap(int* a, int* b);
    // determine the score of path Graph traversal
    void grade_path(Graph& g); 
};

void Specimen::initialise_S(Graph& g) {
    for(auto el : g.node_map) {
        this->S.push_back(el.first);
    }
    this->shuffle_S();
};

void Specimen::shuffle_S() {
    for(auto el : this->S) {
        int a = rand() % this->S.size();
        int b = rand() % this->S.size();
        this->swap(&S[a], &S[b]);
    }
};

void Specimen::swap(int* a, int* b) {
    int temp = *b;
    *b = *a;
    *a = temp;
};

void Specimen::find_path(Graph& g) {
    vector<Node*> new_path;
    // put the element at index 0 to path
    int i = 0;
    auto n_it = g.node_map.find(this->S[i]);
    new_path.push_back(n_it->second);
    i++;
    for(i; i < this->S.size(); i++) {
        vector<Node*> partial_path = dijkstra(g, S[i-1], S[i]);
        for(auto el : partial_path) {
            new_path.push_back(el);
        }
    }
    this->path = new_path;
};

void Specimen::grade_path(Graph& g) {
    this->score = g.grade_traversal(this->path);
};