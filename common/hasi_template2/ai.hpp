#pragma once

#include <sstream>

struct AI {
	string state;

	string Name() {
		return "hoge";
	}
	void Init(int punter_id, int num_of_punters, const Graph& g, bool futures) {
		ostringstream oss;
		oss << punter_id;
		state = oss.str();
	}
	Move Think(const Moves& moves, const string& state) {
		this->state = state;
		return Move();
	}
	vector<pair<int,int>> Future() {
		return {};
	}
	string State() {
		return state;
	}
	int PunterId() {
		return atoi(state.c_str());
	}
};
