// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/cue.h>
#include <pool/engine.h>

namespace pool {

Cue::Cue(b2World* pool_world, float center_x, float center_y) {
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;

  body_def.position.Set(center_x, center_y - 400.0f);
  cue_stick_ = pool_world->CreateBody(&body_def);

  b2PolygonShape stick;

  stick.SetAsBox(kCueHalfLength, 4.0f);

  b2FixtureDef fixture_def;
  fixture_def.shape = &stick;
  fixture_def.density = 1.0f;
  fixture_def.isSensor = true;
  cue_stick_->CreateFixture(&fixture_def);

  b2BodyDef ray_def;
  ray_def.position.Set(0.0f, 0.0f);
  project_ray_ = pool_world->CreateBody(&ray_def);
}

b2Body* Cue::GetStick() const {
  return cue_stick_;
}


b2Body* Cue::GetRay() const {
  return project_ray_;
}

void Cue::Transform(b2Vec2 pos, float angle) {
  cue_stick_->SetTransform(pos, angle);
}

void Cue::SetProjectionRay(b2Vec2 pos, float angle) {
  project_ray_->SetTransform(pos, angle);
}

void Cue::SetupCueStick(b2Vec2 mouse_pos, b2Vec2 ball_pos) {
  b2Vec2 adjust_pos = mouse_pos - ball_pos;
  float angle = atan(adjust_pos.y/adjust_pos.x);
  float length = adjust_pos.Length() - 2 * kBallRadius;
  if (length > 250.0f) {
    length = 250.0f;
  } else if (length < kBallRadius + 1.0f) {
    length = kBallRadius + 1.0f;
  }
  length += pool::kCueHalfLength;
  adjust_pos.Normalize();
  adjust_pos *= length;
  ball_pos += adjust_pos;
  Transform(ball_pos, angle);

  adjust_pos.Normalize();
  adjust_pos *= -4 * pool::kCueHalfLength;
  adjust_pos += ball_pos;
  SetProjectionRay(adjust_pos, angle);
}

b2Vec2 Cue::ReleaseCueStick(b2Vec2 window_center, b2Vec2 mouse_pos, b2Vec2 ball_pos) {
  Transform(b2Vec2(window_center.x, window_center.y - 400), 0.0f);
  SetProjectionRay(b2Vec2(window_center.x, -100.0f), 0.0f);
  b2Vec2 force = ball_pos - mouse_pos;
  float strength = force.Length() - 2 * pool::kBallRadius;
  if (strength > 1000.0f) {
    strength = 1000.0f;
  } else if (strength < pool::kBallRadius + 4.0f) {
    strength = pool::kBallRadius + 4.0f;
  }
  strength -= pool::kBallRadius + 3.0f;
  force.Normalize();
  force *= strength;
  return 4.0f * force;
}

} // namespace pool