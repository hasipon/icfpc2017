#pragma once

#include <sstream>
#include <algorithm>
#include <iostream>
#include <deque>
#include <vector>
#include <set>
#include <map>
#include <queue>

#include <cassert>

#include "./common.hpp"

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

struct UnionFind
{
  map<int, int> data;
  UnionFind() {}
  int root(int x)
  {
    if (data.count(x) == 0) data[x] = -1;
    return data[x] < 0 ? x : data[x] = root(data[x]);
  }
  
  bool merge(int x, int y)
  {
    x = root(x);
    y = root(y);
    if (x == y) return false;
    if (data[y] < data[x]) swap(x, y);
    data[x] += data[y];
    data[y] = x;
    return true;
  }

  bool is_same_set(int a, int b)
  {
    return root(a) == root(b);
  }
};

const int MINE_CONNECTION = 0;
const int SCORE_GREEDY = 1;
const int OTHER = 2;

struct GameState
{
  set<Edge> used;
  map<pair<int, int>, int> cost;
  set<int> connected;
  set<Edge> owned;
  UnionFind uf;
  int mode;

  GameState() {}

  void Update(const Moves& moves)
  {
    each (move, moves) {
      used.insert(Edge(move.source, move.target));
      used.insert(Edge(move.target, move.source));
    }
    return ;
  }

  void Update(Edge e)
  {
    Update(Move(e));
  }
  
  void Update(Move move)
  {
    uf.merge(move.source, move.target);
    owned.insert(Edge(move.source, move.target));
    owned.insert(Edge(move.target, move.source));
    connected.insert(move.source);
    connected.insert(move.target);
    used.insert(Edge(move.source, move.target));
    used.insert(Edge(move.target, move.source));
  }
};

istream& operator >> (istream& is, GameState& state)
{
  string header;
  int x;

  // used
  is >> header >> x;
  for (int i = 0; i < x; ++i) {
    Edge e;
    assert(is >> e.first >> e.second);
    state.used.insert(e);
  }

  // cost
  is >> header >> x;
  for (int i = 0; i < x; ++i) {
    int a, b, c;
    assert(is >> a >> b >> c);
    state.cost[make_pair(a, b)] = c;
  }

  // connected
  assert(is >> header >> x);
  for (int i = 0; i < x; ++i) {
    int y;
    assert(is >> y);
    state.connected.insert(y);
  }

  // owned
  assert(is >> header >> x);
  for (int i = 0; i < x; ++i) {
    int a, b;
    assert(is >> a >> b);
    state.owned.insert(Edge(a, b));
    state.owned.insert(Edge(b, a));
  }

  // mode
  assert(is >> header >> state.mode);

  // uf
  assert(is >> header >> x);
  each (i, state.uf.data) {
    int a, b;
    cin >> a >> b;
    state.uf.data[a] = b;
  }

  return is;
}

ostream& operator << (ostream& os, const GameState& state)
{
  // used
  os << "used: ";
  os << state.used.size() << ' ';
  each (e, state.used) os << e.first <<' ' << e.second << ' ';

  // cost
  os << "cost: ";
  os << state.cost.size() << ' ';
  each (i, state.cost) {
    os << i.first.first << ' ' << i.first.second << ' ' << i.second << ' ';
  }

  // connected
  os << "connected: ";
  os << state.connected.size() << ' ';
  each (i, state.connected) os << i << ' ';

  // owned
  os << "owned: ";
  os << state.owned.size() << ' ';
  each (e, state.owned) os << e.first << ' ' << e.second << ' ';

  // mode
  os << "mode: ";
  os << state.mode;

  // uf
  os << "uf: ";
  os << state.uf.data.size() << ' ';
  each (i, state.uf.data) os << i.first << ' ' << i.second << ' ';

  return os;
}

struct GameContext
{
  int punter_id;
  map<int, vector<int>> g;
  vector<int> node;
  vector<int> mines;
  int num_punters;
};

ostream& operator << (ostream& os, const GameContext& context)
{
  // punter_id
  os << "punterid: ";
  os << context.punter_id << ' ';

  // num_punter
  os << "num_punters";
  os << context.num_punters;

  // g
  os << "g: ";
  int sum = 0;
  each (i, context.g) sum += i.second.size();
  os << sum << ' ';
  each (i, context.g) {
    each (e, i.second) {
      os << i.first << ' ' << e << ' ' ;
    }
  }

  // node
  os << "node: ";
  os << context.node.size() << ' ';
  each (i, context.node) os << i << ' ' ;

  // mines
  os << "mines: ";
  os << context.mines.size() << ' ';
  each (i, context.mines) os << i << ' ';

  return os;
}

