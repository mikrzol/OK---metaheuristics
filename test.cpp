#include "./h/specimen.h"
using namespace std;

int main() {
    Graph g = load_graph_from_file();
    Specimen s;
    s.initialise_S(g);
    s.find_path(g);

    cout << "S: " << endl;
    for(auto el : s.S) {
        cout << el << " ";
    }
    cout << endl << endl;

    cout << "x:" << endl;
    for(auto el : s.x) {
        cout << "[";
        for(auto node : el) {
            cout << node->name << " ";
        }
        cout << "], ";
    }
    cout << endl;

    cout << "zrobione" << endl;
    return 1;
}