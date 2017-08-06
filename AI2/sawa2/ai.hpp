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

struct GameState {
  vector<int> sites;
  vector<Edge> edges;
  vector<int> mines;
  set<Edge, int> owners;
  vector<PunterState> punter_state;
  
  GameState(Graph g, int num_of_punters) {
    g = _g;
    for (int i = 0; i < num_of_punters; i++) {
      punter_state.push_back(PunterState(i))
    }
  }

  string State() const {
    ostringstream oss;
    oss << g.toString() << ' ';

    oss << g.owners.size() << ' ' ;
    each (e, g.edges) oss << e.first << ' ' << e.second << ' ';
    
    oss << punter_state.State() << ' ' ;

    return oss.str();
  }
}

struct PunterState {
  int punter_id;
  set<Edge> owned;
  
  PunterState(int _punter_id) {
    this.punter_id = _punter_id;
  }

  string State() const {
    ostringstream oss;
    oss << punter_id << ' ';

    oss << owned.size() << ' ' ;
    each (e, owned) oss << e << ' ';

    return oss.str();
  }
  
  void Parse(const string& state)
  {
    istringstream sin(state);
    sin >> punter_id;

    int x;

    sin >> x;
    for ()  sin >> e.first >> e.second;

    sin >> x;
    g.mines.resize(x);
    each (i, g.mines)  sin >> i;

    for (int a, b; sin >> a >> b; ) {
      used.insert(Edge(a, b));
      used.insert(Edge(b, a));
    }
    return ;
  }
}

struct AI {
  int punter_id;
  GameState state;

  string Name()
  {
    return "sawa_2";
  }

  void Init(int _punter_id, int num_of_punters, const Graph& g_, bool futures)
  {
    punter_id = _punter_id;
    state = GameState(g, num_of_punters);
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
