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

int main(int argc, char *argv[])
{
  Graph g = random_graph(20, 50, 5);

  each (e, g.edges) {
    cout << e << endl;
  }

  each (i, g.mines) {
    cout << i << endl;
  }


  puts("OK");
  return 0;
}
