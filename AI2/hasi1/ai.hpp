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
	int punter_id, num_of_punters;
	vector<Edge> edges;
	vector<int> mines;
	vector<Moves> moves;

	int N, M;
	map<int,int> idx;
	vector<int> rev;
	set<pair<int,int>> S;
	vector<vector<int>> G;
	vector<vector<int>> D;
	UnionFind UF;

	Move Think(const Moves& moves, const string& state) {
		Load(moves, state);

		N = 0;
		for (auto x : edges) {
			if (!idx.count(x.first)) { idx[x.first] = N++; rev.push_back(x.first); }
			if (!idx.count(x.second)) { idx[x.second] = N++; rev.push_back(x.second); }
			int a = idx[x.first];
			int b = idx[x.second];
			if (a > b) swap(a, b);
			S.insert({a, b});
		}
		G.resize(N);
		for (auto x : edges) {
			int a = idx[x.first];
			int b = idx[x.second];
			if (a > b) swap(a, b);
			G[a].push_back(b);
			G[b].push_back(a);
		}
		for (auto x : mines) {
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
		UF = UnionFind(N);

		for (const auto& mm : this->moves) {
			for (auto x : mm) {
				int a = idx[x.source];
				int b = idx[x.target];
				if (a > b) swap(a, b);
				S.erase({a, b});
			}
		}

		Move r;
		long long score = get_score(UF);
		for (auto x : S) {
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
	void Init(int punter_id, int num_of_punters, const Graph& g) {
		this->punter_id = punter_id;
		this->num_of_punters = num_of_punters;
		edges = g.edges;
		mines = g.mines;
	}
	string Name() {
		return "hasi";
	}
	int PunterId() {
		return punter_id;
	}
	void Load(const Moves& moves, const string& state) {
		istringstream iss(state);
		iss >> punter_id >> num_of_punters;
		{
			int n; iss >> n;
			for (int i = 0; i < n; ++ i) {
				int x, y;
				iss >> x >> y;
				edges.push_back({x,y});
			}
		}
		{
			int n; iss >> n;
			for (int i = 0; i < n; ++ i) {
				int x;
				iss >> x;
				mines.push_back(x);
			}
		}
		{
			int n; iss >> n;
			for (int i = 0; i < n; ++ i) {
				int m; iss >> m;
				Moves mm;
				for (int j = 0; j < m; ++ j) {
					int x, y, z, w;
					iss >> x >> y >> z >> w;
					Move v;
					v.punter_id = x;
					v.source = y;
					v.target = z;
					v.is_pass = (w != 0);
					mm.push_back(v);
				}
				this->moves.push_back(mm);
			}
		}
		this->moves.push_back(moves);
	}
	string State() {
		ostringstream oss;
		oss << punter_id << " " << num_of_punters << " " << edges.size() << " ";
		for (auto x : edges) oss << x.first << " " << x.second << " ";
		oss << mines.size() << " ";
		for (auto x : mines) oss << x << " ";
		oss << moves.size() << " ";
		for (const auto& x : moves) {
			oss << x.size() << " ";
			for (const auto& y : x) {
				if (y.is_pass) {
					oss << y.punter_id << " 0 0 1 ";
				} else {
					oss << y.punter_id << " " << y.source << " " << y.target << " 0 ";
				}
			}
		}
		return oss.str();
	}
};
