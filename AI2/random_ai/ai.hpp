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

  void Init(int _punter_id, int num_of_punters, const Graph& g_, bool futures)
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
    remove_if(g.edges.begin(), g.edges.end(), [&] (const Edge& e) { return used.count(e); });
    return Move(g.edges.front());
  }

  vector<pair<int,int>> Future()
  {
    return {};
  }

  void Parse(const string& state)
  {
    istringstream sin(state);
    sin >> punter_id;

    int x;

    sin >> x;
    g.edges.resize(x);
    each (e, g.edges)  sin >> e.first >> e.second;

    sin >> x;
    g.mines.resize(x);
    each (i, g.mines)  sin >> i;

    for (int a, b; sin >> a >> b; ) {
      used.insert(Edge(a, b));
      used.insert(Edge(b, a));
    }
    return ;
  }

  string State() const
  {
    ostringstream oss;
    oss << punter_id << ' ';

    oss << g.edges.size() << ' ' ;
    each (e, g.edges) oss << e.first << ' ' << e.second << ' ';

    oss << g.mines.size() << ' ';
    each (i, g.mines) oss << i << ' ';

    each (e, used) {
      oss << e.first << ' ' << e.second << ' ';
    }
    return oss.str();
  }

  int PunterId() const
  {
    return punter_id;
  }
};
