// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <pool/table.h>

namespace pool {

void Table::CreateTable(b2World *pool_world, float pos_x, float pos_y) {
  b2BodyDef body_def;
  body_def.type = b2_kinematicBody;

  body_def.position.Set(pos_x - 600, pos_y - 300);
  bottom_edge_ = pool_world->CreateBody(&body_def);
  left_edge_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(pos_x - 600, pos_y + 300);
  top_edge_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(pos_x + 600, pos_y - 300);
  right_edge_ = pool_world->CreateBody(&body_def);

  body_def.position.Set(pos_x- 600, pos_y- 300);
  table_body_ = pool_world->CreateBody(&body_def);

  b2PolygonShape edge_x;
  b2PolygonShape edge_y;
  b2PolygonShape table_box;

  edge_x.SetAsBox(5.0f, 600.0f);
  edge_y.SetAsBox( 1200.0f, 5.0f );
  table_box.SetAsBox(1200.0f, 600.0f);

  b2FixtureDef fixture_def;
  fixture_def.shape = &edge_x;
  left_edge_->CreateFixture(&fixture_def);
  right_edge_->CreateFixture(&fixture_def);

  fixture_def.shape = &edge_y;
  top_edge_->CreateFixture(&fixture_def);
  bottom_edge_->CreateFixture(&fixture_def);

  fixture_def.shape = &table_box;
  fixture_def.isSensor = true;
  fixture_def.friction = 1.0f;
  table_body_->CreateFixture(&fixture_def);
}

b2Body* Table::GetTableBody() const {
  return table_body_;
}

}  // namespace pool
