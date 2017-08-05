#pragma once

#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <sstream>

const int INF = 1<<30;

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
	int punter_id, N, M;
	vector<int> mines;
	vector<int> rev;
	vector<vector<int>> D;
	map<pair<int,int>, int> E;
	map<int,int> idx;

	Move Think(const Moves& moves, const string& state) {
		Load(moves, state);

		UnionFind UF(N);
		for (auto p : E) if (p.second == punter_id) {
			auto x = p.first;
			UF.merge(x.first, x.second);
		}

		Move r;
		long long score = -1;
		for (auto p : E) if (p.second == -1) {
			auto x = p.first;
			UnionFind uf = UF;
			uf.merge(x.first, x.second);
			auto s = get_score(uf);
			if (s > score) {
				r = Move(rev[x.first], rev[x.second]);
				score = s;
			}
		}
		cerr << score << endl;
		cerr << r.source << " " << r.target << endl;
		return r;
	}
	long long get_score(UnionFind& uf) {
		long long r = 0;
		for (int i = 0; i < M; ++ i) {
			int m = mines[i];
			for (int j = 0; j < N; ++ j) if (D[i][j] != INF) {
				if (uf.root(m) == uf.root(j)) {
					int x = D[i][j];
					r += x*x;
				}
			}
		}
		return r;
	}
	vector<pair<int,int>> Future() {
		return {};
	}
	void Init(int punter_id, int num_of_punters, const Graph& g, bool futures) {
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
	}
	string Name() {
		return "hasi";
	}
	int PunterId() {
		return punter_id;
	}
	void Load(const Moves& moves, const string& state) {
		istringstream iss(state);
		iss >> punter_id >> N >> M;
		mines = vector<int>(M);
		for (int i = 0; i < M; ++ i) iss >> mines[i];
		rev = vector<int>(N);
		for (int i = 0; i < N; ++ i) iss >> rev[i];
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
		{
			int n; iss >> n;
			for (int i = 0; i < n; ++ i) {
				int x, y;
				iss >> x >> y;
				idx[x] = y;
			}
		}

		for (auto x : moves) if (!x.is_pass) {
			int a = idx[x.source];
			int b = idx[x.target];
			if (a > b) swap(a, b);
			E[{a, b}] = x.punter_id;
		}
	}
	string State() {
		ostringstream oss;
		oss << punter_id << " " << N << " " << M << " ";
		for (int i = 0; i < M; ++ i) oss << mines[i] << " ";
		for (int i = 0; i < N; ++ i) oss << rev[i] << " ";
		for (int i = 0; i < M; ++ i) for (int j = 0; j < N; ++ j) oss << D[i][j] << " ";
		oss << E.size() << " ";
		for (const auto& x : E) {
			oss << x.first.first << " " << x.first.second << " " << x.second << " ";
		}
		oss << idx.size() << " ";
		for (const auto& x : idx) {
			oss << x.first << " " << x.second << " ";
		}
		return oss.str();
	}
};

