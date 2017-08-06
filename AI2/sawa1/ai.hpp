#pragma once

#include "./base_ai.hpp"
#include <random>

class MonteCarloExam : public BaseAI
{
public:
  MonteCarloExam() : BaseAI("sawa1") {}
  MonteCarloExam(GameState ParentState, GameContext ParentContext) : BaseAI("sawa1")
  {
    state = ParentState;
    context = ParentContext;
  }

  void run(map<Edge, int>& score)
  {
    vector<Edge> candidates;
    each (src, context.mines) {
      each (dst, context.g[src]) {
        if (state.connected.count(dst)) continue;
        const Edge e(src, dst);
        if (state.used.count(e)) continue;
        candidates.push_back(e);
      }
    }
    each (src, state.connected) {
      each (dst, context.g[src]) {
        if (state.connected.count(dst)) continue;
        const Edge e(src, dst);
        if (state.used.count(e)) continue;
        candidates.push_back(e);
      }
    }

    while (candidates.size()) {
      random_shuffle(candidates.begin(), candidates.end());
      each (e, candidates) {
        if (state.connected.count(e.first) != state.connected.count(e.second)) {
          state.Update(e);
          break;
        }
      }
      Moves moves;
      each (e, candidates) {
        unless (state.used.count(e)) {
          moves.push_back(Move(e));
        }
        if (moves.size() + 1 == context.num_punters) {
          state.Update(moves);
          break;
        }
      }
      auto itr = remove_if(candidates.begin(), candidates.end(), [&]  (Edge e) { return state.used.count(e); });
      candidates.erase(itr);
    }

    
    
    return ;
  }
};

class AI : public BaseAI
{
public:
  AI () : BaseAI("sawa1")
  {
  }

  Edge MonteCarlo(int num_exam = 100)
  {
    map<Edge, int> score;

    while (num_exam--) {
      MonteCarloExam(state, context).run(score);
    }

    Edge mx(-1, -1);
    each (i, score) {
      if (score[mx] < i.second) mx = i.first;
    }
    return mx;
  }

  virtual Move _Think(const Moves& moves) override
  {
    Edge e = MonteCarlo();
    return Move(e);
  }
};