istream& operator >> (istream& is, GameContext& context)
{
  string header;
  int x;
  // punter_id
  assert(is >> header >> context.punter_id);

  // num_penters
  assert(is >> header >> context.num_punters);

  // g
  assert(is >> header >> x);
  for (int i = 0; i < x; ++i) {
    int a, b;
    assert(is >> a >> b);
    context.g[a].push_back(b);
  }

  // node
  assert(is >> header >> x);
  context.node.resize(x);
  each (i, context.node) assert(is >> i);


  // mines
  assert(is >> header >> x);
  context.mines.resize(x);
  each (i, context.mines) assert(is >> i);

  return is;
}

class BaseAI
{
public:

  GameState state;
  GameContext context;

  const string punter_name = "bfs3";

  BaseAI (string name) : punter_name(name) {}

  string Name() const
  {
    return punter_name;
  }

  // return num of reachable nodes
  int BFS(int src)
  {
    set<int> vis;
    vis.insert(src);

    queue<int> q;
    for (q.push(src); q.size(); q.pop()) {
      const int curr = q.front();
      each (next, context.g[curr]) {
        unless (vis.count(next)) {
          vis.insert(next);
          q.push(next);
          state.cost[make_pair(src, next)] = state.cost[make_pair(src, curr)] + 1;
        }
      }
    }

    return vis.size();
  }

  virtual void Init(int _punter_id, int num_of_punters, const Graph& g, bool futures)
  {
    context.punter_id = _punter_id;

    context.num_punters = num_of_punters;

    each (e, g.edges) {
      context.g[e.first].push_back(e.second);
      context.g[e.second].push_back(e.first);
    }

    each (i, context.g) context.node.push_back(i.first);

    map<int, int> reachables;
    context.mines = g.mines;
    each (mine, context.mines) reachables[mine] = BFS(mine);
    sort(
      context.mines.begin(),
      context.mines.end(),
      [&] (int a, int b) { return reachables[a] > reachables[b]; }
    );

    state.mode = SCORE_GREEDY;
  }

  Move ScoreGreedy(void)
  {
    Move ret;
    int mx_point = 0;
    int mn = (1 << 29);
    each (src, state.connected) {
      each (dst, context.g[src]) {
        if (state.connected.count(dst)) continue;
        const Edge e(src, dst);
        if (state.used.count(e)) continue;
        const int point = get_point(e);
        if (mx_point < point) {
          mx_point = point;
          ret = Move(e);
        }
      }
    }

    if (ret.is_pass) {
      state.mode = OTHER;
      return FreeEdge();
    }
    return ret;
  }

  Move FreeEdge(void)
  {
    each (src, context.mines) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        unless (state.used.count(e)) return Move(e);
      }
    }
    each (src, context.node) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        unless (state.used.count(e)) return Move(e);
      }
    }
    return Move();
  }

  virtual Move Think(const Moves& moves, const string& state_str)
  {
    Parse(state_str);
    state.Update(moves);
    const Move ret = _Think(moves);
    assert(!ret.is_pass);
    state.Update(ret);
    return ret;
  }

  virtual Move _Think(const Moves& moves)
  {
    switch (state.mode) {
      case SCORE_GREEDY: return ScoreGreedy();
      default:           return FreeEdge();
    }
    return Move();
  }

  int get_h(int v)
  {
    int mn = 1 << 29;
    each (mine, context.mines) {
      if (state.connected.count(mine)) continue;
      const pair<int, int> key = make_pair(mine, v);
      if (state.cost.count(key)) {
        mn = min(mn, state.cost[key]);
      }
    }
    return mn;
  }

  int get_point(Edge e)
  {
    UnionFind uf = state.uf;
    const int h = get_h(e.second);
    int sum = 0;
    uf.merge(e.first, e.second);
    each (mine, context.mines) {
      if (state.uf.is_same_set(mine, e.second)) {
        const pair<int, int> key = make_pair(mine, e.second);
        const int c = state.cost[key];
        sum += c * c - h;
      }
    }

    return sum;
  }

  virtual vector<pair<int,int>> Future()
  {
    return {};
  }

  virtual void Parse(const string& state_str)
  {
    istringstream sin(state_str);
    sin >> context >> state;
    return ;
  }

  virtual string State() const
  {
    ostringstream oss;
    oss << context << state;
    return oss.str();
  }

  int PunterId() const
  {
    return context.punter_id;
  }
};
