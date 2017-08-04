#pragma once

#include <map>
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <queue>

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
  
  void Init(int punter_id, int num_of_punters, const Graph& _g)
  {
    g = _g;
  }
  Move Think(const Moves& moves)
  {
    each (move, moves) {
      used.insert(Edge(move.source, move.target));
      used.insert(Edge(move.target, move.source));
    }

    queue<int> q;
    each (i, g.mines) q.push(i);

    set<int> vis;
    each (i, g.mines) vis.insert(i);
    
    for (; q.size(); q.pop()) {
      const int curr = q.front();
      each (e, g.edges) {
        if (e.first == curr && vis.count(e.second) == 0) {
          if (used.count(e) == 0) return Move(e);
          q.push(e.second);
          vis.insert(e.second);
        }
        if (e.second == curr && vis.count(e.first) == 0) {
          if (used.count(e) == 0) return Move(e);
          q.push(e.first);
          vis.insert(e.first);
        }
      }
    }

    each (e, g.edges) {
      if (used.count(e) == 0) {
        return Move(e);
      }
    }
    
    return Move();
  }
};
