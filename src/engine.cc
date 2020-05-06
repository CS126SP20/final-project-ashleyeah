// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <cinder/app/App.h>
#include <pool/engine.h>
#include <pool/table.h>
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
               {2, b2Vec2(center.x + 300 - (2 * pool::kBallRadius),
                   center.y - pool::kBallRadius)},
               {3, b2Vec2(center.x + 300, center.y + (2 * pool::kBallRadius))},
               {4, b2Vec2(center.x + 300 + (2 * pool::kBallRadius),
                   center.y + pool::kBallRadius)},
               {5, b2Vec2(center.x + 300 + (2 * pool::kBallRadius),
                   center.y - (3 * pool::kBallRadius))},
               {6, b2Vec2(center.x + 300 + (4 * pool::kBallRadius),
                   center.y + (4 * pool::kBallRadius))},
               {7, b2Vec2(center.x + 300 + (4 * pool::kBallRadius), center.y)},
               {8, b2Vec2(center.x + 300, center.y)},
               {9, b2Vec2(center.x + 300 - (2 * pool::kBallRadius),
                   center.y + pool::kBallRadius)},
               {10, b2Vec2(center.x + 300, center.y - (2 * pool::kBallRadius))},
               {11, b2Vec2(center.x + 300 + (2 * pool::kBallRadius),
                   center.y + (3 * pool::kBallRadius))},
               {12, b2Vec2(center.x + 300 + (2 * pool::kBallRadius),
                   center.y - pool::kBallRadius)},
               {13, b2Vec2(center.x + 300 + (4 * pool::kBallRadius),
                   center.y - (4 * pool::kBallRadius))},
               {14, b2Vec2(center.x + 300 + (4 * pool::kBallRadius),
                   center.y + (2 * pool::kBallRadius))},
               {15, b2Vec2(center.x + 300 + (4 * pool::kBallRadius),
                   center.y - (2 * pool::kBallRadius))}};

  player1_name_ = player1_name;
  player2_name_ = player2_name;

  players_ = {{player1_name, 0},
              {player2_name, 0}};

  is_player1_turn_ = true;
}

void Engine::CreateBall(b2World* pool_world, float pos_x, float pos_y, int ball_type) {
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(pos_x, pos_y);

  b2Body* ball = pool_world->CreateBody(&body_def);

  b2CircleShape ballShape;
  ballShape.m_p.Set(0, 0);
  ballShape.m_radius = kBallRadius;

  b2FixtureDef fixture_def;
  fixture_def.shape = &ballShape;
  fixture_def.restitution = 1.0f;
  fixture_def.density = 0.15f;
  ball->CreateFixture(&fixture_def);
  ball->SetLinearDamping(0.03f);
  auto* data = new Ball(ball_type, ball);
  ball->SetUserData(data);

  pool_balls_.emplace(ball_type, data);
}

void Engine::HitCueBall(b2Vec2 force) {
  b2Body* cue_ball = pool_balls_.at(0)->GetBody();
  cue_ball->SetLinearVelocity(force);
}

Ball* Engine::GetBall(int key) const {
  return pool_balls_.at(key);
}

map<int, Ball*> Engine::GetBalls() const {
  return pool_balls_;
}

void Engine::RemoveBall(int key) {
  Ball* temp = pool_balls_.at(key);
  pool_balls_.erase(key);
  delete temp;
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

string Engine::GetPlayerTurn() const {
  if (is_player1_turn_) {
    return player1_name_;
  } else {
    return player2_name_;
  }
}

bool Engine::IsPlayerTurn(const string &name) const {
  if (name == player1_name_) {
    return is_player1_turn_;
  } else if (name == player2_name_) {
    return !is_player1_turn_;
  }
  return false;
}

void Engine::SetPlayerTurn(const string &name) {
  if (name == player1_name_) {
    is_player1_turn_ = true;
  } else if (name == player2_name_) {
    is_player1_turn_ = false;
  }
}

void Engine::SwitchPlayerTurn() {
  is_player1_turn_ = !is_player1_turn_;
}

}