#pragma once

#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <sstream>

const int INF = 1<<30;

struct AI {
	int punter_id, N, M;
	bool splurges;
	vector<int> mines;
	vector<int> rev;
	vector<vector<int>> D;
	map<pair<int,int>, int> E;
	int mode;
	vector<int> param;

	vector<int> Think(const Moves& moves, const string& state) {
		Load(moves, state);

		vector<vector<int>> G(N);
		vector<pair<int,int>> E1;
		for (auto p : E) {
			int x = p.first.first;
			int y = p.first.second;
			if (p.second == -1) {
				G[x].push_back(y);
				G[y].push_back(x);
				E1.push_back(p.first);
			}
		}

		if (splurges) {
			if (param.size() > 0 && param[0] == 1) {
				param = {0};
				if (E1.size() > 0) {
					auto p = E1[rand() % E1.size()];
					int x = p.first, y = p.second;
					if (rand() % 2) swap(x, y);
					vector<int> a;
					for (int q : G[y]) if (q != x) a.push_back(q);
					if (a.size() > 0) {
						int z = a[rand() % a.size()];
						return {rev[x], rev[y], rev[z]};
					} else {
						return {rev[x], rev[y]};
					}
				}
			} else {
				param = {1};
				// passする
			}
		} else {
			if (E1.size() > 0) {
				auto p = E1[rand() % E1.size()];
				return {rev[p.first], rev[p.second]};
			}
		}

		return {};
	}
	vector<pair<int,int>> Future() {
		return {};
	}
	void Init(int punter_id, int num_of_punters, const Graph& g, bool futures, bool splurges) {
		map<int,int> idx;
		this->punter_id = punter_id;
		this->splurges = splurges;
		N = 0;
		for (auto x : g.edges) {
			if (!idx.count(x.first)) { idx[x.first] = N++; rev.push_back(x.first); }
			if (!idx.count(x.second)) { idx[x.second] = N++; rev.push_back(x.second); }
			int a = idx[x.first];
			int b = idx[x.second];
			if (a > b) swap(a, b);
			E[{a, b}] = -1;
		}
		vector<vector<int>> G(N);
		for (auto x : g.edges) {
			int a = idx[x.first];
			int b = idx[x.second];
			if (a > b) swap(a, b);
			G[a].push_back(b);
			G[b].push_back(a);
		}
		for (auto x : g.mines) {
			if (!idx.count(x)) continue;
			mines.push_back(idx[x]);
		}
		M = mines.size();
		
		D.resize(M, vector<int>(N, INF));
		for (int i = 0; i < M; ++ i) {
			queue<int> Q;
			D[i][mines[i]] = 0;
			Q.push(mines[i]);
			while (!Q.empty()) {
				int x = Q.front(); Q.pop();
				int dx = D[i][x];
				for (int y : G[x]) if (D[i][y] == INF) {
					D[i][y] = dx+1;
					Q.push(y);
				}
			}
		}
		mode = 0;
	}
	string Name() {
		return "hasi_random";
	}
	int PunterId() {
		return punter_id;
	}
	void Load(const Moves& moves, const string& state) {
		map<int,int> idx;
		istringstream iss(state);
		int spl;
		iss >> punter_id >> N >> M >> spl;
		splurges = (spl != 0);
		mines = vector<int>(M);
		for (int i = 0; i < M; ++ i) iss >> mines[i];
		rev = vector<int>(N);
		for (int i = 0; i < N; ++ i) {
			iss >> rev[i];
			idx[rev[i]] = i;
		}
		D.resize(M, vector<int>(N));
		for (int i = 0; i < M; ++ i) for (int j = 0; j < N; ++ j) iss >> D[i][j];
		{
			int n; iss >> n;
			for (int i = 0; i < n; ++ i) {
				int x, y, z;
				iss >> x >> y >> z;
				E[{x,y}] = z;
			}
		}
		iss >> mode;
		{
			int n; iss >> n;
			for (int i = 0; i < n; ++ i) {
				int x;
				iss >> x;
				param.push_back(x);
			}
		}

		for (auto x : moves) {
			for (int i = 1; i < x.route.size(); ++ i) {
				int a = idx[x.route[i-1]];
				int b = idx[x.route[i]];
				if (a > b) swap(a, b);
				E[{a, b}] = x.punter_id;
			}
		}
	}
	string State() {
		ostringstream oss;
		oss << punter_id << " " << N << " " << M << " " << (splurges ? 1 : 0) << " ";
		for (int i = 0; i < M; ++ i) oss << mines[i] << " ";
		for (int i = 0; i < N; ++ i) oss << rev[i] << " ";
		for (int i = 0; i < M; ++ i) for (int j = 0; j < N; ++ j) oss << D[i][j] << " ";
		oss << E.size() << " ";
		for (const auto& x : E) {
			oss << x.first.first << " " << x.first.second << " " << x.second << " ";
		}
		oss << mode << " ";
		oss << param.size() << " ";
		for (int x : param) oss << x << " ";
		return oss.str();
	}
};
