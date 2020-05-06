// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_BALL_H_
#define FINALPROJECT_INCLUDE_POOL_BALL_H_

#include <Box2D/Box2D.h>

namespace pool {

class Ball {
 public:
  Ball(int ball_type, b2Body* body);

  // Getter for each ball aspect
  int GetBallType() const;
  int GetContact() const;
  b2Body* GetBody() const;

  // Resets ball contact to default 0
  void ResetContact();

  // Sets contact when contact listener detects contact
  void HandleContact(Ball* other);
  ~Ball();

 private:
  int ball_type_;
  b2Body* ball_;
  int contact_;

};

} // namespace pool

#endif  // FINALPROJECT_INCLUDE_POOL_BALL_H_
