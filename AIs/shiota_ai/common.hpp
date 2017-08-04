#pragma once

#include <vector>
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

typedef pair<int, int> Edge;

typedef struct {
    vector<Edge> edges;
    vector<int> mines;
} Graph;
