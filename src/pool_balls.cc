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
  ball->CreateFixture(&fixture_def);

  pool_balls_.emplace(ball_type, ball);
}

void PoolBalls::MoveCue(float force_x, float force_y) {
  b2Vec2 force(-force_x, -force_y);
  b2Body* cue_ball = pool_balls_.at(0);
  cue_ball->ApplyLinearImpulse(force, cue_ball->GetPosition());
}

b2Body* PoolBalls::GetBall(int key) const {
  return pool_balls_.at(key);
}

}
