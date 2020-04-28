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

Engine::Engine(const cinder::vec2& center) {
  pocket_pos_ = {{center.x - kHalfWidth, center.y - kHalfHeight},
                 {center.x, center.y - kHalfHeight},
                 {center.x + kHalfWidth, center.y - kHalfHeight},
                 {center.x - kHalfWidth, center.y + kHalfHeight},
                 {center.x, center.y + kHalfHeight},
                 {center.x + kHalfWidth, center.y + kHalfHeight}};
  ball_pos_ = {{0, b2Vec2(center.x - 300, center.y)},
               {1, b2Vec2(center.x + 300 - (4 * 14), center.y)},
               {2, b2Vec2(center.x + 300 - (2 * 14), center.y - 14)},
               {3, b2Vec2(center.x + 300, center.y + (2 * 14))},
               {4, b2Vec2(center.x + 300 + (2 * 14), + center.y + 14)},
               {5, b2Vec2(center.x + 300 + (2 * 14), + center.y - (3 * 14))},
               {6, b2Vec2(center.x + 300 + (4 * 14), center.y + (4 * 14))},
               {7, b2Vec2(center.x + 300 + (4 * 14), center.y)},
               {8, b2Vec2(center.x + 300, center.y)},
               {9, b2Vec2(center.x + 300 - (2 * 14), center.y + 14)},
               {10, b2Vec2(center.x + 300, center.y - (2 * 14.0f))},
               {11, b2Vec2(center.x + 300 + (2 * 14), + center.y + (3 * 14))},
               {12, b2Vec2(center.x + 300 + (2 * 14), + center.y - 14)},
               {13, b2Vec2(center.x + 300 + (4 * 14), center.y - (4 * 14))},
               {14, b2Vec2(center.x + 300 + (4 * 14), center.y + (2 * 14))},
               {15, b2Vec2(center.x + 300 + (4 * 14), center.y - (2 * 14))},};
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

}