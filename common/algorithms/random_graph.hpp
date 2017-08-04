#pragma once

#include <vector>
#include <map>
#include <set>
#include <random>

#include <cassert>

#include "../hasi_template/common.hpp"

Graph random_graph(int v, int e, int m)
{
  assert(v - 1 <= e);

  random_device seed_gen;
  mt19937 engine(seed_gen());

  vector<int> name;

  Graph g;
  int prev = -1;
  for (int i = 0; i < v; ++i) {
    int curr = engine() / 2;
    if (prev != -1) {
      g.edges.push_back(make_pair(prev, curr));
      --e;
    }
    name.push_back(curr);
    prev = curr;
  }

  set<pair<int, int>> vis;
  uniform_real_distribution<double> dist(0, name.size() - 1);
  while (e) {
    const int idx1 = dist(engine);
    const int idx2 = dist(engine);
    if (idx1 == idx2) continue;
    pair<int, int> p1 = make_pair(idx1, idx2);
    pair<int, int> p2 = make_pair(idx2, idx1);
    if (vis.count(p1) || vis.count(p2)) continue;
    g.edges.push_back(make_pair(name[idx1], name[idx2]));
    vis.insert(p1);
    vis.insert(p2);
    --e;
  }

  set<int> mine;
  while (mine.size() < m) {
    const int idx = dist(engine);
    if (mine.count(idx)) continue;
    mine.insert(idx);
    g.mines.push_back(name[idx]);
  }

  return g;
}
