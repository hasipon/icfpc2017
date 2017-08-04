#pragma once

#include <vector>
#include <map>

#include "../include/Graph.hpp"

typedef vector<vector<int>> _graph;

pair<map<int, int>, _graph> normalize_graph(const Graph& g)
{
  map<int, int> conv;
  for (auto& i: g.mines) {
    if (conv.count(i)) {
      const int n = conv.size();
      conv[i] = n;
    }
  }

  _graph h(conv.size());
  for (auto& e: g.edges) {
    h[conv[e.first]].emplace_back(conv[e.second]);
  }

  return make_pair(conv, h);
}
