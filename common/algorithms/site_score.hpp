#pragma once

#include <cassert>

#include "./BFS.hpp"

map<int, int> site_score(const Graph& g, int mine)
{
  bool f = false;
  for (auto& i: g.mines) {
    f = f || (i == mine);
  }
  assert(f);
  
  map<int, int>&& cost = bfs(g, mine);
  for (auto& i: cost) {
    i.second = i.second * i.second;
  }
  return cost;
}
