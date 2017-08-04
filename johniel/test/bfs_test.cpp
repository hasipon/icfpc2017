#include <algorithm>
#include <complex>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

#include <cassert>
#include <cstdio>
#include <cstring>

#define each(i, c) for (auto& i : c)
#define unless(cond) if (!(cond))

using namespace std;

typedef long long int lli;
typedef unsigned long long ull;
typedef complex<double> point;

template<typename P, typename Q>
ostream& operator << (ostream& os, pair<P, Q> p)
{
  os << "(" << p.first << "," << p.second << ")";
  return os;
}

#include "../../common/algorithms/BFS.hpp"
#include "../../common/algorithms/bridge.hpp"

int main(int argc, char *argv[])
{
  Graph g;
  for (int i = 0; i < 5; ++i) {
    g.mines.push_back(i);
  }
  g.edges.push_back(make_pair(0, 1));
  g.edges.push_back(make_pair(1, 2));
  g.edges.push_back(make_pair(0, 2));
  g.edges.push_back(make_pair(2, 3));
  g.edges.push_back(make_pair(4, 0));

  map<int, int> cost = bfs(g, 0);
  assert(cost[0] == 0);
  assert(cost[1] == 1);
  assert(cost[2] == 1);
  assert(cost[3] == 2);
  assert(cost[4] == 1);

  puts("OK");
  return 0;
}
