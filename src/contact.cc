// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/contact.h>
#include <pool/ball.h>

namespace pool {

void Contact::BeginContact(b2Contact* contact) {
  // Gets the user data from the bodies involved in the contact
  void* body_a_data = contact->GetFixtureA()->GetBody()->GetUserData();
  void* body_b_data = contact->GetFixtureB()->GetBody()->GetUserData();

  // If bodies have data, that means they are balls
  if (body_a_data && body_b_data) {
    auto* ball_a = static_cast<Ball*>(body_a_data);
    auto* ball_b = static_cast<Ball*>(body_b_data);

    // If one of the balls is type 0, that means it is the cue, so take note
    // of this contact and the other body involved
    if (ball_a->GetBallType() == 0) {
      ball_a->HandleContact(ball_b);
    } else if (ball_b->GetBallType() == 0) {
      ball_b->HandleContact(ball_a);
    }
  }
}

} // namespace pool