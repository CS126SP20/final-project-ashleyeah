// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/cue.h>

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

}