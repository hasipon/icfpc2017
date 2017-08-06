#pragma once

#include "./base_ai.hpp"
#include <random>

class MonteCarloExam : public BaseAI
{
public:
  MonteCarloExam() : BaseAI("sawa") {}
  MonteCarloExam(GameState ParentState, GameContext ParentContext) : BaseAI("sawa")
  {
    state = ParentState;
    context = ParentContext;
  }

  void run(map<Edge, int>& scores)
  {
    Edges unused;
    each (src, context.node) {
      each (dst, context.g[src]) {
        const Edge e(src, dst);
        if (state.IsFree(e)) unused.push_back(e);
      }
    }
    random_shuffle(unused.begin(), unused.end());

    Edges adj = Adjacence();
    if (adj.empty()) return ;
    
    while (unused.size() && adj.size()) {
      {
        random_shuffle(adj.begin(), adj.end());
        while (adj.size() && !state.IsFree(adj.back())) adj.pop_back();
        if (adj.empty()) break;
        const Edge e = adj.back();
        adj.pop_back();
        _Move move(context.punter_id);
        move.push_back(e.first);
        move.push_back(e.second);
        Edges more = state.connected.count(e.first) ? Adjacence(e.second) : Adjacence(e.first);
        adj.insert(adj.end(), more.begin(), more.end());
        state.Update(move, context.punter_id);
      }
      for (int i = 1, j; i < context.num_punters; ++i) {
        for (j = 0; j < unused.size(); ++j) {
          if (state.IsFree(unused[j])) {
            _Move move(context.punter_id);
            move.push_back(adj.back().first);
            move.push_back(adj.back().second);
            state.Update(move, NULL_PUNTER_ID);
            break;
          }
        }
      }
      auto itr = remove_if(unused.begin(), unused.end(), [&] (Edge e) { return !state.IsFree(e); });
      if (itr != unused.end()) {
        unused.erase(itr);
      }
    }

    const lli score = GetCurrentScore();
    each (e, state.owned) {
      UnionFind uf;
      each (f, state.owned) {
        if (e != f) {
          uf.merge(f.first, f.second);
        }
      }
      const UnionFind tmp = state.uf;
      state.uf = uf;
      scores[e] += score - GetCurrentScore();
      state.uf = tmp;
    }

    // cerr << "---------------------" << endl;
    // each (e, state.owned) cerr << e << endl;
    
    return ;
  }
};

class AI : public BaseAI
{
public:
  AI () : BaseAI("sawa42")
  {
  }

  Edge MonteCarlo(int num_exam = 50)
  {
    map<Edge, int> scores;
    while (num_exam--) {
      MonteCarloExam(state, context).run(scores);
    }

    Edge mx(NULL_NODE_ID, NULL_NODE_ID);
    each (i, scores) {
      if (scores[mx] < i.second) {
        mx = i.first;
      }
    }
    if (mx.first == mx.second) {
      return FreeEdge();
    }
    return mx;
  }

  virtual _Move _Think(const _Moves& moves) override
  {
    _Move move(context.punter_id);
    const Edge e = MonteCarlo();
    assert(e.first != NULL_NODE_ID && e.second != NULL_NODE_ID);
    move.push_back(e.first);
    move.push_back(e.second);
    return move;
  }
};
