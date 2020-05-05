// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_ENGINE_H_
#define FINALPROJECT_INCLUDE_POOL_ENGINE_H_

#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <pool/ball.h>

#include <vector>
#include <string>
#include <map>

namespace pool {

using std::vector;
using std::string;
using std::map;

const float kHalfWidth = 600.0f;
const float kHalfHeight = 300.0f;
const float kBallRadius = 15.0f;

class Engine {

 public:
  Engine(const cinder::vec2& center, const string& player1_name, const string& player2_name);

  void CreateBall(b2World* pool_world, float pos_x, float pos_y, int ball_type);
  map<int, b2Vec2> GetBallPositions();
  void HitCueBall(b2Vec2 force);
  Ball* GetBall(int key) const;
  map<int, Ball*> GetBalls() const;
  void RemoveBall(int key);

  bool Pocketed(b2Body* ball) const;
  void IncreasePlayerScore(const string& name);
  int GetPlayerScore(const string& name) const;
  bool PlayerTurn(const string& name) const;
  void SetPlayerTurn(const string& name);
  void SwitchPlayerTurn();

 private:
  map<int, Ball*> pool_balls_;
  vector<cinder::vec2> pocket_pos_;
  map<int, b2Vec2> ball_pos_;
  string player1_name_;
  string player2_name_;
  map<string, int> players_;
  bool is_player1_turn_;

};

} // namespace pool

#endif  // FINALPROJECT_INCLUDE_POOL_ENGINE_H_
