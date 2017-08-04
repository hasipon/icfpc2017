#pragma once

#include <vector>
using namespace std;

typedef pair<int, int> Edge;

struct Move {
    int punter_id;
    int source;
    int target;
    bool is_pass;
    Move() : is_pass(true) {}
    Move(int source, int target) : is_pass(false), source(source), target(target) {}
    Move(Edge e) : is_pass(false), source(e.first), target(e.second) {}
};
using Moves = vector<Move>;

typedef struct {
    vector<Edge> edges;
    vector<int> mines;
} Graph;
