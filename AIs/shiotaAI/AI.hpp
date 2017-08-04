#ifndef INCLUDED_AI
#define INCLUDED_AI

#include "../../common/include/Graph.hpp"
#include "../../common/include/Move.hpp"

#include<string>

using namespace std;

class AI {
    public:
        Graph m_Graph;
        void Init(int punter_id, int num_of_punters, const Graph g);
        Move Think(const Moves & moves);
};
#endif
