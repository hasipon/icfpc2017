#pragma once

#include "./base_ai.hpp"

class AI : public BaseAI
{
public:
  AI () : BaseAI("bfs1") {}

  virtual _Move _Think(const _Moves& moves) override
  {
    Edge best(NULL_NODE_ID, NULL_NODE_ID);
    lli mx = -1;
    each (src, context.node) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        const lli point = GetPoint(e);
        if (state.IsFree(e) && state.IsAdjacence(context, e)) {
          if (mx < point) {
            mx = point;
            best = e;
          }
          if (mx == point && GetH(best.second) < GetH(dst)) {
            best = e;
          }
        }
      }
    }

    if (best.first == best.second) {
      best = FreeEdge();
    }
    assert(best.first != best.second);
    return _Move(context.punter_id, best);
  }

  bool Reachable(int src, int dst)
  {
    return true;
  }

  lli GetH(int v)
  {
    int mn = 1 << 29;
    each (mine, context.mines) {
      if (state.connected.count(mine)) continue;
      if (!Reachable(mine, v)) continue;
      const pair<int, int> key = make_pair(mine, v);
      if (state.cost.count(key)) {
        mn = min(mn, state.cost[key]);
      }
    }

    return mn;
  }

  lli GetPoint(Edge e)
  {
    UnionFind uf = state.uf;
    lli sum = 0;
    uf.merge(e.first, e.second);
    each (mine, context.mines) {
      if (uf.is_same_set(mine, e.second)) {
        const pair<int, int> key = make_pair(mine, e.second);
        if (state.cost.count(key)) {
          const int c = state.cost[key];
          sum += c * c;
        }
      }
    }
    return sum;
  }
};
