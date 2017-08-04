#ifndef INCLUDED_MOVE
#define INCLUDED_MOVE


#include<vector>

using namespace std;

struct Move {
    int punter_id;
    int source;
    int target;
    bool is_pass;
    Move() : is_pass(true) {}
    Move(int source, int target) : is_pass(false), source(source), target(target) {}
};
using Moves = vector<Move>;

#endif
