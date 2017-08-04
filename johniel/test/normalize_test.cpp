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
#include "../../common/algorithms/normalize_graph.hpp"

int main(int argc, char *argv[])
{
  Graph g;
  for (int i = 0; i < 5; ++i) {
    g.mines.push_back(i * 10);
  }
  auto fn = [&] (int a, int b) {
    g.edges.push_back(make_pair(a, b));
  };
  fn(0, 10);
  fn(10, 20);
  fn(0, 20);
  fn(20, 30);
  fn(40, 00);

  _graph y(g.mines.size());
  each (e, g.edges) {
    y[e.first / 10].push_back(e.second / 10);
    y[e.second / 10].push_back(e.first / 10);
  }

  _graph x = normalize_graph(g).second;
  _graph z = x;

  auto show = [] (_graph g) {
    for (int i = 0; i < g.size(); ++i) {
      cout << i << ": ";
      each (j, g[i]) {
        cout << j << ' ' ;
      }
      cout << endl;
    }
  };

  // show(y);
  // show(z);
  assert(z == y);
  
  puts("OK");
  return 0;
}
