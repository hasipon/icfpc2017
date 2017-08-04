#pragma once

#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

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

struct AI
{
  Graph g;
  int penter_id;
  int num_punters;

  vector<int> vis;
  set<Edge> used;
  int turn_conut = 0;

  void Init(int punter_id, int num_of_punters, const Graph& _g)
  {
    this->penter_id = punter_id;
    this->num_punters = num_of_punters;
    this->g = _g;
  }

  Move Think(const Moves& moves)
  {
    each (move, moves) {
      used.insert(Edge(move.source, move.target));
      used.insert(Edge(move.target, move.source));
    }

    Move ret;

    auto fn = [&] () {
      set<int> mines(g.mines.begin(), g.mines.end());
      each (e, g.edges) {
        if (used.count(e)) continue;
        ret = Move(e);
        if (mines.count(e.first)) {
          vis.push_back(e.first);
          vis.push_back(e.second);
          break;
        }
        if (mines.count(e.second)) {
          vis.push_back(e.second);
          vis.push_back(e.first);
          break;
        }
      }
      return ;
    };
    
    if (turn_conut) {
      for (int i = vis.size() - 1; 0 <= i; --i) {
        const int prev = vis[i];
        each (e, g.edges) {
          if (used.count(e)) continue;
          ret = Move(e);
          if (e.first == prev) {
            vis.push_back(e.second);
            i = -1;
            break;
          }
          if (e.second == prev) {
            vis.push_back(e.first);
            i = -1;
            break;
          }
        }
      }
      if (ret.is_pass) fn();
    } else {
      fn();
    }

    used.insert(Edge(ret.source, ret.target));
    ++turn_conut;
    // cerr << Edge(ret.source, ret.target) << endl;
    return ret;
  }
};
