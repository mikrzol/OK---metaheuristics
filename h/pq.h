#pragma once
#include <iostream>
#include <vector>
using namespace std;

#define INF 100000

struct Vertex {
    int name;
    bool visited;
    int distance;
    Vertex* prev;

    Vertex(int n, int dist) : name(n), visited(0), distance(dist), prev(NULL) {};
};

struct P_QUEUE {
    vector<Vertex*> nodes;

    void swap(Vertex* a, Vertex* b) {
        Vertex temp = *b;
        *b = *a;
        *a = temp;
    }

    void heapify(int i) {
        int size = this->nodes.size();
        // find the largest el on this level
        int largest = i;
        int l_child = i*2 +1;
        int r_child = i*2 +2;

        if(l_child < size && nodes[l_child]->distance < nodes[largest]->distance) {
            largest = l_child;
        }
        if(r_child < size && nodes[r_child]->distance < nodes[largest]->distance) {
            largest = r_child;
        }

        // swap and move heapify "up a level"
        if(largest != i) {
            swap(this->nodes[i], this->nodes[largest]);
            heapify(largest);
        }
    }

    void insert(Vertex* new_node) {
        if(this->nodes.size() == 0) {
            this->nodes.push_back(new_node);
        } else {
            this->nodes.push_back(new_node);
            for(int i = this->nodes.size()/2 -1; i >= 0; i--) {
                heapify(i);
            }
        }
    }

    Vertex* top() {
        return this->nodes[0];
    }

    Vertex* pop() {
        swap(this->nodes[0], this->nodes[this->nodes.size()-1]);
        Vertex* to_return = this->nodes[this->nodes.size()-1];
        this->nodes.pop_back();
        heapify(0);
        return to_return;
    }

    void decrease_priority(int name, int cost) {
        int i = 0;
        for(i; i < this->nodes.size(); i++) {
            if(nodes[i]->name == name) {
                nodes[i]->distance = cost;
            }
        }
        if(i/2-1 > 0) {
            for(int c = i/2 -1; c >= 0; c--) {
                heapify(c);
            }
        } else {
            heapify(0);
        }
    };

    void print_pq() {
        cout << "P_QUEUE: " << endl;
        for(auto el : this->nodes) {
            cout << el->name << "(" << el->distance << ")" << " ";
        }
        cout << endl;
    }


    P_QUEUE(){};
    ~P_QUEUE() {
        for(auto el : this->nodes) {
            delete el;
        }
    }

};