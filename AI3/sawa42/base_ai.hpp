#pragma once

#include "./common.hpp"

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

istream& operator >> (istream& is, UnionFind& x)
{
  is >> x.data;
  return is;
}

ostream& operator << (ostream& os, const UnionFind& x)
{
  os << x.data;
  return os;
}


constexpr int MINE_CONNECTION = 0;
constexpr int SCORE_GREEDY = 1;
constexpr int OTHER = 2;

struct GameContext
{
  int punter_id;
  int num_punters;
  map<int, vector<int>> g;
  vector<int> node;
  set<int> mines;
};

ostream& operator << (ostream& os, const GameContext& context)
{
  assert(os << context.punter_id);   os << ' ';
  assert(os << context.num_punters); os << ' ';
  assert(os << context.g);           os << ' ';
  assert(os << context.node);        os << ' ';
  assert(os << context.mines);       os << ' ';
  return os;
}

istream& operator >> (istream& is, GameContext& context)
{
  assert(is >> context.punter_id);
  assert(is >> context.num_punters);
  assert(is >> context.g);
  assert(is >> context.node);
  assert(is >> context.mines);
  return is;
}

struct GameState
{
  map<Edge, int> used;
  map<pair<int, int>, int> cost;
  set<int> connected;
  set<Edge> owned;
  UnionFind uf;
  int mode;

  void Update(const _Move& move, int my_punter_id)
  {
    for (int i = 0; i + 1 < move.size(); ++i) {
      const Edge a(move[i], move[i + 1]);
      const Edge b(move[i + 1], move[i]);
      used[a] = move.punter_id;
      used[b] = move.punter_id;
      if (move.punter_id == my_punter_id) {
        owned.insert(a);
        owned.insert(b);
      }
    }
    each (site, move) {
      connected.insert(site);
      uf.merge(site, move.front());
    }
    return ;
  }

  void Update(const _Moves& moves, int my_punter_id)
  {
    each (move, moves) Update(move, my_punter_id);
    return ;
  }

  bool IsFree(Edge e) const
  {
    return used.count(e) == 0;
  }

  bool IsAdjacence(GameContext context, Edge e) const
  {
    if (connected.count(e.first) != connected.count(e.second)) {
      return true;
    }
    if (connected.count(e.first) == 0 && context.mines.count(e.second) == 1) {
      return true;
    }
    if (connected.count(e.second) == 0 && context.mines.count(e.first) == 1) {
      return true;
    }
    return false;
  }
};

istream& operator >> (istream& is, GameState& state)
{
  assert(is >> state.used);
  assert(is >> state.cost);
  assert(is >> state.connected);
  assert(is >> state.owned);
  assert(is >> state.mode);
  assert(is >> state.uf);
  return is;
}

ostream& operator << (ostream& os, const GameState& state)
{
  assert(os << state.used);      os << ' ';
  assert(os << state.cost);      os << ' ';
  assert(os << state.connected); os << ' ';
  assert(os << state.owned);     os << ' ';
  assert(os << state.mode);      os << ' ';
  assert(os << state.uf);        os << ' ';
  return os;
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

  virtual void Init(int _punter_id, int num_of_punters, const Graph& g, bool futures, bool splurges)
  {
    context.punter_id = _punter_id;

    context.num_punters = num_of_punters;

    each (e, g.edges) {
      context.g[e.first].push_back(e.second);
      context.g[e.second].push_back(e.first);
    }

    each (i, context.g) context.node.push_back(i.first);

    map<int, int> reachables;
    context.mines = set<int>(g.mines.begin(), g.mines.end());
    each (mine, context.mines) BFS(mine);
    state.mode = SCORE_GREEDY;
  }

  Edge FreeEdge(void)
  {
    each (src, context.mines) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        unless (state.used.count(e)) return e;
      }
    }
    each (src, context.node) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        unless (state.used.count(e)) return e;
      }
    }
    return Edge(-1, -1);
  }

  virtual vector<SiteID> Think(const Moves& moves, const string& state_str)
  {
    Parse(state_str);
    state.Update(moves, context.punter_id);
    const _Move move = _Think(_Moves(moves));
    assert(move.isValid());

    vector<SiteID> v;
    each (i, move) {
      cerr << i << endl;
      v.push_back(i);
    }
    return v;
  }

  virtual _Move _Think(const _Moves& moves)
  {
    assert(false);
  }

  lli GetCurrentScore(void)
  {
    lli score = 0;
    each (src, context.mines) {
      each (dst, context.node) {
        if (state.uf.is_same_set(src, dst)) {
          const pair<int, int> key = make_pair(src, dst);
          lli c = state.cost[key];
          score += c * c;
        }
      }
    }
    return score / 2;
  }

  virtual vector<Edge> Future()
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

  vector<Edge> Adjacence(SiteID src)
  {
    vector<Edge> es;

    each (dst, context.g[src]) {
      const Edge e(src, dst);
      if (state.IsFree(e) && state.IsAdjacence(context, e)) {
        es.push_back(e);
      }
    }

    return es;
  }

  vector<Edge> Adjacence()
  {
    vector<Edge> es;

    each (src, context.node) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        if (state.IsFree(e) && state.IsAdjacence(context, e)) {
          es.push_back(e);
        }
      }
    }

    return es;
  }

};
