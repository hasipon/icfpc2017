#pragma once

#include <vector>
#include <map>

#include "../include/Graph.hpp"

typedef vector<vector<int>> _graph;

// class _graph : public vector<vector<int>>
// {
// public:
//   _graph() : vector<vector<int>>()
//   {
//   }
//   _graph(int size) : vector<vector<int>>(size)
//   {
//   }
//   size_t degree(int index) const
//   {
//     if (this->size() <= index) {
//       return -1;
//     }
//     return at(index).size();
//   }
// };

pair<map<int, int>, _graph> normalize_graph(const Graph& g)
{
  map<int, int> conv;
  map<int, int> rev;
  for (auto& i: g.mines) {
    if (conv.count(i) == 0) {
      const int n = conv.size();
      conv[i] = n;
      rev[n] = i;
    }
  }

  _graph h(conv.size());
  for (auto& e: g.edges) {
    int a = conv[e.first];
    int b = conv[e.second];
    h[a].emplace_back(b);
    h[b].emplace_back(a);
  }

  return make_pair(rev, h);
}
