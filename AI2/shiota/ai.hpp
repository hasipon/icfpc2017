#pragma once

#include <sstream>
#include <set>
#include <iostream>

struct AI {
	string state;
    int pid;

	string Name() {
		return "shiota";
	}
	void Init(int punter_id, int num_of_punters, const Graph& gg, bool futures) {
        Graph g = gg;
		ostringstream oss;

        // fields
        pid = punter_id;

        // serilize
		oss << punter_id << ' ';

        oss << g.edges.size() << ' ';
        for(auto e : g.edges){
            oss << e.first <<' ';
            oss << e.second <<' ';
        }

        oss << g.mines.size() <<' ';
        for(auto m : g.mines){
            oss << m <<' ';
        }

        oss << g.toString() <<' ';
		state = oss.str();
	}
	Move Think(const Moves& moves, const string& state) {
        try{
        stringstream iss(state);
        ostringstream oss;
        
        iss >> pid;

        // S: 今後取りうる辺
        set<Edge> S;
        // 自分の領域
        set<int> mySite;

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
        Graph g = parseGraph(iss.str());

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
            if(mySite.count(e.first) || mySite.count(e.second)){
                int score = site_score[e.first] + site_score[e.second];
                kouho.push_back(make_pair(score, e));
            }
        }
        Edge choosed;
        if(kouho.size()){
        choosed = max_element(kouho.begin(), kouho.end())->second;
        }else{
            choosed = *S.begin();
        }
        mySite.insert(choosed.first);
        mySite.insert(choosed.second);

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

        oss << g.toString() <<' ';
		this->state = oss.str();

		return Move(choosed.first, choosed.second);
        }catch(char* s){
            cerr << s <<endl;
        }
        return Move();
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
