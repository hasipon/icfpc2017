#pragma once

#include <sstream>

struct AI {
	string state;
	bool options;
	bool use_option;

	string Name() {
		return "hoge";
	}
	void Init(int punter_id, int num_of_punters, const Graph& g, bool futures, bool splurges) {
		ostringstream oss;
		oss << punter_id;
		state = oss.str();
	}
	vector<int> Think(const Moves& moves, const string& state) {
		this->state = state;
		return {};
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
