//
// Created by Tetsuya Shiota on 2017/08/06.
//

#ifndef ICFPC2017_CUT_INFO_HPP_H
#define ICFPC2017_CUT_INFO_HPP_H

#include <cassert>

using namespace std;


#define REP(i,b,n) for(int64_t i=b;i<(int64_t)n;i++)
#define rep(i,n)   REP(i,0,n)


namespace cut {
    const int64_t V = 10010;
    const int64_t INF = (1LL<<60);

    class MyEdge {
    public:
        int64_t to, cap, rev;

        MyEdge() {}

        MyEdge(int64_t t, int64_t c, int64_t r) {
            to = t;
            cap = c;
            rev = r;
        }
    };

    class MaxFlow {
    public:
        vector<MyEdge> G[V];
        int64_t level[V];
        int64_t iter[V];

        MaxFlow() {
        }

        void clear() {
            rep(i, V)G[i].clear();
        }

        void addEdge(int64_t from, int64_t to, int64_t cap) {
            G[from].push_back(MyEdge(to, cap, G[to].size()));
            G[to].push_back(MyEdge(from, 0, G[from].size() - 1));
        }

        void BFS(int64_t s) {
            rep(i, V)level[i] = -1;
            queue<int64_t> Q;
            level[s] = 0;
            Q.push(s);
            while (!Q.empty()) {
                int64_t v = Q.front();
                Q.pop();
                rep(i, (int64_t) G[v].size()) {
                    MyEdge e = G[v][i];
                    if (e.cap > 0 && level[e.to] < 0) {
                        level[e.to] = level[v] + 1;
                        Q.push(e.to);
                    }
                }
            }
        }

        int64_t DFS(int64_t v, int64_t t, int64_t f) {
            if (v == t) {
                return f;
            }
            REP(i, iter[v], (int64_t) G[v].size()) {
                MyEdge e = G[v][i];
                iter[v] = i;
                if (e.cap > 0 && level[v] < level[e.to]) {
                    int64_t d = DFS(e.to, t, min(f, e.cap));
                    if (d > 0) {
                        G[v][i].cap -= d;
                        G[e.to][e.rev].cap += d;
                        return d;
                    }
                }
            }
            return 0;
        }

        //verified AOJ 2304
        int64_t solve(int64_t s, int64_t t) {
            int64_t flow = 0;
            while (1) {
                BFS(s);
                if (level[t] < 0)return flow;
                rep(i, V)iter[i] = 0;
                int64_t f;
                while ((f = DFS(s, t, INF)) > 0) {
                    flow += f;
                }
            }
            return flow;
        }

        int64_t putFlow(int64_t s, int64_t t, int64_t f) {
            int64_t flow = 0;
            BFS(s);
            if (level[t] < 0)return flow;
            rep(i, V)iter[i] = 0;
            return DFS(s, t, f);
        }

        //辺(s, t)のcapaを一つ減らす
        //減らしても流し直すことが出来ればtrueを返す
        //verified livearchive 4957
        bool decreaseCapa(int64_t s, int64_t t) {
            int64_t id = -1;
            rep(i, G[s].size()) {
                if (G[s][i].to == t)id = i;
            }
            assert(id > 0);
            MyEdge &e = G[s][id];
            if (e.cap > 0) {
                e.cap--;
                return true;
            }
            int64_t res = putFlow(s, t, 1);

            if (res == 1) {
                e.cap--;
                return true;
            }
            return false;
        }

    };

    map<pair<int64_t, int64_t>, int64_t> calcMinCut(const vector<vector<int64_t> > &G, const vector<int64_t> &mines) {
        map<pair<int64_t, int64_t>, int64_t> ret;
        if(G.size() >= V){
            cerr << "min cut calc too large!!" << endl;
            return ret;
        }
        MaxFlow initState;
        for(int64_t src =0; src<G.size(); src++){
            for(auto dst : G[src]){
                initState.addEdge(src, dst, 1);
            }
        }
        for(int64_t i =0 ; i< mines.size(); i++){
            for(int64_t j = i+1; j<mines.size(); j++){
                MaxFlow now = initState;
                int64_t cut = now.solve(mines[i], mines[j]);
                ret[make_pair(mines[i], mines[j])] = cut;
                ret[make_pair(mines[j], mines[i])] = cut;
                // cerr << "cut(" << i <<"," << j << ")" << ' ' << cut <<endl;
            }
        }
        return ret;
    }
    map<pair<int64_t, int64_t>, int64_t> calcMinCutForThink(const vector<vector<pair<int64_t, int64_t> > > &G, const vector<int64_t> &mines) {

        map<pair<int64_t, int64_t>, int64_t> ret;
        if(G.size() >= V){
            cerr << "min cut calc too large!!" << endl;
            return ret;
        }
        MaxFlow initState;
        set<int64_t> mySite;
        for(int64_t src =0; src<G.size(); src++){
            for(auto dst : G[src]){
                if(dst.second == 0) {
                    initState.addEdge(src, dst.first, INF / 3);
                }else{
                    initState.addEdge(src, dst.first, 1);
                }
            }
        }
        for(int64_t i =0 ; i< mines.size(); i++){
            for(int64_t j = i+1; j<mines.size(); j++){
                MaxFlow now = initState;
                int64_t cut = now.solve(mines[i], mines[j]);
                ret[make_pair(mines[i], mines[j])] = cut;
                ret[make_pair(mines[j], mines[i])] = cut;
                // cerr << "cut(" << i <<"," << j << ")" << ' ' << cut <<endl;
            }
        }
        return ret;
    }
    // */
}



#endif //ICFPC2017_CUT_INFO_HPP_H
