#pragma once

#include <vector>
#include <map>
#include <string>
#include <sstream>
using namespace std;

using Edge = pair<int64_t, int64_t>;

struct Move {
    int64_t punter_id;
    vector<int64_t> route;
    Move() {}
    Move(int64_t punter_id, const vector<int64_t>& route) : punter_id(punter_id), route(route) {}
};

using Moves = vector<Move>;

struct Site {
    int64_t id;
    double x, y;
};

struct Graph {
    map<int64_t, Site> sites;
    vector<Edge> edges;
    vector<int64_t> mines;
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
        for(int64_t m : mines){
            ss << m <<' ';
        }
        return ss.str();
    }
};

Graph parseGraph(string in){
    Graph g;
    stringstream ss(in);

    // 辺数
    int64_t en;
    ss >> en;
    // 辺
    for(int64_t i = 0; i< en; i++){
        Edge e;
        ss >> e.first >> e.second;
        g.edges.push_back(e);
    }

    // mine数
    int64_t em;
    ss >> em;
    // mine
    for(int64_t i = 0; i< em; i++){
        int64_t m;
        ss >> m;
        g.mines.push_back(m);
    }
    return g;
}
