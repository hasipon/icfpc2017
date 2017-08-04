#include "AI.hpp"

void AI::Init(int punter_id, int num_of_punters, const Graph g){
    m_Graph = g;
}

Move AI::Think(const Moves &moves){
    Move m;
    m.is_pass = true;
    
    return m;
}
