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
  fixture_def.density = 0.05f;
  ball->CreateFixture(&fixture_def);
  ball->SetLinearDamping(0.01f);

  pool_balls_.emplace(ball_type, ball);
}

void PoolBalls::MoveCue(b2Vec2 force) {
  b2Body* cue_ball = pool_balls_.at(0);
  cue_ball->SetLinearVelocity(force);
}

b2Body* PoolBalls::GetBall(int key) const {
  return pool_balls_.at(key);
}

map<int, b2Body*> PoolBalls::GetBalls() const {
  return pool_balls_;
}

void PoolBalls::RemoveBall(int key) {
  b2Body* temp = pool_balls_.at(key);
  pool_balls_.erase(key);
  temp->GetWorld()->DestroyBody(temp);
}

}
