#pragma once

#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <sstream>

#include "cut_info.hpp"

const int64_t INF = 1LL<<60;

struct UnionFind {
	vector<int64_t> data;
	UnionFind() {}
	UnionFind(int64_t n) : data(n, -1) {}
	int64_t root(int64_t x) {
		return data[x] < 0 ? x : data[x] = root(data[x]);
	}
	bool merge(int64_t x, int64_t y) {
		x = root(x);
		y = root(y);
		if (x == y) return false;
		if (data[y] < data[x]) swap(x, y);
		data[x] += data[y];
		data[y] = x;
		return true;
	}
};

enum ParamHeader {
	DST0,
};

struct AI {
	int64_t punter_id, N, M, remain_turn;
	vector<int64_t> mines;
	vector<int64_t> rev;
	vector<vector<int64_t>> D;
	map<pair<int64_t,int64_t>, int64_t> E;
	map<pair<int64_t,int64_t>, int64_t> E_opt;
	bool splurges, options;
	bool use_option = false;

	int64_t mode;
	vector<int64_t> param;

	vector<int64_t> ThinkM() {
		vector<bool> is_mine_or_mine(N);
		for (int64_t i = 0; i < M; ++ i) is_mine_or_mine[mines[i]] = true;

		UnionFind uf0(N);
		vector<pair<int64_t,int64_t>> E1;
		for (auto p : E) {
			int64_t x = p.first.first;
			int64_t y = p.first.second;
			if (p.second == punter_id) {
				uf0.merge(x, y);
				is_mine_or_mine[x] = true;
				is_mine_or_mine[y] = true;
			} else if (E_opt.count(p.first) && E_opt[p.first] == punter_id) {
				uf0.merge(x, y);
				is_mine_or_mine[x] = true;
				is_mine_or_mine[y] = true;
			} else if (p.second == -1 || !E_opt.count(p.first)) {
				E1.push_back(p.first);
			}
		}

		long long score = -1;
		int64_t cnt = 1;
		pair<int64_t,int64_t> sel;
		for (auto p : E1) if (is_mine_or_mine[p.first] || is_mine_or_mine[p.second]) {
			auto uf = uf0;
			uf.merge(p.first, p.second);
			auto s = get_score(uf);
			if (s > score || (s == score && rand() % ++cnt == 0)) {
				sel = p;
				if (s > score) cnt = 1;
				score = s;
			}
		}

		if (score >= 0) {
			if (E[sel] != -1) {
				use_option = true;
			} else {
			}
			vector<int64_t> r = {rev[sel.first], rev[sel.second]};
			return r;
		}
		return {};
	}
	vector<int64_t> Think(const Moves& moves, const string& state) {
		Load(moves, state);

		if (options && 0 <= remain_turn && remain_turn < M) {
			return ThinkM();
		}

		vector<vector<pair<int64_t,int64_t>>> G(N);
		map<int64_t, int64_t> site_panter_count;
		vector<pair<int64_t,int64_t>> E1;
		set<int64_t> mySite, myMine;

		vector<bool> is_mine(N);
		for (int64_t i = 0; i < M; ++ i) is_mine[mines[i]] = true;

		for (auto p : E) {
			int64_t x = p.first.first;
			int64_t y = p.first.second;
			if (p.second == punter_id) {
				G[x].push_back({y, 0});
				G[y].push_back({x, 0});
				mySite.insert(y);
				mySite.insert(x);
				if(is_mine[y]) myMine.insert(y);
				if(is_mine[x]) myMine.insert(x);
			} else if (p.second == -1) {
				G[x].push_back({y, 1});
				G[y].push_back({x, 1});
				E1.push_back(p.first);
			}else {
                site_panter_count[x]++;
				site_panter_count[y]++;
			}
		}
		vector<int64_t> dist_from_mine(N, INF);
		calc_dist_from_my_mine(dist_from_mine, mines, G);

		vector<int64_t> r;
		if (mode == 0) {
			map<pair<int64_t, int64_t>, int64_t> cuts = cut::calcMinCutForThink(G, mines);

			int64_t minicost = INF, minidist = INF;
			int64_t cnt = 1, src = -1, dst = -1;
			for (int64_t i = 0; i < M; ++ i) {
				int64_t m = mines[i];
				vector<int64_t> dist(N, INF);
				priority_queue<pair<int64_t,int64_t>> Q;
				dist[m] = 0;
				Q.push({0,m});
				while (!Q.empty()) {
					int64_t d = -Q.top().first;
					int64_t v = Q.top().second;
					Q.pop();
					if (dist[v] != d) continue;
					if (d > 0 && is_mine[v]) {
						// cut x dist
						int64_t cut_x_dist = d * cuts[make_pair(m, v)];
						if (cut_x_dist < minicost) {
							src = m; dst = v;
							minicost = cut_x_dist;
							minidist = d;
						}
					}
					for (auto p : G[v]) {
						int64_t dd = d + p.second;
						if (dd < dist[p.first]) {
							dist[p.first] = dd;
							Q.push({-dd, p.first});
						}
					}
				}
			}
			if (minicost == INF) {
				mode = 1;
			} else {
				vector<int64_t> dist1(N, INF);
				vector<int64_t> dist2(N, INF);
				calc_dist(src, dst, dist1, dist2, minidist, G);
				pair<pair<int64_t, int64_t>, vector<int64_t>> kouho = make_pair(make_pair(INF, INF), vector<int64_t>());
				int64_t cc = 0;
				for (auto e : E1) {
					int64_t x = e.first;
					int64_t y = e.second;
					if (dist1[x] == INF || dist1[y] == INF || dist2[x] == INF || dist2[y] == INF) continue;
					if (dist1[x] + dist2[y] + 1 == minidist || dist1[y] + dist2[x] + 1 == minidist) {
						// 小さいほどよい
						pair<int64_t, int64_t> hyoka = make_pair(0, 0);

						if(is_mine[x] || is_mine[y]){
							hyoka.first = -INF;
						}else{
							hyoka.first -= site_panter_count[x] + site_panter_count[y];
						}
						hyoka.second = min(dist_from_mine[x], dist_from_mine[y]);

						if(hyoka > kouho.first)continue;
						cc++;
						if(hyoka < kouho.first){
							kouho = make_pair(hyoka, vector<int64_t>{rev[x], rev[y]});
							continue;
						}
						if (rand()%2)
							kouho = make_pair(hyoka, vector<int64_t>{rev[x], rev[y]});
					}
				}
				r = kouho.second;
				if (cc == 0) throw 1;
			}
		}

		if (mode == 1) {
			UnionFind uf1(N);
			UnionFind uf2(N);
			for (auto p : E) {
				int64_t x = p.first.first;
				int64_t y = p.first.second;
				if (p.second == punter_id) {
					uf1.merge(x, y);
					uf2.merge(x, y);
				} else if (p.second == -1) {
					uf1.merge(x, y);
				}
			}
			vector<long long> gain(N);
			vector<int64_t> base(N);
			for (int64_t i = 0; i < N; ++ i) {
				for (int64_t j = 0; j < M; ++ j) {
					if (uf2.root(mines[j]) == uf2.root(i)) continue;
					if (uf1.root(mines[j]) == uf1.root(i)) {
						int64_t x = D[j][i];
						gain[i] += x*x;
						base[i] = mines[j];
					}
				}
			}
			int64_t max_gain = -1, cnt = 1, dst = -1, mini_dist = INF;
			/*
			if (param.size() >= 1 && param[0] == DST0) {
				int64_t max_gain0 = param[1];
				int64_t dst0 = param[2];
				if (gain[dst0] == max_gain0) {
					max_gain = max_gain0;
					dst = dst0;
				}
			}*/
			if (max_gain == -1) {
				for (int64_t i = 0; i < N; ++ i) {
					if (gain[i] > max_gain || (gain[i] == max_gain && dist_from_mine[i] < mini_dist)){
						if(dist_from_mine[i] == INF){
							continue;
						}
						dst = i;
						if (gain[i] > max_gain) cnt = 1;
						max_gain = gain[i];
						mini_dist = dist_from_mine[i];
					}
				}
			}
			if (max_gain <= 0) {
				mode = 2;
			} else {
				param = {DST0, max_gain, dst};
				int64_t src = base[dst];
				vector<int64_t> dist1(N, INF);
				vector<int64_t> dist2(N, INF);
				calc_dist(src, dst, dist1, dist2, INF, G);
				pair<pair<int64_t, int64_t>, vector<int64_t>> kouho = make_pair(make_pair(INF, INF), vector<int64_t>());
				int64_t cc = 0;
				int64_t dd = dist1[dst] - 1;
				for (auto e : E1) {
					int64_t x = e.first;
					int64_t y = e.second;
					if ((dist1[x] == 0 && dist2[y] == dd) || (dist1[y] == 0 && dist2[x] == dd)) {
						// 小さいほどよい
						pair<int64_t, int64_t> hyoka = make_pair(0, 0);

						if(is_mine[x] || is_mine[y]){
							hyoka.first = -INF;
						}else{
							hyoka.first -= site_panter_count[x] + site_panter_count[y];
						}
						hyoka.second = min(dist_from_mine[x], dist_from_mine[y]);

						if(hyoka > kouho.first)continue;
						cc++;
						if(hyoka < kouho.first){
							kouho = make_pair(hyoka, vector<int64_t>{rev[x], rev[y]});
							continue;
						}
						if (rand()%2)
							kouho = make_pair(hyoka, vector<int64_t>{rev[x], rev[y]});
					}
				}
				r = kouho.second;
				if (cc == 0) throw 1;
			}
		}

		if (mode == 2) {
			if (E1.size() > 0) {
				auto p = E1[rand() % E1.size()];
				r = {rev[p.first], rev[p.second]};
			}
		}

		return r;
	}

