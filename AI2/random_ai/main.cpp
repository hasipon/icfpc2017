#include "common.hpp"
#include "ai.hpp"
#include <iostream>
#include <cstdio>
#include <sstream>
using namespace std;

int io_get_n() {
	int n = 0;
	for (;;) {
		int c = getchar();
		if (c == ':') break;
		if (!('0' <= c && c <= '9')) { cerr << "io_get_n " << c << " " << n << endl; throw 1; }
		n = n * 10 + (c - '0');
	}
	return n;
}

struct io_Json {
	char* buf;
	int p, n;
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
	int peek() {
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
	int read_number() {
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
		int r = 0;
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
		int s = p;
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
	bool is_ws(int c) {
		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
	}
};

struct io_Main {
	int punter_id, num_of_punters;
	AI ai;
	Graph g;
	Moves moves;
	string state;
	bool futures = false;

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
					int c = 0;
					int source, target;
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
			} else {
				s->read_value();
			}
		}
	}

	Move read_move(io_Json* s) {
		Move r;
		s->start_object();
		for (; !s->end_object(); s->read_separator()) {
			auto k = s->read_key();
			if (k == "claim") {
				r.is_pass = false;
				s->start_object();
				for (; !s->end_object(); s->read_separator()) {
					auto kk = s->read_key();
					if (kk == "punter") {
						r.punter_id = s->read_number();
					} else if (kk == "source") {
						r.source = s->read_number();
					} else if (kk == "target") {
						r.target = s->read_number();
					} else {
						s->read_value();
					}
				}
			} else if (k == "pass") {
				r.is_pass = true;
				s->start_object();
				for (; !s->end_object(); s->read_separator()) {
					auto kk = s->read_key();
					if (kk == "punter") {
						r.punter_id = s->read_number();
					} else {
						s->read_value();
					}
				}
			} else {
				s->read_value();
			}
		}
		return r;
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
		int mode = 1;
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
			ai.Init(punter_id, num_of_punters, g, futures);
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
			if (move.is_pass) {
				ostringstream ss;
				ss << "{\"pass\":{\"punter\":" << ai.PunterId() << "},\"state\":\"" << ai.State() << "\"}";
				write(ss.str());
			} else {
				ostringstream ss;
				ss << "{\"claim\":{\"punter\":" << ai.PunterId() << ",\"source\":" << move.source << ",\"target\":" << move.target << "},\"state\":\"" << ai.State() << "\"}";
				write(ss.str());
			}
		}
	}
};

int main() {
	io_Main hoge;
	hoge.run();
}
