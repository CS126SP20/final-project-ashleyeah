// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <Box2D/Box2D.h>
#include <pool/pool_balls.h>

namespace pool {

void PoolBalls::CreateBall(b2World* pool_world, float pos_x, float pos_y, int ball_type) {
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(pos_x, pos_y);

  b2Body* ball = pool_world->CreateBody(&body_def);

  b2CircleShape ballShape;
  ballShape.m_p.Set(0, 0);
  ballShape.m_radius = kBallRadius;

  b2FixtureDef fixture_def;
  fixture_def.shape = &ballShape;
  fixture_def.restitution = 1.0f;
  fixture_def.density = 0.15f;
  ball->CreateFixture(&fixture_def);
  ball->SetLinearDamping(0.015f);
  auto* data = new Ball(ball_type, ball);
  ball->SetUserData(data);

  pool_balls_.emplace(ball_type, data);

//  pool_balls_.emplace(ball_type, ball);
}

void PoolBalls::MoveCue(b2Vec2 force) {
  b2Body* cue_ball = pool_balls_.at(0)->GetBody();
  cue_ball->SetLinearVelocity(force);
}

Ball* PoolBalls::GetBall(int key) const {
  return pool_balls_.at(key);
}

map<int, Ball*> PoolBalls::GetBalls() const {
  return pool_balls_;
}

void PoolBalls::RemoveBall(int key) {
  Ball* temp = pool_balls_.at(key);
  pool_balls_.erase(key);
  delete temp;
}


}
