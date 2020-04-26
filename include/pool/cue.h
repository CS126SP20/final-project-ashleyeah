// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_CUE_H_
#define FINALPROJECT_INCLUDE_POOL_CUE_H_

#include <Box2D/Box2D.h>

namespace pool {

const float kCueHalfLength = 175.0f;

class Cue {
 public:
  Cue(b2World* pool_world, float center_x, float center_y);
  b2Body* GetStick() const;
  void Transform(b2Vec2 pos, float angle);
 private:
  b2Body* cue_stick_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_CUE_H_
