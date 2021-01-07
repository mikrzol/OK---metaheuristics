#include "graph.h"
#include <iostream>
#include <string>
#include <sstream>

#pragma once
using namespace std;

Graph load_graph_from_file() {
    ifstream data;
    data.open("saved_graph.txt");
    string line;
    // first line contains the size of the graph
    int size;
    getline(data, line);
    size = stoi(line);
    // create a graph based on this size
    Graph g = Graph(size);

    while(getline(data, line)) {
        string text(line);
        istringstream iss(text);
        string segment;
        vector<string> seglist;
        while(getline(iss, segment, '\t')) {
            seglist.push_back(segment);
        }
        // name of the src node is in seglist[0]
        string node_name = seglist[0];
        // info on all the connections is in seglist[1:]
        for(int i = 1; i < seglist.size(); i++) {
            // string parsing based on on a delimiter
            string delimiter = "#";
            size_t pos = 0;
            vector<string> tokens;
            while((pos = seglist[i].find(delimiter)) != string::npos) {
                string t = seglist[i].substr(0, pos);
                tokens.push_back(t);
                seglist[i].erase(0, pos + delimiter.length());
            }
            tokens.push_back(seglist[i]);

            string dest = tokens[0], type = tokens[1], weight = tokens[2];
            
            // create conncetions based on info stored in tokens
            if(type == "E"){
                g.add_edge_from_file(stoi(node_name), stoi(dest), stoi(weight));
            } else if (type == "A"){
                g.add_arc(stoi(node_name), stoi(dest), stoi(weight));
            }
            
            // printing
            /*
            for(auto t : tokens) {
                cout << t << " ";
            }
            cout << "\t";
            */
        }
    }
    return g;
};