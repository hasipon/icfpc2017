#pragma once

#include "../../common/hasi_template3/common.hpp"

// --------------------------------------------------------

constexpr int NULL_PUNTER_ID = -148048032;
constexpr int NULL_NODE_ID = -148048031;
constexpr int NULL_EDGE_ID = -148048030;

using Edges = vector<Edge>;

#include <sstream>
#include <algorithm>
#include <iostream>
#include <deque>
#include <vector>
#include <set>
#include <map>
#include <queue>

#include <cassert>

using namespace std;

typedef long long int lli;
typedef unsigned long long ull;

#define each(i, c) for (auto& i : c)
#define unless(cond) if (!(cond))

// pair

template<typename P, typename Q>
ostream& operator << (ostream& os, const pair<P, Q> p)
{
  os << p.first << ' ' << p.second << ' ';
  return os;
}

template<typename P, typename Q>
istream& operator >> (istream& is, pair<P, Q>& p)
{
  is >> p.first >> p.second;
  return is;
}

// vector

template<typename T>
istream& operator >> (istream& is, vector<T>& v)
{
  int x;
  is >> x;
  v.resize(x);
  each (i, v) is >> i;
  return is;
}

template<typename T>
ostream& operator << (ostream& os, const vector<T>& v)
{
  os << v.size() << ' ';
  each (i, v) os << i << ' ';
  return os;
}

// set

template<typename T>
istream& operator >> (istream& is, set<T>& s)
{
  int x;
  is >> x;
  for (int i = 0; i < x; ++i) {
    T y;
    is >> y;
    s.insert(y);
  }
  return is;
}

template<typename T>
ostream& operator << (ostream& os, const set<T>& s)
{
  os << s.size() << ' ' ;
  each (i, s) os << i << ' ';
  return os;
}

// map

template<typename T, typename U>
istream& operator >> (istream& is, map<T, U>& m)
{
  int x;
  is >> x;
  for (int i = 0; i < x; ++i) {
    T y;
    U z;
    is >> y >> z;
    m[y] = z;
  }
  return is;
}

template<typename T, typename U>
ostream& operator << (ostream& os, const map<T, U>& m)
{
  os << m.size() << ' ' ;
  each (i, m) os << i.first << ' ' << i.second << ' ';
  return os;
}

using SiteID = int;

struct _Move : public vector<SiteID>
{
  int punter_id = NULL_PUNTER_ID;

  _Move(Move move) : punter_id(move.punter_id)
  {
    each (i, move.route) push_back(i);
  }

  _Move(int punter_id) : punter_id(punter_id)
  {
  }

  _Move(int punter_id, Edge e) : _Move(punter_id)
  {
    Add(e);
  }

  void Add(Edge e)
  {
    if (empty()) {
      push_back(e.first);
      push_back(e.second);
    } else {
      if (0) ;
      else if (back() == e.first) push_back(e.second);
      else if (back() == e.second) push_back(e.first);
      else assert(false);
    }
  }
  
  Move toMove() const
  {
    return Move(punter_id, *this);
  }

  bool isValid() const
  {
    return punter_id != -1;
  }
};

struct _Moves : public vector<_Move>
{
  _Moves(const Moves& moves)
  {
    each (move, moves) push_back(_Move(move));
  }
};
