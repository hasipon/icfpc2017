#pragma once

#include<iostream>

#define GV_JS
#include "gv.hpp"

using namespace std;

struct AI {
  int punter_id;
  Graph g;
  Moves moves;

  void ScaleMap() {
    double inf = 1e100;
    double min_x = inf;
    double min_y = inf;
    double max_x = -inf;
    double max_y = -inf;

    for (auto& kv : g.sites) {
      auto& s = kv.second;
      min_x = min(min_x, s.x);
      min_y = min(min_y, s.y);
      max_x = max(max_x, s.x);
      max_y = max(max_y, s.y);
    }

    if (max_x == inf) return;

    double dx = max_x - min_x;
    double dy = max_y - min_y;

    double ratio = 1;
    if (dy < dx) {
      ratio = 100.0 / dx;
    } else {
      ratio = 100.0 / dy;
    }

    for (auto& kv : g.sites) {
      auto& s = kv.second;
      s.x *= ratio;
      s.y *= ratio;
    }
  }

  void VisualizeMap() {
    for (auto& kv : g.sites) {
      const auto& s = kv.second;
      gvCircle(s.x, s.y, 0.3, gvRGB(128, 128, 128));
      gvText(s.x, s.y, 0.1, gvRGB(0, 0, 0), "%d", s.id);
    }

    for (auto& e : g.edges) {
      const auto& s1 = g.sites.at(e.first);
      const auto& s2 = g.sites.at(e.second);
      gvLine(s1.x, s1.y, s2.x, s2.y, 0.5, gvRGB(200, 200, 200));
    }

    for (auto& m : g.mines) {
      const auto& s = g.sites.at(m);
      gvCircle(s.x, s.y, 0.5, gvRGB(200, 10, 200));
    }

    for (const auto& m : moves) {
      if (m.is_pass) continue;
      const auto& s1 = g.sites.at(m.source);
      const auto& s2 = g.sites.at(m.target);

      if (this->punter_id == m.punter_id) {
        gvLine(s1.x, s1.y, s2.x, s2.y, 3, gvRGB(255, 0, 0));
      } else {
        gvLine(s1.x, s1.y, s2.x, s2.y, 1, gvRGB(0, 0, 0));
      }
    }
  }

	void Init(int punter_id, int num_of_punters, const Graph& g) {
    this->punter_id = punter_id;
    this->g = g;

    ScaleMap();
    g_gvDefaultAlpha = 128;
    gvNewTime();
    VisualizeMap();
	}

	Move Think(const Moves& moves) {
    this->moves.insert(this->moves.end(), moves.begin(), moves.end());

    gvNewTime();
    VisualizeMap();
		return Move();
	}
};
