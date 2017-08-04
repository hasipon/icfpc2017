#pragma once

#include <map>
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <queue>

#include "../../common/algorithms/BFS.hpp"

#define each(i, c) for (auto& i : c)
#define unless(cond) if (!(cond))

using namespace std;

typedef long long int lli;
typedef unsigned long long ull;

template<typename P, typename Q>
ostream& operator << (ostream& os, pair<P, Q> p)
{
  os << "(" << p.first << "," << p.second << ")";
  return os;
}

struct AI {
  Graph g;
  set<Edge> used;
  map<int, int> cost;
  int turn = 0;
  int base;
  set<int> own;

  void Init(int punter_id, int num_of_punters, const Graph& _g)
  {
    g = _g;
    each (e, _g.edges) {
      g.edges.push_back(Edge(e.second, e.first));
    }
  }

  Move BuildCost(void)
  {
    set<int> mines(g.mines.begin(), g.mines.end());
    Edge edge;
    each (e, g.edges) {
      if (used.count(e)) continue;
      edge = e;
      if (mines.count(e.first)) {
        base = e.first;
        break;
      }
    }

    cost = bfs(g, base);
    return Move(edge);
  }

  Move Think(const Moves& moves)
  {
    each (move, moves) {
      used.insert(Edge(move.source, move.target));
      used.insert(Edge(move.target, move.source));
    }

    Move ret;

    if (turn == 0) {
      ret = BuildCost();
    }

    int c = 0;
    Edge best;
    each (e, g.edges) {
      if (used.count(e)) continue;
      if (own.count(e.first) == 1 && own.count(e.second) == 0) {
        if (c < cost[e.second]) {
          c = cost[e.second];
          best = e;
        }
      }
    }

    if (ret.is_pass) {
      each (e, g.edges) {
        if (used.count(e)) continue;
        ret = e;
        break;
      }
    }

    ++turn;
    own.insert(ret.source);
    own.insert(ret.target);
    return ret;
  }
};
