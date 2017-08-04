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

#include "../../common/include/Graph.hpp"
#include "../../common/algorithms/BFS.hpp"
#include "../../common/algorithms/bridge.hpp"

int main(int argc, char *argv[])
{
  Graph g;
  for (int i = 0; i < 8; ++i) {
    g.mines.push_back(i * 10);
  }
  auto fn = [&] (int a, int b) {
    return make_pair(a*10, b*10);
  };
  g.edges.push_back(fn(0, 1));
  g.edges.push_back(fn(1, 2));
  g.edges.push_back(fn(0, 2));
  g.edges.push_back(fn(2, 3));
  g.edges.push_back(fn(4, 0));
  g.edges.push_back(fn(4, 5));
  g.edges.push_back(fn(6, 7));

  vector<Edge> es = find_bridge(g);
  sort(es.begin(), es.end());
  each (e, es) cout << e << endl;

  auto is_included = [&] (int a, int b) {
    if (binary_search(es.begin(), es.end(), fn(a, b))) return true;
    if (binary_search(es.begin(), es.end(), fn(b, a))) return true;
    return false;
  };

  assert(is_included(6, 7));
  assert(is_included(4, 0));
  assert(is_included(4, 5));
  assert(is_included(2, 3));

  puts("OK");
  return 0;
}
