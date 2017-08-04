#include <set>

struct AI {
	Graph g;
	set<pair<int,int>> s;
	void Init(int punter_id, int num_of_punters, const Graph& g) {
		this->g = g;
		for (const auto& x : g.edges) {
			auto a = min(x.first, x.second);
			auto b = max(x.first, x.second);
			s.insert({a,b});
		}
	}
	Move Think(const Moves& moves) {
		for (const auto& x : moves) {
			if (!x.is_pass) {
				auto a = min(x.source, x.target);
				auto b = max(x.source, x.target);
				s.erase({a,b});
			}
		}
		auto p = *s.begin();
		return Move(p.first, p.second);
	}
};
