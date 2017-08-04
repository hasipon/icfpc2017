#include<iostream>

#include "graph.h"

using namespace std;

int main(){
    Graph g = {0, 0, 0, true};
    cout << g.punter << ' '
        << g.source << ' '
        << g.target << ' '
        << g.is_pass <<endl;

}
