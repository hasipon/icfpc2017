#pragma once

#include<string>
#include<iostream>


struct AI {

    string state;

    void pre(const Moves &moves){
        /*
        for(auto move : moves){
          canUse.erase(make_pair(move.source, move.target));
          canUse.erase(make_pair(move.target, move.source));
        }
        vector<Edge> v;
        for(auto e : g.edges){
          if(used.count(e))continue;
          v.push_back(e);
        }
        */
    }

    Move Think(const Moves& moves) {
        try {
        } catch(char *s){
            cerr << "FATAL " << *s <<endl;
            return Move();
        }
        return Move();
    }

    void Init(int punter_id, int num_of_punters, const Graph& g) {
        /*
        pid = punter_id;
        V = num_of_punters;
        this->g = g;
        for(auto i : g.mines){
            myArea.insert(i);
        }
        for(auto e : g.edges){
            canUse.insert(e);
            canUse.insert(make_pair(e.second, e.first));
        }
        */
        cerr << g.toString() << endl;
        cerr << parseGraph(g.toString()).toString() <<endl;
    }
};
