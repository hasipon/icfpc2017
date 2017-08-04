#include <map>
#include <set>
#include <queue>
#include <iostream>

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
	int N, M;
	map<int,int> idx;
	vector<int> rev;
	set<pair<int,int>> S;
	vector<vector<int>> G;
	vector<int> mines;
	vector<vector<int>> D;
	UnionFind UF;
	void Init(int punter_id, int num_of_punters, const Graph& g) {
		N = 0;
		for (auto x : g.edges) {
			if (!idx.count(x.first)) { idx[x.first] = N++; rev.push_back(x.first); }
			if (!idx.count(x.second)) { idx[x.second] = N++; rev.push_back(x.second); }
			int a = idx[x.first];
			int b = idx[x.second];
			if (a > b) swap(a, b);
			S.insert({a, b});
		}
		G.resize(N);
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
		UF = UnionFind(N);
	}
	Move Think(const Moves& moves) {
		for (auto x : moves) {
			int a = idx[x.source];
			int b = idx[x.target];
			if (a > b) swap(a, b);
			S.erase({a, b});
		}
		Move r;
		long long score = get_score(UF);
		UnionFind uf1 = UF;
		for (auto x : S) {
			UnionFind uf = UF;
			uf.merge(x.first, x.second);
			auto s = get_score(uf);
			if (s > score) {
				r = Move(rev[x.first], rev[x.second]);
				score = s;
				uf1 = uf;
			}
		}
		UF = uf1;
		cerr << score << endl;
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
};
