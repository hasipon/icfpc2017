#include "common.hpp"
#include "ai.hpp"
#include <iostream>
#include <cstdio>
#include <sstream>
using namespace std;

int64_t io_get_n() {
	int64_t n = 0;
	for (;;) {
		int64_t c = getchar();
		if (c == ':') break;
		if (!('0' <= c && c <= '9')) { cerr << "io_get_n " << c << " " << n << endl; throw 1; }
		n = n * 10 + (c - '0');
	}
	return n;
}

struct io_Json {
	char* buf;
	int64_t p, n;
	io_Json() {
		n = io_get_n();
		buf = new char[n+1];
		if (fread(buf, 1, n, stdin) != n) { cerr << "io_Json" << endl; throw 1; }
		buf[n] = 0;

		p = 0;
	}
	virtual ~io_Json() {
		delete[] buf;
	}
	int64_t peek() {
		if (p >= n) return -1;
		return buf[p];
	}
	void read_ws() {
		for (;;) {
			auto c = peek();
			if (!is_ws(c)) break;
			++p;
		}
	}
	bool read_bool() {
		read_ws();
		bool r = false;
		{
			auto c = peek();
			if (c == 't') {
				p += 4;
				r = true;
			} else if (c == 'f') {
				p += 5;
			} else {
				cerr << "read_bool" << c << endl; throw 1;
			}
		}
		return r;
	}
	int64_t read_number() {
		read_ws();
		bool minus = false;
		{
			auto c = peek();
			if (!(('0' <= c && c <= '9') || c == '-')) { cerr << "read_number 1" << endl; throw 1; }
			if (c == '-') {
				minus = true;
				++p;
			}
		}
		int64_t r = 0;
		for (;;) {
			auto c = peek();
			if ('0' <= c && c <= '9') {
				r = r * 10 + (c - '0');
				++p;
			} else {
				break;
			}
		}
		if (peek() == '.') {
			++p;
			for (;;) {
				auto c = peek();
				if ('0' <= c && c <= '9') ++p;
				else if (c == -1) { cerr << "read_number 2" << endl; throw 1; }
				else break;
			}
		}
		if (peek() == 'e') {
			++p;
			if (peek() == '+' || peek() == '-') ++p;
			for (;;) {
				auto c = peek();
				if ('0' <= c && c <= '9') ++p;
				else if (c == -1) { cerr << "read_number 3" << endl; throw 1; }
				else break;
			}
		}
		return minus ? -r : r;
	}
	string read_string() {
		read_ws();
		if (peek() != '"') { cerr << "read_string 1" << endl; throw 1; }
		++p;
		int64_t s = p;
		for (;;) {
			auto c = peek(); ++p;
			if (c == -1) { cerr << "read_string 2" << endl; throw 1; }
			if (c == '"') break;
			if (c == '\\') {
				auto cc = peek(); ++p;
				if (cc == 'u') {
					p += 4;
				}
			}
		}
		return string(&buf[s], p-s-1);
	}
	void read_value() {
		read_ws();
		{
			auto c = peek();
			if (('0' <= c && c <= '9') || c == '-') {
				read_number();
			} else if (c == '"') {
				read_string();
			} else if (c == '[') {
				start_array();
				for (; !end_array(); read_separator()) {
					read_value();
				}
			} else if (c == '{') {
				start_object();
				for (; !end_object(); read_separator()) {
					read_key();
					read_value();
				}
			} else if (c == 't' || c == 'n') {
				p += 4;
			} else if (c == 'f') {
				p += 5;
			} else {
				cerr << "read_value" << c << endl; throw 1;
			}
		}
	}
	void start_object() {
		read_ws();
		if (peek() != '{') { cerr << "start_object" << endl; throw 1; }
		++p;
	}
	bool end_object() {
		read_ws();
		if (peek() == '}') {
			++p;
			return true;
		} else {
			return false;
		}
	}
	void start_array() {
		read_ws();
		if (peek() != '[') { cerr << "start_array" << endl; throw 1; }
		++p;
	}
	bool end_array() {
		read_ws();
		if (peek() == ']') {
			++p;
			return true;
		} else {
			return false;
		}
	}
	void read_separator() {
		read_ws();
		if (peek() == ',') {
			++p;
		}
	}
	string read_key() {
		string r = read_string();
		read_ws();
		if (peek() != ':') { cerr << "read_key" << endl; throw 1; }
		++p;
		return r;
	}
	string read_state() {
		read_ws();
		if (peek() == '"') {
			return read_string();
		} else {
			read_value();
			return "";
		}
	}
	bool is_ws(int64_t c) {
		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
	}
};

struct io_Main {
	int64_t punter_id, num_of_punters;
	AI ai;
	Graph g;
	Moves moves;
	string state;
	bool futures = false;
	bool splurges = false;
	bool options = false;

	void write(const string& s) {
		ostringstream ss;
		ss << s.length() << ":" << s;
		string t = ss.str();
		fwrite(t.c_str(), 1, t.length(), stdout);
		fflush(stdout);
	}

