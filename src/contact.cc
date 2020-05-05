// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/contact.h>
#include <pool/ball.h>

namespace pool {

void Contact::BeginContact(b2Contact* contact) {
  void* body_a_data = contact->GetFixtureA()->GetBody()->GetUserData();
  void* body_b_data = contact->GetFixtureB()->GetBody()->GetUserData();
  if (body_a_data && body_b_data) {
    auto* ball_a = static_cast<Ball*>(body_a_data);
    auto* ball_b = static_cast<Ball*>(body_b_data);
    if (ball_a->GetBallType() == 0) {
      ball_a->HandleContact(ball_b);
    } else if (ball_b->GetBallType() == 0) {
      ball_b->HandleContact(ball_a);
    }
  }
}

} // namespace pool