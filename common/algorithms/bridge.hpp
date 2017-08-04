#pragma once

#include <map>
#include <vector>

#include "../include/Graph.hpp"
#include "./normalize_graph.hpp"

namespace _diamond_princess {
  void traverse(const _graph& G, int curr, int d[], int p[], int l[], int &time)
  {
    d[curr] = l[curr] = ++time;
    for (auto& next: G[curr]) {
      if (!d[next]) {
        p[next] = curr;
        traverse(G, next, d, p, l, time);
        l[curr] = min(l[curr], l[next]);
      } else if(p[curr] != next) {
        l[curr] = min(l[curr], d[next]);
      }
    }
    return ;
  }
};

vector<pair<int, int>> find_bridge(const Graph& _g)
{
  auto norm = normalize_graph(_g);
  _graph& g = get<2>(norm);
  map<int, int>& rev = get<1>(norm);

  const int N = g.size();
  int d[N], p[N], l[N], time = 0;
  vector<Edge> B;

  fill(d, d + N, false);
  fill(p, p + N, -1);

  for (int i = 0; i < N; ++i) {
    if (d[i]) continue;
    _diamond_princess::traverse(g, i, d, p, l, time);
  }
  
  for (int i = 0; i < N; ++i) {
    if (p[i] != -1 && d[i] == l[i]) {
      const int a = rev[p[i]];
      const int b = rev[i];
      B.emplace_back(make_pair(a, b));
    }
  }

  return B;
}
