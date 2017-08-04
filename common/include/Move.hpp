#ifndef INCLUDED_MOVE
#define INCLUDED_MOVE


#include<vector>

using namespace std;

struct Move {
    int source;
    int punter_id;
    int target;
    bool is_pass;
};
using Moves = vector<Move>;

#endif
