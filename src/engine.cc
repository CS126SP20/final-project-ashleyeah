// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <cinder/app/App.h>
#include <pool/engine.h>
#include <pool/table.h>

namespace pool {

using std::sqrt;
using std::pow;

float CalculateDistance(float x1, float x2, float y1, float y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Engine::Engine(vector<cinder::vec2> pocket_pos) {
  pocket_pos_ = std::move(pocket_pos);
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

}