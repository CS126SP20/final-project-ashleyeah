// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_CUE_BALL_H_
#define FINALPROJECT_INCLUDE_POOL_CUE_BALL_H_

namespace pool {

class CueBall {

 public:
  void Move(float force_x, float force_y);
  b2Body* GetBall() const;
  void SetBall(b2Body* ball);

 private:
  b2Body* cue_ball_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_CUE_BALL_H_
