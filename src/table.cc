// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/table.h>

namespace pool {

Table::Table(b2World *pool_world, float center_x, float center_y) {
  // Creates and adds each edge of the table as well as the body to the world
  b2BodyDef body_def;
  body_def.type = b2_kinematicBody;

  body_def.position.Set(center_x - kHalfTableWidth / 2 + 2.5,
      center_y - kHalfTableHeight);
  bottom_edge1_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(center_x + kHalfTableWidth / 2 - 2.5, center_y - kHalfTableHeight);
  bottom_edge2_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(center_x - kHalfTableWidth, center_y);
  left_edge_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(center_x - kHalfTableWidth / 2 + 2.5, center_y + kHalfTableHeight);
  top_edge1_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(center_x + kHalfTableWidth / 2 - 2.5, center_y + kHalfTableHeight);
  top_edge2_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(center_x + kHalfTableWidth, center_y);
  right_edge_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(center_x, center_y);
  table_body_ = pool_world->CreateBody(&body_def);

  b2PolygonShape edge_x;
  b2PolygonShape edge_y;
  b2PolygonShape table_box;

  edge_x.SetAsBox(2.5f, kHalfTableHeight - kPocketRadius - 12.5);
  edge_y.SetAsBox( kHalfTableWidth / 2 - kPocketRadius - 10, 2.5f );
  table_box.SetAsBox(kHalfTableWidth, kHalfTableHeight);

  b2FixtureDef fixture_def;
  fixture_def.shape = &edge_x;
  left_edge_->CreateFixture(&fixture_def);
  right_edge_->CreateFixture(&fixture_def);

  fixture_def.shape = &edge_y;
  top_edge1_->CreateFixture(&fixture_def);
  top_edge2_->CreateFixture(&fixture_def);
  bottom_edge1_->CreateFixture(&fixture_def);
  bottom_edge2_->CreateFixture(&fixture_def);

  fixture_def.shape = &table_box;
  fixture_def.isSensor = true;
  fixture_def.friction = 1.0f;
  table_body_->CreateFixture(&fixture_def);
}

b2Body* Table::GetTableBody() const {
  return table_body_;
}

void Table::CreateFriction(b2World *world, b2Body *ball) {
  // Process creates a friction joint between the ball and the table body
  b2Vec2 temp_vec(0.0f, 0.0f);
  b2FrictionJointDef friction_joint_def;
  friction_joint_def.localAnchorA = temp_vec;
  friction_joint_def.localAnchorB = temp_vec;
  friction_joint_def.bodyA = ball;
  friction_joint_def.bodyB = table_body_;
  friction_joint_def.maxForce = 10.0f;
  friction_joint_def.maxTorque = 0;
  world->CreateJoint(&friction_joint_def);
}

}  // namespace pool