	void read_map(io_Json* s) {
		s->start_object();
		for (; !s->end_object(); s->read_separator()) {
			auto k = s->read_key();
			if (k == "rivers") {
				s->start_array();
				for (; !s->end_array(); s->read_separator()) {
					int64_t c = 0;
					int64_t source, target;
					s->start_object();
					for (; !s->end_object(); s->read_separator()) {
						auto kk = s->read_key();
						if (kk == "source") {
							source = s->read_number();
							c |= 1;
						} else if (kk == "target") {
							target = s->read_number();
							c |= 2;
						} else {
							s->read_value();
						}
					}
					if (c != 3) { cerr << "read_map" << endl; throw 1; }
					g.edges.push_back({source, target});
				}
			} else if (k == "mines") {
				s->start_array();
				for (; !s->end_array(); s->read_separator()) {
					g.mines.push_back(s->read_number());
				}
			} else {
				s->read_value();
			}
		}
	}

	void read_settings(io_Json* s) {
		s->start_object();
		for (; !s->end_object(); s->read_separator()) {
			auto k = s->read_key();
			if (k == "futures") {
				futures = s->read_bool();
			} else if (k == "splurges") {
				splurges = s->read_bool();
			} else if (k == "options") {
				options = s->read_bool();
			} else {
				s->read_value();
			}
		}
	}

	Move read_move(io_Json* s) {
		int64_t punter_id = -1;
		vector<int64_t> route;
		s->start_object();
		for (; !s->end_object(); s->read_separator()) {
			auto k = s->read_key();
			if (k == "claim" || k == "option") {
				route = vector<int64_t>(2, -1);
				s->start_object();
				for (; !s->end_object(); s->read_separator()) {
					auto kk = s->read_key();
					if (kk == "punter") {
						punter_id = s->read_number();
					} else if (kk == "source") {
						route[0] = s->read_number();
					} else if (kk == "target") {
						route[1] = s->read_number();
					} else {
						s->read_value();
					}
				}
			} else if (k == "pass") {
				s->start_object();
				for (; !s->end_object(); s->read_separator()) {
					auto kk = s->read_key();
					if (kk == "punter") {
						punter_id = s->read_number();
					} else {
						s->read_value();
					}
				}
			} else if (k == "splurge") {
				s->start_object();
				for (; !s->end_object(); s->read_separator()) {
					auto kk = s->read_key();
					if (kk == "punter") {
						punter_id = s->read_number();
					} else if (kk == "route") {
						s->start_array();
						for (; !s->end_array(); s->read_separator()) {
							route.push_back(s->read_number());
						}
					} else {
						s->read_value();
					}
				}
			} else {
				s->read_value();
			}
		}
		return Move(punter_id, route);
	}

	void run() {
		{
			ostringstream ss;
			ss << "{\"me\":\"" << ai.Name() << "\"}";
			write(ss.str());
		}
		{
			auto s = new io_Json();
			delete s;
		}
		int64_t mode = 1;
		{
			auto s = new io_Json();
			s->start_object();
			for (; !s->end_object(); s->read_separator()) {
				auto k = s->read_key();
				if (k == "punter") {
					punter_id = s->read_number();
				} else if (k == "punters") {
					num_of_punters = s->read_number();
				} else if (k == "map") {
					read_map(s);
				} else if (k == "settings") {
					read_settings(s);
				} else if (k == "move") {
					mode = 2;
					s->start_object();
					for (; !s->end_object(); s->read_separator()) {
						auto kk = s->read_key();
						if (kk == "moves") {
							s->start_array();
							for (; !s->end_array(); s->read_separator()) {
								moves.push_back(read_move(s));
							}
						} else {
							s->read_value();
						}
					}
				} else if (k == "stop") {
					mode = 3;
					break;
				} else if (k == "state") {
					state = s->read_state();
				} else {
					s->read_value();
				}
			}
			delete s;
		}
		if (mode == 3) return;
		if (mode == 1) {
			ai.options = options;
			ai.Init(punter_id, num_of_punters, g, futures, splurges);
			ostringstream ss;
			if (futures) {
				ss << "{\"ready\":" << ai.PunterId() << ",\"state\":\"" << ai.State() << "\",\"futures\":[";
				bool flag = false;
				for (auto x : ai.Future()) {
					if (flag) ss << ",";
					flag = true;
					ss << "{\"source\":" << x.first << ",\"target\":" << x.second << "\"}";
				}
				ss << "]}";
			} else {
				ss << "{\"ready\":" << ai.PunterId() << ",\"state\":\"" << ai.State() << "\"}";
			}
			write(ss.str());
		} else {
			auto move = ai.Think(moves, state);
			if (move.size() < 2) {
				ostringstream ss;
				ss << "{\"pass\":{\"punter\":" << ai.PunterId() << "},\"state\":\"" << ai.State() << "\"}";
				write(ss.str());
			} else if (move.size() == 2) {
				if (ai.use_option) {
					ostringstream ss;
					ss << "{\"option\":{\"punter\":" << ai.PunterId() << ",\"source\":" << move[0] << ",\"target\":" << move[1] << "},\"state\":\"" << ai.State() << "\"}";
					write(ss.str());
				} else {
					ostringstream ss;
					ss << "{\"claim\":{\"punter\":" << ai.PunterId() << ",\"source\":" << move[0] << ",\"target\":" << move[1] << "},\"state\":\"" << ai.State() << "\"}";
					write(ss.str());
				}
			} else {
				ostringstream ss;
				ss << "{\"splurge\":{\"punter\":" << ai.PunterId() << ",\"route\":[";
				for (int64_t i = 0; i < move.size(); ++ i) {
					if (i) ss << ",";
					ss << move[i];
				}
				ss << "]},\"state\":\"" << ai.State() << "\"}";
				write(ss.str());
			}
		}
	}
};

int main() {
    ios::sync_with_stdio(false);
	io_Main hoge;
	hoge.run();
}
