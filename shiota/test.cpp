#include<iostream>

#include "../common/include/Graph.hpp"
#include "../common/include/Move.hpp"

using namespace std;

int main(){
    Graph g;
    cout << g.edges.size() << endl;
    cout << g.mines.size() <<endl;
    Move m = (Move){0, 0, 0, 0};
    cout << m.source << endl;
    cout << m.punter_id << endl;
}
