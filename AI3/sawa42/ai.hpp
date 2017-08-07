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

  void run(map<Edge, double>& scores, map<Edge, int>& cnt)
  {
    each (n, context.node) {
      random_shuffle(context.g[n].begin(), context.g[n].end());
    }

    int turn = context.node.size() / context.num_punters - state.owned.size();

    queue<Edge> q;
    for (; turn && q.size(); q.pop()) {
      const int src = q.front().second;
      if (turn == 0) break;
      each (dst, context.g[src]) {
        Edge e(src, dst);
        if (context.mines.count(dst)) continue;
        if (state.IsFree(e)) {
          q.push(e);
          state.Update(_Move(context.punter_id, e), context.punter_id);
        }
      }
    }

    const lli score = GetCurrentScore();
    each (e, state.owned) {
      scores[e] += score;
      ++cnt[e];
    }

    return ;
  }
};

class AI : public BaseAI
{
public:
  AI () : BaseAI("sawa42")
  {
  }

  Edge MonteCarlo(int num_exam = 100)
  {
    map<Edge, double> scores;
    map<Edge, int> cnt;
    while (num_exam--) {
      MonteCarloExam(state, context).run(scores, cnt);
    }

    Edge mx(NULL_NODE_ID, NULL_NODE_ID);
    each (i, scores) {
      const Edge& e = i.first;
      const double a = scores[mx] / max(1, cnt[mx]);
      const double b = scores[e] / cnt[e];
      if (state.IsFree(e) && state.IsAdjacence(context, e) && a < b) {
        mx = e;
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
