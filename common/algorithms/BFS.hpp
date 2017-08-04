#pragma once

#include <map>
#include <queue>
#include "../include/Graph.hpp"

std::map<int, int> bfs(const Graph& g, int source)
{
  map<int, vector<int>> es;
  queue<int> q;
  for (auto& e : g.edges) {
    es[e.first].emplace_back(e.second);
    es[e.second].emplace_back(e.first);
  }

  map<int, int> cost;
  cost[source] = 0;
  for (q.push(source); q.size(); q.pop()) {
    const auto curr = q.front();
    for (auto& next: es[curr]) {
      if (cost.count(next) == 0) {
        cost[next] = cost[curr] + 1;
        q.push(next);
      }
    }
  }
  return cost;
}
