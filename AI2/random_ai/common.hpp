#pragma once

#include <vector>
#include <map>
using namespace std;

using Edge = pair<int, int>;

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

struct Site {
    int id;
    double x, y;
};

struct Graph {
    map<int, Site> sites;
    vector<Edge> edges;
    vector<int> mines;
};
