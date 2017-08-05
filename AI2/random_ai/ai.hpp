#pragma once

#include <sstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <set>
#include <map>

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
  int punter_id;

  string Name()
  {
    return "DOM-DOM-RANDOM";
  }

  void Init(int _punter_id, int num_of_punters, const Graph& g_)
  {
    ostringstream oss;
    oss << _punter_id;
    punter_id = _punter_id;

    g = g_;
    each (e, g_.edges) {
      g.edges.push_back(Edge(e.second, e.first));
    }
  }

  Move Think(const Moves& moves, const string& state)
  {
    Parse(state);

    each (move, moves) {
      unless (move.is_pass) {
        used.insert(Edge(move.source, move.target));
        used.insert(Edge(move.target, move.source));
      }
    }

    random_shuffle(g.edges.begin(), g.edges.end());
    remove_if(g.edges.begin(), g.edges.end(), [&] (Edge e) { return used.count(e); });
    return Move(g.edges.front());
  }

  void Parse(const string& state)
  {
    istringstream sin(state);
    for (int a, b; sin >> a >> b; ) {
      used.insert(Edge(a, b));
      used.insert(Edge(b, a));
    }
    return ;
  }

  string State()
  {
    ostringstream oss;
    each (e, used) {
      int a, b;
      oss << a << b << endl;
    }
        return oss.str();
  }

  int PunterId()
  {
    return punter_id;
  }
};
