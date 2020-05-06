// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/ball.h>

namespace pool {

Ball::Ball(int ball_type, b2Body* body) {
  ball_type_ = ball_type;
  ball_ = body;
  contact_ = 0;
}

int Ball::GetBallType() const {
  return ball_type_;
}

int Ball::GetContact() const {
  return contact_;
}

b2Body* Ball::GetBody() const {
  return ball_;
}

void Ball::ResetContact() {
  contact_ = 0;
}

void Ball::HandleContact(Ball *other) {
  if (contact_ == 0) {
    contact_ = other->GetBallType();
  }
}

Ball::~Ball() {
  // Calls for Box2D to destroy the ball body
  ball_->GetWorld()->DestroyBody(ball_);
}

} // namespace pool
