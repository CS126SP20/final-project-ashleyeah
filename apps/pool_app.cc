// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "pool_app.h"

#include <Box2D/Box2D.h>
#include <cinder/Text.h>
#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>

namespace poolapp {

using cinder::Color;
using cinder::Rectf;
using cinder::app::KeyEvent;
using cinder::app::MouseEvent;

PoolApp::PoolApp() {}

void PoolApp::setup() {
  b2Vec2 gravity;
  gravity.Set(0.0f, 0.0f);
  pool_world_ = new b2World(gravity);

  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(getWindowCenter().x - 200, getWindowCenter().y);

  cue_ball_.SetBall(pool_world_->CreateBody(&body_def));

  b2CircleShape ballShape;
  ballShape.m_p.Set(0, 0);
  ballShape.m_radius = 17.0f;

  b2FixtureDef fixture_def;
  fixture_def.shape = &ballShape;
  fixture_def.restitution = 1.0f;
  cue_ball_.GetBall()->CreateFixture(&fixture_def);

  for (int i = 0; i < 3; ++i) {
    body_def.position.Set(getWindowCenter().x + 200, getWindowCenter().y - 25.5 + (i * 17));
    object_balls_.AddBall(pool_world_->CreateBody(&body_def));
    object_balls_.GetBall(i)->CreateFixture(&fixture_def);
  }

  CreateTableBody();
}

void PoolApp::update() {
  for( int i = 0; i < 8; ++i ) {
    pool_world_->Step( 1 / 30.0f, 1, 1 );
  }
}

void PoolApp::draw() {
  cinder::gl::clear();
  DrawPoolTable();
  DrawPoolBalls();
}

void PoolApp::CreateTableBody() {
  b2BodyDef body_def;
  body_def.type = b2_kinematicBody;

  body_def.position.Set(getWindowCenter().x - 600, getWindowCenter().y - 300);
  b2Body* bottom_edge = pool_world_->CreateBody(&body_def);
  b2Body* left_edge = pool_world_->CreateBody(&body_def);

  body_def.position.Set( getWindowCenter().x - 600, getWindowCenter().y + 300);
  b2Body* top_edge = pool_world_->CreateBody(&body_def);

  body_def.position.Set( getWindowCenter().x + 600, getWindowCenter().y - 300);
  b2Body* right_edge = pool_world_->CreateBody(&body_def);

  body_def.position.Set(getWindowCenter().x - 600, getWindowCenter().y - 300);
  b2Body* table_body = pool_world_->CreateBody(&body_def);

  b2PolygonShape edge_x;
  b2PolygonShape edge_y;
  b2PolygonShape table_box;

  edge_x.SetAsBox(5.0f, 600.0f);
  edge_y.SetAsBox( 1200.0f, 5.0f );
  table_box.SetAsBox(1200.0f, 600.0f);

  b2FixtureDef fixture_def;
  fixture_def.shape = &edge_x;
  left_edge->CreateFixture(&fixture_def);
  right_edge->CreateFixture(&fixture_def);

  fixture_def.shape = &edge_y;
  top_edge->CreateFixture(&fixture_def);
  bottom_edge->CreateFixture(&fixture_def);

  fixture_def.shape = &table_box;
  fixture_def.isSensor = true;
  fixture_def.friction = 1.0f;
  table_body->CreateFixture(&fixture_def);

  b2Vec2 temp_vec(0.0f, 0.0f);
  b2FrictionJointDef friction_joint_def;
  friction_joint_def.localAnchorA = temp_vec;
  friction_joint_def.localAnchorB = temp_vec;
  friction_joint_def.bodyA = cue_ball_.GetBall();
  friction_joint_def.bodyB = table_body;
  friction_joint_def.maxForce = 0.7f;
  friction_joint_def.maxTorque = 0;

  pool_world_->CreateJoint(&friction_joint_def);

  for (int i = 0; i < 3; ++i) {
    friction_joint_def.bodyA = object_balls_.GetBall(i);
    pool_world_->CreateJoint(&friction_joint_def);
  }
}

void PoolApp::DrawPoolTable() const {
  cinder::vec2 center = getWindowCenter();
  float table_x1 = center.x - 600;
  float table_x2 = center.x + 600;
  float table_y1 = center.y - 300;
  float table_y2 = center.y + 300;

  cinder::gl::color(0.486f, 0.341f, 0.169f);
  cinder::gl::drawSolidRoundedRect(Rectf(table_x1 - 35.0f, table_y1 - 35.0f, table_x2 + 35.0f, table_y2 + 35.0f), 20.0f);
  cinder::gl::color(1, 1, 1);
  cinder::gl::drawStrokedRoundedRect(Rectf(table_x1 - 35.0f, table_y1 - 35.0f, table_x2 + 35.0f, table_y2 + 35.0f), 20.0f);

  cinder::gl::color(0.039f, 0.424f, 0.012f);
  cinder::gl::drawSolidRect(Rectf(table_x1, table_y1, table_x2, table_y2));
  cinder::gl::color(1, 1, 1);
  cinder::gl::drawStrokedRect(Rectf(table_x1, table_y1, table_x2, table_y2), 5);

  cinder::vec2 circle_center;
  for (int i = 0; i < 6; ++i) {
    if (i == 0) {
      circle_center = {table_x1 + 5, table_y1 + 5};
    } else if (i == 1) {
      circle_center = {getWindowCenter().x, table_y1 + 5};
    } else if (i == 2) {
      circle_center = {table_x2 - 5, table_y1 + 5};
    } else if (i == 3) {
      circle_center = {table_x1 + 5, table_y2 - 5};
    } else if (i == 4) {
      circle_center = {getWindowCenter().x, table_y2 - 5};
    } else {
      circle_center = {table_x2 - 5, table_y2 - 5};
    }
    cinder::gl::color(0, 0, 0);
    cinder::gl::drawSolidCircle(circle_center, 25.0f);
    cinder::gl::color(1, 1, 1);
    cinder::gl::drawStrokedCircle(circle_center, 25.0f, 3.0f);
  }
}

void PoolApp::DrawPoolBalls() const {
  float x = cue_ball_.GetBall()->GetPosition().x;
  float y = cue_ball_.GetBall()->GetPosition().y;
  cinder::vec2 center = {x, y};

  cinder::gl::color(1.0f, 1.0f, 1.0f);
  cinder::gl::drawSolidCircle(center, 17.0f);

  for (int i = 0; i < 3; ++i) {
    x = object_balls_.GetBall(i)->GetPosition().x;
    y = object_balls_.GetBall(i)->GetPosition().y;
    center = {x, y};
    cinder::gl::color(1.0f, 0.0f, 0.0f);
    cinder::gl::drawSolidCircle(center, 17.0f);
  }
}

void PoolApp::keyDown(KeyEvent event) {}

void PoolApp::mouseDown(MouseEvent event) {
  if (event.isLeftDown()) {
    float force_x = static_cast<float>(event.getX()) - cue_ball_.GetBall()->GetPosition().x;
    float force_y = static_cast<float>(event.getY()) - cue_ball_.GetBall()->GetPosition().y;
    cue_ball_.Move(force_x, force_y);
  }
}

}  // namespace myapp
