#pragma once

#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <sstream>

#include "cut_info.hpp"
#include "common.hpp"

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
	int mode;
	vector<int> param;

	Move Think(const Moves& moves, const string& state) {
		Load(moves, state);

		vector<vector<pair<int,int>>> G(N);
		vector<pair<int,int>> E1;
		for (auto p : E) {
			int x = p.first.first;
			int y = p.first.second;
			if (p.second == punter_id) {
				G[x].push_back({y, 0});
				G[y].push_back({x, 0});
			} else if (p.second == -1) {
				G[x].push_back({y, 1});
				G[y].push_back({x, 1});
				E1.push_back(p.first);
			}
		}

		cut::calcMinCutForThink(G, mines);

		Move r;
		if (mode == 0) {
			vector<bool> is_mine(N);
			for (int i = 0; i < M; ++ i) is_mine[mines[i]] = true;

			int mindist = INF;
			int cnt = 1, src = -1, dst = -1;
			for (int i = 0; i < M; ++ i) {
				int m = mines[i];
				vector<int> dist(N, INF);
				priority_queue<pair<int,int>> Q;
				dist[m] = 0;
				Q.push({0,m});
				while (!Q.empty()) {
					int d = -Q.top().first;
					int v = Q.top().second;
					Q.pop();
					if (dist[v] != d) continue;
					if (d > mindist) break;
					if (d > 0 && is_mine[v]) {
						if (d < mindist || (d == mindist || rand() % ++cnt == 0)) { src = m; dst = v; }
						if (d < mindist) cnt = 1;
						mindist = d;
					}
					for (auto p : G[v]) {
						int dd = d + p.second;
						if (dd < dist[p.first]) {
							dist[p.first] = dd;
							Q.push({-dd, p.first});
						}
					}
				}
			}
			if (mindist == INF) {
				mode = 1;
			} else {
				vector<int> dist1(N, INF);
				vector<int> dist2(N, INF);
				calc_dist(src, dst, dist1, dist2, mindist, G);
				int cc = 0;
				for (auto e : E1) {
					int x = e.first;
					int y = e.second;
					if (dist1[x] == INF || dist1[y] == INF || dist2[x] == INF || dist2[y] == INF) continue;
					if (dist1[x] + dist2[y] + 1 == mindist || dist1[y] + dist2[x] + 1 == mindist) {
						if (rand() % ++ cc == 0) {
							r = Move(rev[x], rev[y]);
						}
					}
				}
				cerr << "(mode=0) cc = " << cc << endl;
				if (cc == 0) throw 1;
			}
		}

		if (mode == 1) {
			UnionFind uf1(N);
			UnionFind uf2(N);
			for (auto p : E) {
				int x = p.first.first;
				int y = p.first.second;
				if (p.second == punter_id) {
					uf1.merge(x, y);
					uf2.merge(x, y);
				} else if (p.second == -1) {
					uf1.merge(x, y);
				}
			}
			vector<long long> gain(N);
			vector<int> base(N);
			for (int i = 0; i < N; ++ i) {
				for (int j = 0; j < M; ++ j) {
					if (uf2.root(mines[j]) == uf2.root(i)) continue;
					if (uf1.root(mines[j]) == uf1.root(i)) {
						int x = D[j][i];
						gain[i] += x*x;
						base[i] = mines[j];
					}
				}
			}
			int max_gain = -1, cnt = 1, dst = -1;
			for (int i = 0; i < N; ++ i) {
				if (gain[i] > max_gain || (gain[i] == max_gain && rand() % ++cnt == 0)) {
					dst = i;
					if (gain[i] > max_gain) cnt = 1;
					max_gain = gain[i];
				}
			}
			cerr << "max_gain = " << max_gain << endl;
			if (max_gain <= 0) {
				mode = 2;
			} else {
				int src = base[dst];
				vector<int> dist1(N, INF);
				vector<int> dist2(N, INF);
				calc_dist(src, dst, dist1, dist2, INF, G);
				int cc = 0;
				int dd = dist1[dst] - 1;
				for (auto e : E1) {
					int x = e.first;
					int y = e.second;
					if ((dist1[x] == 0 && dist2[y] == dd) || (dist1[y] == 0 && dist2[x] == dd)) {
						if (rand() % ++ cc == 0) {
							r = Move(rev[x], rev[y]);
						}
					}
				}
				cerr << "(mode=1) cc = " << cc << endl;
				if (cc == 0) throw 1;
			}
		}

		if (mode == 2) {
			if (E1.size() > 0) {
				auto p = E1[rand() % E1.size()];
				r = Move(rev[p.first], rev[p.second]);
			}
		}

		cerr << "move = " << r.source << " " << r.target << endl;
		return r;
	}
	void calc_dist(int src, int dst, vector<int>& dist1, vector<int>& dist2, int mindist, const vector<vector<pair<int,int>>>& G) {
		dist1[src] = 0;
		{
			priority_queue<pair<int,int>> Q;
			Q.push({0,src});
			while (!Q.empty()) {
				int d = -Q.top().first;
				int v = Q.top().second;
				Q.pop();
				if (dist1[v] != d) continue;
				if (d > mindist) break;
				for (auto p : G[v]) {
					int dd = d + p.second;
					if (dd < dist1[p.first]) {
						dist1[p.first] = dd;
						Q.push({-dd, p.first});
					}
				}
			}
		}
		dist2[dst] = 0;
		{
			priority_queue<pair<int,int>> Q;
			Q.push({0,dst});
			while (!Q.empty()) {
				int d = -Q.top().first;
				int v = Q.top().second;
				Q.pop();
				if (dist2[v] != d) continue;
				if (d > mindist) break;
				for (auto p : G[v]) {
					int dd = d + p.second;
					if (dd < dist2[p.first]) {
						dist2[p.first] = dd;
						Q.push({-dd, p.first});
					}
				}
			}
		}
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
		map<int,int> idx;
		this->punter_id = punter_id;
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

		cut::calcMinCut(G, mines);

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
		return "shiota5";
	}
	int PunterId() {
		return punter_id;
	}
	void Load(const Moves& moves, const string& state) {
		map<int,int> idx;
		istringstream iss(state);
		iss >> punter_id >> N >> M;
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
		oss << mode << " ";
		oss << param.size() << " ";
		for (int x : param) oss << x << " ";
		return oss.str();
	}
};
