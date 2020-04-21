// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/object_balls.h>

namespace pool {

void ObjectBalls::AddBall(b2Body* ball) {
  pool_balls_.push_back(ball);
}

b2Body* ObjectBalls::GetBall(int num) const {
  return pool_balls_[num];
}

void ObjectBalls::SetInitialPosition() {}

}