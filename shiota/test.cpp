#include<iostream>

#include "../common/include/Graph.hpp"
#include "../common/include/Move.hpp"
#include "../AIs/shiotaAI/AI.hpp"


using namespace std;

int main(){
    Graph g;
    g.edges.push_back(make_pair(1, 2));
    g.edges.push_back(make_pair(2, 3));
    g.edges.push_back(make_pair(3, 4));
    g.mines.push_back(0);
    g.mines.push_back(2);
    cout << g.edges.size() << endl;
    cout << g.mines.size() <<endl;
    Move m = (Move){0, 0, 0, 0};
    cout << m.source << endl;
    cout << m.punter_id << endl;

    Moves ms;

    AI ai;
    ai.Init(1, 10, g);
    ai.Think(ms);
}
