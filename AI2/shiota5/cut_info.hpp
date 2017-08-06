//
// Created by Tetsuya Shiota on 2017/08/06.
//

#ifndef ICFPC2017_CUT_INFO_HPP_H
#define ICFPC2017_CUT_INFO_HPP_H

#include <cassert>

using namespace std;


#define REP(i,b,n) for(int i=b;i<(int)n;i++)
#define rep(i,n)   REP(i,0,n)


namespace cut {
    const int V = 10000;
    const int INF = 1e9;

    class MyEdge {
    public:
        int to, cap, rev;

        MyEdge() {}

        MyEdge(int t, int c, int r) {
            to = t;
            cap = c;
            rev = r;
        }
    };

    class MaxFlow {
    public:
        vector<MyEdge> G[V];
        int level[V];
        int iter[V];

        MaxFlow() {
        }

        void clear() {
            rep(i, V)G[i].clear();
        }

        void addEdge(int from, int to, int cap) {
            G[from].push_back(MyEdge(to, cap, G[to].size()));
            G[to].push_back(MyEdge(from, 0, G[from].size() - 1));
        }

        void BFS(int s) {
            rep(i, V)level[i] = -1;
            queue<int> Q;
            level[s] = 0;
            Q.push(s);
            while (!Q.empty()) {
                int v = Q.front();
                Q.pop();
                rep(i, (int) G[v].size()) {
                    MyEdge e = G[v][i];
                    if (e.cap > 0 && level[e.to] < 0) {
                        level[e.to] = level[v] + 1;
                        Q.push(e.to);
                    }
                }
            }
        }

        int DFS(int v, int t, int f) {
            if (v == t) {
                return f;
            }
            REP(i, iter[v], (int) G[v].size()) {
                MyEdge e = G[v][i];
                iter[v] = i;
                if (e.cap > 0 && level[v] < level[e.to]) {
                    int d = DFS(e.to, t, min(f, e.cap));
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
        int solve(int s, int t) {
            int flow = 0;
            while (1) {
                BFS(s);
                if (level[t] < 0)return flow;
                rep(i, V)iter[i] = 0;
                int f;
                while ((f = DFS(s, t, INF)) > 0) {
                    flow += f;
                }
            }
            return flow;
        }

        int putFlow(int s, int t, int f) {
            int flow = 0;
            BFS(s);
            if (level[t] < 0)return flow;
            rep(i, V)iter[i] = 0;
            return DFS(s, t, f);
        }

        //辺(s, t)のcapaを一つ減らす
        //減らしても流し直すことが出来ればtrueを返す
        //verified livearchive 4957
        bool decreaseCapa(int s, int t) {
            int id = -1;
            rep(i, G[s].size()) {
                if (G[s][i].to == t)id = i;
            }
            assert(id > 0);
            MyEdge &e = G[s][id];
            if (e.cap > 0) {
                e.cap--;
                return true;
            }
            int res = putFlow(s, t, 1);

            if (res == 1) {
                e.cap--;
                return true;
            }
            return false;
        }

    };

    map<pair<int, int>, int> calcMinCut(const vector<vector<int> > &G, const vector<int> &mines) {
        map<pair<int, int>, int> ret;
        if(G.size() >= V){
            cerr << "min cut calc too large!!" << endl;
            return ret;
        }
        MaxFlow initState;
        for(int src =0; src<G.size(); src++){
            for(auto dst : G[src]){
                initState.addEdge(src, dst, 1);
            }
        }
        for(int i =0 ; i< mines.size(); i++){
            for(int j = i+1; j<mines.size(); j++){
                MaxFlow now = initState;
                int cut = now.solve(mines[i], mines[j]);
                ret[make_pair(mines[i], mines[j])] = cut;
                ret[make_pair(mines[j], mines[i])] = cut;
                // cerr << "cut(" << i <<"," << j << ")" << ' ' << cut <<endl;
            }
        }
        return ret;
    }
    map<pair<int, int>, int> calcMinCutForThink(const vector<vector<pair<int, int> > > &G, const vector<int> &mines) {

        map<pair<int, int>, int> ret;
        if(G.size() >= V){
            cerr << "min cut calc too large!!" << endl;
            return ret;
        }
        MaxFlow initState;
        set<int> mySite;
        for(int src =0; src<G.size(); src++){
            for(auto dst : G[src]){
                if(dst.second == 0) {
                    initState.addEdge(src, dst.first, INF / 3);
                }else{
                    initState.addEdge(src, dst.first, 1);
                }
            }
        }
        for(int i =0 ; i< mines.size(); i++){
            for(int j = i+1; j<mines.size(); j++){
                MaxFlow now = initState;
                int cut = now.solve(mines[i], mines[j]);
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