	void calc_dist_from_my_mine(vector<int64_t>& dist, vector<int64_t> myMine, const vector<vector<pair<int64_t,int64_t>>>& G) {
		{
			priority_queue<pair<int64_t, int64_t>> Q;
			for(auto src : myMine){
				dist[src] = 0;
				Q.push({0, src});
			}
			while (!Q.empty()) {
				int64_t d = -Q.top().first;
				int64_t v = Q.top().second;
				Q.pop();
				if (dist[v] != d) continue;
				if (d > remain_turn) break;
				for (auto p : G[v]) {
					int64_t dd = d + p.second;
					if (dd < dist[p.first]) {
						dist[p.first] = dd;
						Q.push({-dd, p.first});
					}
				}
			}
		}
	}

	void calc_dist(int64_t src, int64_t dst, vector<int64_t>& dist1, vector<int64_t>& dist2, int64_t mindist, const vector<vector<pair<int64_t,int64_t>>>& G) {
		dist1[src] = 0;
		{
			priority_queue<pair<int64_t,int64_t>> Q;
			Q.push({0,src});
			while (!Q.empty()) {
				int64_t d = -Q.top().first;
				int64_t v = Q.top().second;
				Q.pop();
				if (dist1[v] != d) continue;
				if (d > mindist) break;
				for (auto p : G[v]) {
					int64_t dd = d + p.second;
					if (dd < dist1[p.first]) {
						dist1[p.first] = dd;
						Q.push({-dd, p.first});
					}
				}
			}
		}
		dist2[dst] = 0;
		{
			priority_queue<pair<int64_t,int64_t>> Q;
			Q.push({0,dst});
			while (!Q.empty()) {
				int64_t d = -Q.top().first;
				int64_t v = Q.top().second;
				Q.pop();
				if (dist2[v] != d) continue;
				if (d > mindist) break;
				for (auto p : G[v]) {
					int64_t dd = d + p.second;
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
		for (int64_t i = 0; i < M; ++ i) {
			int64_t m = mines[i];
			for (int64_t j = 0; j < N; ++ j) if (D[i][j] != INF) {
					if (uf.root(m) == uf.root(j)) {
						int64_t x = D[i][j];
						r += x*x;
					}
				}
		}
		return r;
	}
	vector<pair<int64_t,int64_t>> Future() {
		return {};
	}
	void Init(int64_t punter_id, int64_t num_of_punters, const Graph& g, bool futures, bool splurges) {
		remain_turn = g.edges.size() / num_of_punters;
		map<int64_t,int64_t> idx;
		this->punter_id = punter_id;
		this->splurges = splurges;
		N = 0;
		for (auto x : g.edges) {
			if (!idx.count(x.first)) { idx[x.first] = N++; rev.push_back(x.first); }
			if (!idx.count(x.second)) { idx[x.second] = N++; rev.push_back(x.second); }
			int64_t a = idx[x.first];
			int64_t b = idx[x.second];
			if (a > b) swap(a, b);
			E[{a, b}] = -1;
		}
		vector<vector<int64_t>> G(N);
		for (auto x : g.edges) {
			int64_t a = idx[x.first];
			int64_t b = idx[x.second];
			if (a > b) swap(a, b);
			G[a].push_back(b);
			G[b].push_back(a);
		}
		for (auto x : g.mines) {
			if (!idx.count(x)) continue;
			mines.push_back(idx[x]);
		}
		M = mines.size();

		D.resize(M, vector<int64_t>(N, INF));
		for (int64_t i = 0; i < M; ++ i) {
			queue<int64_t> Q;
			D[i][mines[i]] = 0;
			Q.push(mines[i]);
			while (!Q.empty()) {
				int64_t x = Q.front(); Q.pop();
				int64_t dx = D[i][x];
				for (int64_t y : G[x]) if (D[i][y] == INF) {
						D[i][y] = dx+1;
						Q.push(y);
					}
			}
		}
		mode = 0;
	}

	string Name() {
		return "DiamondPrincess8.99";
	}

	int64_t PunterId() {
		return punter_id;
	}

	void Load(const Moves& moves, const string& state) {
		map<int64_t,int64_t> idx;
		istringstream iss(state);
		int64_t spl, opt;
		iss >> punter_id >> N >> M >> spl >> opt;
		splurges = (spl != 0);
		options = (opt != 0);
		mines = vector<int64_t>(M);
		for (int64_t i = 0; i < M; ++ i) iss >> mines[i];
		rev = vector<int64_t>(N);
		for (int64_t i = 0; i < N; ++ i) {
			iss >> rev[i];
			idx[rev[i]] = i;
		}
		D.resize(M, vector<int64_t>(N));
		for (int64_t i = 0; i < M; ++ i) for (int64_t j = 0; j < N; ++ j) iss >> D[i][j];
		{
			int64_t n; iss >> n;
			for (int64_t i = 0; i < n; ++ i) {
				int64_t x, y, z;
				iss >> x >> y >> z;
				E[{x,y}] = z;
			}
		}
		{
			int64_t n; iss >> n;
			for (int64_t i = 0; i < n; ++ i) {
				int64_t x, y, z;
				iss >> x >> y >> z;
				E_opt[{x,y}] = z;
			}
		}
		iss >> mode;
		{
			int64_t n; iss >> n;
			for (int64_t i = 0; i < n; ++ i) {
				int64_t x;
				iss >> x;
				param.push_back(x);
			}
		}

		for (auto x : moves) {
			for (int64_t i = 1; i < x.route.size(); ++ i) {
				int64_t a = idx[x.route[i-1]];
				int64_t b = idx[x.route[i]];
				if (a > b) swap(a, b);
				if (E[{a,b}] == -1) {
					E[{a, b}] = x.punter_id;
				} else {
					E_opt[{a, b}] = x.punter_id;
				}
			}
		}
		iss >> remain_turn;
	}

	string State() {
		ostringstream oss;
		oss << punter_id << " " << N << " " << M << " ";
		oss << (splurges ? 1 : 0) << " ";
		oss << (options ? 1 : 0) << " ";
		for (int64_t i = 0; i < M; ++ i) oss << mines[i] << " ";
		for (int64_t i = 0; i < N; ++ i) oss << rev[i] << " ";
		for (int64_t i = 0; i < M; ++ i) for (int64_t j = 0; j < N; ++ j) oss << D[i][j] << " ";
		oss << E.size() << " ";
		for (const auto& x : E) {
			oss << x.first.first << " " << x.first.second << " " << x.second << " ";
		}
		oss << E_opt.size() << " ";
		for (const auto& x : E_opt) {
			oss << x.first.first << " " << x.first.second << " " << x.second << " ";
		}
		oss << mode << " ";
		oss << param.size() << " ";
		for (int64_t x : param) oss << x << " ";
		oss << remain_turn - 1 << " ";
		return oss.str();
	}
};
