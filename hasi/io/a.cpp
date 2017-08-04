#include <iostream>
#include <cstdio>
#include <sstream>
#include <vector>
using namespace std;

struct Move {
	int punter;
	int source;
	int target;
	bool is_pass;
	Move() : is_pass(true) {}
	Move(int source, int target) : is_pass(false), source(source), target(target) {}
};

using Moves = vector<Move>;

struct Graph {
	vector<pair<int, int> > edges;
	vector<int> mines;
};

struct AI {
	void Init(int punter_id, int num_of_punters, const Graph& g) {}
	Move Think(const Moves& moves) {
		return Move();
	}
};

int io_get_n() {
	int n = 0;
	for (;;) {
		int c = getchar();
		if (c == ':') break;
		if (!('0' <= c && c <= '9')) throw 1;
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
		if (fread(buf, 1, n, stdin) != n) throw 1;
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
	int read_number() {
		read_ws();
		{
			auto c = peek();
			if (!('0' <= c && c <= '9')) throw 1;
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
		return r;
	}
	string read_string() {
		read_ws();
		if (peek() != '"') throw 1;
		++p;
		int s = p;
		for (;;) {
			auto c = peek(); ++p;
			if (c == -1) throw 1;
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
			if ('0' <= c && c <= '9') {
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
			} else {
				throw 1; // not implemented
			}
		}
	}
	void start_object() {
		read_ws();
		if (peek() != '{') throw 1;
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
		if (peek() != '[') throw 1;
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
		if (peek() != ':') throw 1;
		++p;
		return r;
	}
	bool is_ws(int c) {
		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
	}
};

struct io_Main {
	int punter_id, num_of_punters;
	AI ai;
	Graph g;

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
						r.punter = s->read_number();
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
						r.punter = s->read_number();
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

	bool receive(Moves& moves) {
		bool stop = false;
		auto s = new io_Json();
		s->start_object();
		for (; !s->end_object(); s->read_separator()) {
			auto k = s->read_key();
			if (k == "move") {
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
			} else {
				s->read_value();
			}
		}
		delete s;
		return stop;
	}

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
			if (k == "sites") {
				s->read_value();
			} else if (k == "rivers") {
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
					if (c != 3) throw 1;
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

	void run() {
		{
			auto s = new io_Json();
			int c = 0;
			s->start_object();
			for (; !s->end_object(); s->read_separator()) {
				auto k = s->read_key();
				if (k == "punter") {
					punter_id = s->read_number();
					c |= 1;
				} else if (k == "punters") {
					num_of_punters = s->read_number();
					c |= 2;
				} else if (k == "map") {
					read_map(s);
					c |= 4;
				} else {
					s->read_value();
				}
			}
			if (c != 7) throw 1;
			ai.Init(punter_id, num_of_punters, g);
			delete s;
		}
		{
			ostringstream ss;
			ss << "{\"ready\":" << punter_id << "}";
			write(ss.str());
		}
		for (;;) {
			Moves moves;
			if (receive(moves)) break;
			auto move = ai.Think(moves);
			if (move.is_pass) {
				ostringstream ss;
				ss << "{\"pass\":{\"punter\":" << punter_id << "}}";
				write(ss.str());
			} else {
				ostringstream ss;
				ss << "{\"claim\":{\"punter\":" << punter_id << ",\"source\":" << move.source << ",\"target\":" << move.target << "}}";
				write(ss.str());
			}
		}
	}
};

int main() {
	io_Main hoge;
	hoge.run();
}
