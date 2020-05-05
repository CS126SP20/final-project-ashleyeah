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
  b2Body* GetRay() const;
  void SetupCueStick(b2Vec2 mouse_pos, b2Vec2 ball_pos);
  b2Vec2 ReleaseCueStick(b2Vec2 window_center, b2Vec2 mouse_pos, b2Vec2 ball_pos);

 private:
  void Transform(b2Vec2 pos, float angle);
  void SetProjectionRay(b2Vec2 pos, float angle);

  b2Body* cue_stick_;
  b2Body* project_ray_;

};

} // namespace pool

#endif  // FINALPROJECT_INCLUDE_POOL_CUE_H_
