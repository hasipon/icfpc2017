#pragma once

#include "./base_ai.hpp"

class AI : public BaseAI
{
public:
  AI () : BaseAI("bfs3") {}

  virtual _Move _Think(const _Moves& moves) override
  {
    Edge best(NULL_NODE_ID, NULL_NODE_ID);
    lli mx = -1;
    each (src, context.node) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        unless (state.IsFree(e)) continue;
        const lli point = GetPoint(e);
        if (state.IsAdjacence(context, e) && mx < point) {
          mx = point;
          best = e;
        }
      }
    }

    if (best.first == best.second) {
      best = FreeEdge();
    }
    return _Move(context.punter_id, best);
  }
  
  lli GetPoint(Edge e)
  {
    UnionFind uf = state.uf;
    lli sum_c = 0;
    int mn_h = 1 << 29;
    uf.merge(e.first, e.second);
    each (mine, context.mines) {
      const pair<int, int> key = make_pair(mine, e.second);
      unless (state.cost.count(key)) continue;
      if (uf.is_same_set(mine, e.second)) {
          sum_c += state.cost[key];
      } else {
        mn_h = min(mn_h, state.cost[key]);
      }
    }
    return sum_c * sum_c - mn_h;
  }
};
