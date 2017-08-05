#pragma once

#include <vector>
#include <map>
#include <string>
#include <sstream>
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
    string toString() const {
        stringstream ss;
        // 辺数
        ss << edges.size() << ' ';
        // 辺
        for(Edge e : edges){
            ss << e.first << ' ' << e.second <<' ';
        }
        // mine数
        ss << mines.size() <<' ';
        // mine
        for(int m : mines){
            ss << m <<' ';
        }
        return ss.str();
    }
};

Graph parseGraph(string in){
    Graph g;
    stringstream ss(in);

    // 辺数
    int en;
    ss >> en;
    // 辺
    for(int i = 0; i< en; i++){
        Edge e;
        ss >> e.first >> e.second;
        g.edges.push_back(e);
    }

    // mine数
    int em;
    ss >> em;
    // mine
    for(int i = 0; i< em; i++){
        int m;
        ss >> m;
        g.mines.push_back(m);
    }
    return g;
}
