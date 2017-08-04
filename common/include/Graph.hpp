#ifndef INCLUDED_GRAPH
#define INCLUDED_GRAPH

#include<vector>

using namespace std;

typedef pair<int, int> Edge;

typedef struct {
    vector<Edge> edges;
    vector<int> mines;
} Graph;
#endif
