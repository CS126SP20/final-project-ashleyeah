// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_ENGINE_H_
#define FINALPROJECT_INCLUDE_POOL_ENGINE_H_

#include <cinder/app/App.h>
#include <Box2D/Box2D.h>

#include <vector>
#include <string>
#include <map>

namespace pool {

using std::vector;
using std::string;
using std::map;

const float kHalfWidth = 600.0f;
const float kHalfHeight = 300.0f;

class Engine {
 public:
  Engine(const cinder::vec2& center, const string& player1_name, const string& player2_name);
  map<int, b2Vec2> GetBallPositions();
  bool Pocketed(b2Body* ball) const;
  void IncreasePlayerScore(const string& name);
  int GetPlayerScore(const string& name) const;
  bool PlayerTurn(const string& name) const;
  void SwitchPlayerTurn();

 private:
  vector<cinder::vec2> pocket_pos_;
  map<int, b2Vec2> ball_pos_;
  map<string, int> players_;
  bool player1_turn_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_ENGINE_H_
