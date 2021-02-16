#pragma once
#include "./GA_v2.h"
using namespace std;

void print_average_score(const vector<Specimen>& children, int& iteration) {
    float score = 0.0;
    for(auto el : children) {
        score += el.score;
    }
    cout << iteration << ": " << score/children.size() << endl;
};

void print_best_score(const vector<Specimen>& children, int& iteration) {
    int best_score = INT_MAX;
    for(auto el : children) {
        if(el.score < best_score) {
            best_score = el.score;
        }
    }
    cout << iteration << ": " << best_score << endl;
};

void print_parents_to_file(const vector<Specimen>& parents, ofstream& out_file) {
    out_file << "==============================================================" << endl;
    out_file << "PARENTS BEFORE GA:" << endl;
        for(auto parent : parents) {
            for(auto el : parent.S) {
                out_file << el << " ";
            }
            out_file << "\nPATH:" << endl;
            for(auto el : parent.path) {
                out_file << el->name << " ";
            }
            out_file << endl << "SCORE = " << parent.score;
            out_file << endl << endl;
        }
    out_file << "==============================================================" << endl;
};

void print_parents_to_console(const vector<Specimen>& parents) {
    cout << "==============================================================" << endl;
    cout << "PRINTING PARENTS (BEFORE GA):" << endl;
        for(auto parent : parents) {
            for(auto el : parent.S) {
                cout << el << " ";
            }
            cout << "\nPATH:" << endl;
            for(auto el : parent.path) {
                cout << el->name << " ";
            }
            cout << endl << "SCORE = " << parent.score;
            cout << endl << endl;
        }
    cout << "==============================================================" << endl;
};

void print_children_to_file(const vector<Specimen>& children, ofstream& out_file) {
    out_file << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    out_file << "CHILDREN (AFTER GA):" << endl;
        for(auto child : children) {
            for(auto el : child.S) {
                out_file << el << " ";
            }
            out_file << "\nPATH:" << endl;
            for(auto el : child.path) {
                out_file << el->name << " ";
            }
            out_file << "\nSCORE = " << child.score;
            out_file << endl << endl;
        }
    out_file << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
};

void print_children_to_console(const vector<Specimen>& children, ofstream& out_file) {
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "PRINTING CHILDREN (AFTER GA):" << endl;
        for(auto child : children) {
            unordered_map<int, bool> mapka;
            for(auto el : child.S) {
                cout << el << " ";
                auto map_it = mapka.find(el);
                if(map_it == mapka.end()) {
                    mapka[el] = 1;
                } else {
                    cout << "FOUND DUPLICATE EL: " << el << endl;
                }
            }
            cout << "\nPATH:" << endl;
            for(auto el : child.path) {
                cout << el->name << " ";
            }
            cout << "\nSCORE = " << child.score;
            cout << endl << endl;
        }
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}