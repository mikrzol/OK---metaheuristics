#include "./h/specimen.h"
using namespace std;

int main() {
    srand(time(NULL));
    Graph g = load_graph_from_file();
    Specimen s;
    s.initialise_S(g);
    s.find_path(g);
    s.grade_path(g);

    cout << "S: " << endl;
    for(auto el : s.S) {
        cout << el << " ";
    }
    cout << endl << endl;

    cout << "path:" << endl;
    for(auto el : s.path) {
        cout << el->name << ", ";
    }
    cout << endl;

    cout << "path score = " << s.score << endl;
    pair<vector<Node*>, int> result = g.traverse_graph_naive();
    cout << "\nnaive traversal result score = " << result.second << endl;
    for(auto el : result.first) {
        cout << el->name << " ";
    }
    cout << endl;

    cout << endl << "zrobione" << endl;
    return 1;
}