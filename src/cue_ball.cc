// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <Box2D/Box2D.h>
#include <pool/cue_ball.h>

namespace pool {

void CueBall::Move(float force_x, float force_y) {
  b2Vec2 force(kForceFactor * force_x, kForceFactor * force_y);
  cue_ball_->ApplyForce(force, cue_ball_->GetPosition());
}

b2Body* CueBall::GetBall() const {
  return cue_ball_;
}

void CueBall::SetBall(b2Body* ball) {
  cue_ball_ = ball;
}

}
