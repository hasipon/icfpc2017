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

#include "../../common/algorithms/random_graph.hpp"
#include "../../common/algorithms/site_score.hpp"

int main(int argc, char *argv[])
{
  Graph g = random_graph(20, 30, 5);

  map<int, int> score = site_score(g, g.mines[0]);
  each (i, score) {
    cout << i << endl;
  }

  puts("OK");
  return 0;
}
