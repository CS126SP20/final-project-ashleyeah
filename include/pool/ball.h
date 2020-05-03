// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_BALL_H_
#define FINALPROJECT_INCLUDE_POOL_BALL_H_

#include <Box2D/Box2D.h>

namespace pool {

class Ball {
 public:
  Ball(int ball_type, b2Body* body);
  int GetBallType() const;
  int GetContact() const;
  b2Body* GetBody() const;
  void ResetContact();
  void HandleContact(Ball* other);
  ~Ball();
 private:
  int ball_type_;
  b2Body* ball_;
  int contact_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_BALL_H_
