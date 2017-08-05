#pragma once

// ただのランダムですよ

#include <set>
#include <map>
#include <iostream>
#include <queue>
using namespace std;


std::map<int, int> bfs(const Graph& g, queue<int> &q, int limit){
  {
    map<int, int> cost;
    map<int, vector<int>> es;
    for (auto& e : g.edges) {
      es[e.first].emplace_back(e.second);
      es[e.second].emplace_back(e.first);
      cost[e.first] = cost[e.second] = limit;
    }

    for (; q.size(); q.pop()) {
      const auto curr = q.front();
      if(limit == cost[curr])cost[curr] = 0;
      if(cost[curr] == limit-1)break;
      for (auto& next: es[curr]) {
        if (cost.count(next) == 0) {
          cost[next] = cost[curr] + 1;
          q.push(next);
        }
      }
    }
    return cost;
  }
}

  struct AI {
    int pid, V;
    Graph g;

    set<int> myArea;
    set<pair<int, int> > used;

    void Init(int punter_id, int num_of_punters, const Graph& g) {
      pid = punter_id;
      V = num_of_punters;
      this->g = g;

      for(auto i : g.mines){
        myArea.insert(i);
      }
    }

    Move Think(const Moves& moves) {
      Move ret;
      try {
        for(auto move : moves){
          used.insert(make_pair(move.source, move.target));
          used.insert(make_pair(move.target, move.source));
        }
        vector<Edge> v;
        for(auto e : g.edges){
          if(used.count(e))continue;
          v.push_back(e);
        }
        Edge choosed = v[rand() % v.size()];
        return build(choosed);
      } catch(char *s){
        cerr << "FATAL " << *s <<endl;
        return Move();
      }
      return ret;
    }

    Move build(Edge e){
      Move ret;
      ret.punter_id = pid;
      ret.source = e.first;
      ret.target = e.second;
      ret.is_pass = false;
      return ret;
    }
};
