#pragma once

#include <sstream>
#include <set>
#include <iostream>

struct UnionFind {
    vector<int> data;
    UnionFind() {}
    UnionFind(int n) : data(n, -1) {}
    int root(int x) {
        return data[x] < 0 ? x : data[x] = root(data[x]);
    }
    bool merge(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) return false;
        if (data[y] < data[x]) swap(x, y);
        data[x] += data[y];
        data[y] = x;
        return true;
    }
};

struct AI {
    string state;
    int pid;

    string Name() {
        return "shiota";
    }
    void Init(int punter_id, int num_of_punters, const Graph& gg, bool futures) {
        Graph g = gg;
        ostringstream oss;
        set<int> sites;

        // fields
        pid = punter_id;

        // serilize
        oss << punter_id << ' ';
        oss << g.edges.size() << ' ';
        for(auto e : g.edges){
            oss << e.first <<' ';
            oss << e.second <<' ';
            sites.insert(e.first);
            sites.insert(e.second);
        }

        oss << g.mines.size() <<' ';
        for(auto m : g.mines){
            oss << m <<' ';
        }

        UnionFind UF(sites.size());
        oss << UF.data.size() << ' ';
        for(auto d : UF.data){
            oss << d <<' ';
        }

        oss << "Graph" <<' ';

        oss << g.toString() <<' ';
        state = oss.str();
    }
    Move Think(const Moves& moves, const string& state) {
        Move ret;
        UnionFind UF;
        // S: 今後取りうる辺
        set<Edge> S;
        // 自分の領域
        set<int> mySite;
        Graph g;
        try{
            stringstream iss(state);

            iss >> pid;


            int en;
            iss >> en;
            for(int i =0; i<en; i++){
                int a, b;
                iss >> a >> b;
                S.insert(make_pair(min(a, b), max(a,b)));
            }

            int myN;
            iss >> myN;
            for(int i = 0; i<myN; i++){
                int t;
                iss >> t;
                mySite.insert(t);
            }

            int ufSize;
            iss>> ufSize;
            UF = UnionFind(ufSize);
            for(int i = 0; i<ufSize; i++){
                iss >> UF.data[i];
            }

            string s;
            iss >> s;
            g = parseGraph(iss);


            // 入力ここまで

            for(Move m : moves){
                int a = m.source;
                int b = m.target;
                S.erase(make_pair(min(a,b), max(a,b)));
            }

            // siteのスコア計算
            map<int, int> site_score;
            for(auto s : S){
                site_score[s.first]++;
                site_score[s.second]++;
            }

            vector<pair<int, Edge> > kouho;
            for(Edge e : S){
                if(mySite.count(e.first) && mySite.count(e.second)){
                    if(UF.root(e.first) != UF.root(e.second)){
                        kouho.push_back(make_pair(10000, e));
                    }else{
                        kouho.push_back(make_pair(0, e));
                    }
                    continue;
                }
                if(mySite.count(e.first) || mySite.count(e.second)){
                    int score = site_score[e.first] + site_score[e.second];
                    kouho.push_back(make_pair(score, e));
                }
            }
            if(S.size()){
                Edge choosed;
                if(kouho.size()){
                    choosed = max_element(kouho.begin(), kouho.end())->second;
                }else{
                    choosed = *S.begin();
                }
                UF.merge(choosed.first, choosed.second);
                mySite.insert(choosed.first);
                mySite.insert(choosed.second);


                ret =  Move(choosed.first, choosed.second);
            }
        }catch(char* s){
            cerr << s <<endl;
        }
        ostringstream oss;
        // serilize
        oss << pid << ' ';
        // 今後取りうる辺
        oss << S.size() << ' ';
        for(auto e : S){
            oss << e.first <<' ';
            oss << e.second <<' ';
        }

        oss << mySite.size() <<' ';
        for(auto s : mySite){
            oss << s <<' ';
        }

        oss << UF.data.size() << ' ';
        for(auto d : UF.data){
            oss << d <<' ';
        }

        oss << "Graph" << ' ';
        oss << g.toString() <<' ';
        this->state = oss.str();
        return ret;
    }
    vector<pair<int,int>> Future() {
        return {};
    }
    string State() {
        return state;
    }
    int PunterId() {
        return pid;
    }
};
