// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_ENGINE_H_
#define FINALPROJECT_INCLUDE_POOL_ENGINE_H_

#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <map>

namespace pool {

using std::vector;
using std::map;
const float kHalfWidth = 600.0f;
const float kHalfHeight = 300.0f;

class Engine {
 public:
  explicit Engine(const cinder::vec2& center);
  map<int, b2Vec2> GetBallPositions();
  bool Pocketed(b2Body* ball) const;

 private:
  vector<cinder::vec2> pocket_pos_;
  map<int, b2Vec2> ball_pos_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_ENGINE_H_
