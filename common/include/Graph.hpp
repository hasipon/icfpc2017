#ifndef INCLUDED_GRAPH
#define INCLUDED_GRAPH

#include<vector>

using namespace std;

typedef struct {
    vector<pair<int, int> > edges;
    vector<int> mines;
} Graph;
#endif
