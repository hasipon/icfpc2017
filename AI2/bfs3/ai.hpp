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

struct UnionFind {
  vector<int> data;
  UnionFind() {}
  UnionFind(int n) : data(n, -1) {}
  int root(int x) {
    return data[x] < 0 ? x : data[x] = root(data[x]);
  }
  bool merge(int x, int y) {
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

struct AI {
  int punter_id;
  map<int, vector<int>> g;
  vector<int> mines;
  set<Edge> used;
  // key: (mine, site), value: dist
  map<pair<int, int>, int> cost;
  vector<int> node;

  set<int> connected;
  set<Edge> owned;

  UnionFind uf;

  int mode;

  string Name()
  {
    return "bfs3";
  }

  // return num of reachable nodes
  int BFS(int src)
  {
    set<int> vis;
    vis.insert(src);

    queue<int> q;
    for (q.push(src); q.size(); q.pop()) {
      const int curr = q.front();
      each (next, g[curr]) {
        unless (vis.count(next)) {
          vis.insert(next);
          q.push(next);
          cost[make_pair(src, next)] = cost[make_pair(src, curr)] + 1;
        }
      }
    }

    return vis.size();
  }

  void Init(int _punter_id, int num_of_punters, const Graph& _g, bool futures)
  {
    punter_id = _punter_id;

    each (e, _g.edges) {
      g[e.first].push_back(e.second);
      g[e.second].push_back(e.first);
    }

    each (i, g) node.push_back(i.first);

    map<int, int> reachables;
    mines = _g.mines;
    each (mine, mines) reachables[mine] = BFS(mine);
    sort(mines.begin(), mines.end(), [&] (int a, int b) { return reachables[a] > reachables[b]; });

    uf = UnionFind(node.size());

    mode = SCORE_GREEDY;
  }

  Move ScoreGreedy(void)
  {
    Move ret;
    int mx_point = 0;
    int mn = (1 << 29);
    each (src, connected) {
      each (dst, g[src]) {
        if (connected.count(dst)) continue;
        const Edge e(src, dst);
        if (used.count(e)) continue;
        const int point = get_point(e);
        if (mx_point < point) {
          mx_point = point;
          ret = Move(e);
        }
      }
    }

    if (ret.is_pass) {
      mode = OTHER;
      return FreeEdge();
    }
    return ret;
  }

  Move FreeEdge(void)
  {
    each (src, mines) {
      each (dst, g[src]) {
        const Edge e(src, dst);
        unless (used.count(e)) return Move(e);
      }
    }
    each (src, node) {
      each (dst, g[src]) {
        const Edge e(src, dst);
        unless (used.count(e)) return Move(e);
      }
    }
    return Move();
  }

  Move Think(const Moves& moves, const string& state)
  {
    Parse(state);
    each (move, moves) {
      used.insert(Edge(move.source, move.target));
      used.insert(Edge(move.target, move.source));
    }

    Move ret;
    switch (mode) {
      case SCORE_GREEDY:
        ret = ScoreGreedy();
        break;
      default:
        ret = FreeEdge();
        break;
    }

    assert(!ret.is_pass);
    uf.merge(ret.source, ret.target);
    owned.insert(Edge(ret.source, ret.target));
    owned.insert(Edge(ret.target, ret.source));
    connected.insert(ret.source);
    connected.insert(ret.target);
    used.insert(Edge(ret.source, ret.target));
    used.insert(Edge(ret.target, ret.source));

    return ret;
  }

  int get_h(int v)
  {
    int mn = 1 << 29;
    each (mine, mines) {
      if (connected.count(mine)) continue;
      const pair<int, int> key = make_pair(mine, v);
      if (cost.count(key)) {
        mn = min(mn, cost[key]);
      }
    }
    return mn;
  }

  int get_point(Edge e)
  {
    UnionFind tmp = uf;
    const int h = get_h(e.second);
    int sum = 0;
    uf.merge(e.first, e.second);
    each (mine, mines) {
      if (uf.is_same_set(mine, e.second)) {
        const pair<int, int> key = make_pair(mine, e.second);
        const int c = cost[key];
        sum += c * c - h;
      }
    }

    uf = tmp;
    return sum;
  }

  vector<pair<int,int>> Future()
  {
    return {};
  }

  void Parse(const string& state)
  {
    string header;
    int x;
    istringstream sin(state);

    // punter_id
    assert(sin >> header >> punter_id);

    // mode
    assert(sin >> header >> mode);

    // g
    assert(sin >> header >> x);
    for (int i = 0; i < x; ++i) {
      int a, b;
      assert(sin >> a >> b);
      g[a].push_back(b);
    }

    // mines
    assert(sin >> header >> x);
    mines.resize(x);
    each (i, mines) assert(sin >> i);

    // used
    sin >> header >> x;
    for (int i = 0; i < x; ++i) {
      Edge e;
      assert(sin >> e.first >> e.second);
      used.insert(e);
    }

    // cost
    sin >> header >> x;
    for (int i = 0; i < x; ++i) {
      int a, b, c;
      assert(sin >> a >> b >> c);
      cost[make_pair(a, b)] = c;
    }

    // node
    assert(sin >> header >> x);
    node.resize(x);
    each (i, node) assert(sin >> i);

    // connected
    assert(sin >> header >> x);
    for (int i = 0; i < x; ++i) {
      int y;
      assert(sin >> y);
      connected.insert(y);
    }

    // owned
    assert(sin >> header >> x);
    for (int i = 0; i < x; ++i) {
      int a, b;
      assert(sin >> a >> b);
      owned.insert(Edge(a, b));
      owned.insert(Edge(b, a));
    }

    assert(sin >> header >> x);
    uf.data.resize(x);
    each (i, uf.data) assert(sin >> i);

    return ;
  }

  string State() const
  {
    ostringstream oss;

    // punter_id
    oss << "punterid: ";
    oss << punter_id << ' ';

    // mode
    oss << "mode: ";
    oss << mode;

    // g
    oss << "g: ";
    int sum = 0;
    each (i, g) sum += i.second.size();
    oss << sum << ' ';
    each (i, g) {
      each (e, i.second) {
        oss << i.first << ' ' << e << ' ' ;
      }
    }

    // mines
    oss << "mines: ";
    oss << mines.size() << ' ';
    each (i, mines) oss << i << ' ';

    // used
    oss << "used: ";
    oss << used.size() << ' ';
    each (e, used) oss << e.first <<' ' << e.second << ' ';

    // cost
    oss << "cost: ";
    oss << cost.size() << ' ';
    each (i, cost) {
      oss << i.first.first << ' ' << i.first.second << ' ' << i.second << ' ';
    }

    // node
    oss << "node: ";
    oss << node.size() << ' ';
    each (i, node) oss << i << ' ' ;

    // connected
    oss << "connected: ";
    oss << connected.size() << ' ';
    each (i, connected) oss << i << ' ';

    // owned
    oss << "owned: ";
    oss << owned.size() << ' ';
    each (e, owned) oss << e.first << ' ' << e.second << ' ';

    // uf
    oss << "uf: ";
    oss << uf.data.size() << ' ';
    each (i, uf.data) oss << i << ' ';

    return oss.str();
  }

  int PunterId() const
  {
    return punter_id;
  }
};
