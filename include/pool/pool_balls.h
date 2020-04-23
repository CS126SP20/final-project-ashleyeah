// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_POOL_BALLS_H_
#define FINALPROJECT_INCLUDE_POOL_POOL_BALLS_H_

#include <Box2D/Box2D.h>
#include <map>

namespace pool {

using std::map;

const float kBallRadius = 14.0f;

class PoolBalls {
 public:
  void CreateBall(b2World* pool_world, float pos_x, float pos_y, int ball_type);
  void MoveCue(float force_x, float force_y);
  b2Body* GetBall(int key) const;
  map<int, b2Body*> GetBalls() const;
  void RemoveBall(int key);

 private:
  map<int, b2Body*> pool_balls_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_POOL_BALLS_H_
