// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_OBJECT_BALLS_H_
#define FINALPROJECT_INCLUDE_POOL_OBJECT_BALLS_H_

#include <Box2D/Box2D.h>
#include <vector>

namespace pool {

class ObjectBalls {
 public:
  void AddBall(b2Body* ball);
  b2Body* GetBall(int num) const;
  void SetInitialPosition();
 private:
  std::vector<b2Body*> pool_balls_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_OBJECT_BALLS_H_
