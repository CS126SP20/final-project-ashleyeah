// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <cinder/app/App.h>
#include <pool/engine.h>
#include <pool/table.h>
#include <pool/pool_balls.h>
#include <string>

namespace pool {

using std::sqrt;
using std::pow;
using std::string;

float CalculateDistance(float x1, float x2, float y1, float y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Engine::Engine(const cinder::vec2& center, const string& player1_name, const string& player2_name) {
  pocket_pos_ = {{center.x - kHalfWidth, center.y - kHalfHeight},
                 {center.x, center.y - kHalfHeight},
                 {center.x + kHalfWidth, center.y - kHalfHeight},
                 {center.x - kHalfWidth, center.y + kHalfHeight},
                 {center.x, center.y + kHalfHeight},
                 {center.x + kHalfWidth, center.y + kHalfHeight}};
  ball_pos_ = {{0, b2Vec2(center.x - 300, center.y)},
               {1, b2Vec2(center.x + 300 - (4 * pool::kBallRadius), center.y)},
               {2, b2Vec2(center.x + 300 - (2 * pool::kBallRadius), center.y - pool::kBallRadius)},
               {3, b2Vec2(center.x + 300, center.y + (2 * pool::kBallRadius))},
               {4, b2Vec2(center.x + 300 + (2 * pool::kBallRadius), + center.y + pool::kBallRadius)},
               {5, b2Vec2(center.x + 300 + (2 * pool::kBallRadius), + center.y - (3 * pool::kBallRadius))},
               {6, b2Vec2(center.x + 300 + (4 * pool::kBallRadius), center.y + (4 * pool::kBallRadius))},
               {7, b2Vec2(center.x + 300 + (4 * pool::kBallRadius), center.y)},
               {8, b2Vec2(center.x + 300, center.y)},
               {9, b2Vec2(center.x + 300 - (2 * pool::kBallRadius), center.y + pool::kBallRadius)},
               {10, b2Vec2(center.x + 300, center.y - (2 * pool::kBallRadius))},
               {11, b2Vec2(center.x + 300 + (2 * pool::kBallRadius), + center.y + (3 * pool::kBallRadius))},
               {12, b2Vec2(center.x + 300 + (2 * pool::kBallRadius), + center.y - pool::kBallRadius)},
               {13, b2Vec2(center.x + 300 + (4 * pool::kBallRadius), center.y - (4 * pool::kBallRadius))},
               {14, b2Vec2(center.x + 300 + (4 * pool::kBallRadius), center.y + (2 * pool::kBallRadius))},
               {15, b2Vec2(center.x + 300 + (4 * pool::kBallRadius), center.y - (2 * pool::kBallRadius))},};
  players_ = {{player1_name, 0},
              {player2_name, 0}};
}

map<int, b2Vec2> Engine::GetBallPositions() {
  return ball_pos_;
}

bool Engine::Pocketed(b2Body* ball) const {
  float ball_x = ball->GetPosition().x;
  float ball_y = ball->GetPosition().y;
  for (auto & pos : pocket_pos_) {
    float pocket_x = pos.x;
    float pocket_y = pos.y;
    if (CalculateDistance(ball_x, pocket_x, ball_y, pocket_y) < kPocketRadius) {
      return true;
    }
  }
  return false;
}

void Engine::IncreasePlayerScore(const string& name) {
  ++players_.at(name);
}

int Engine::GetPlayerScore(const string& name) const {
  return players_.at(name);
}

}