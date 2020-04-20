// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/Text.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>


namespace myapp {

using cinder::Color;
using cinder::ColorA;
using cinder::Rectf;
using cinder::TextBox;
using cinder::app::KeyEvent;
using cinder::app::MouseEvent;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using std::string;


MyApp::MyApp() {}

void MyApp::setup() {
  b2Vec2 gravity;
  gravity.Set(0.0f, 0.0f);
  pool_world_ = new b2World(gravity);

  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.gravityScale = 0.0f;
  body_def.active = true;
  body_def.position.Set(1000.0f, 500.0f);

  cue_ball_ = pool_world_->CreateBody(&body_def);

  b2CircleShape ballShape;
  ballShape.m_p.Set(0, 0);
  ballShape.m_radius = 15.0f;

  b2FixtureDef fixture_def;
  fixture_def.shape = &ballShape;
  fixture_def.restitution = 1.0f;
  cue_ball_->CreateFixture(&fixture_def);

  CreateTableBody();
}

void MyApp::update() {
  for( int i = 0; i < 5; ++i ) {
    pool_world_->Step( 1 / 30.0f, 1, 10 );
  }
}

void MyApp::draw() {
  cinder::gl::clear();
  DrawPoolTable();
  float x = cue_ball_->GetPosition().x;
  float y = cue_ball_->GetPosition().y;
  const cinder::vec2 center = {x, y};

  cinder::gl::pushModelMatrix();
  cinder::gl::color(1.0f, 1.0f, 1.0f);
  cinder::gl::drawSolidCircle(center, 15.0f);
  cinder::gl::popModelMatrix();
}

void MyApp::CreateTableBody() {
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
  table_body->CreateFixture(&fixture_def);

  b2Vec2 temp_vec(0.0f, 0.0f);
  b2FrictionJointDef friction_joint_def;
  friction_joint_def.localAnchorA = temp_vec;
  friction_joint_def.localAnchorB = temp_vec;
  friction_joint_def.bodyA = cue_ball_;
  friction_joint_def.bodyB = table_body;
  friction_joint_def.maxForce = 1.0f;
  friction_joint_def.maxTorque = 0;

  pool_world_->CreateJoint(&friction_joint_def);
}

void MyApp::DrawPoolTable() const {
  const cinder::vec2 center = getWindowCenter();
  const float table_x1 = center.x - 600;
  const float table_x2 = center.x + 600;
  const float table_y1 = center.y - 300;
  const float table_y2 = center.y + 300;

  cinder::gl::color(0.039f, 0.424f, 0.012f);
  cinder::gl::drawSolidRect(Rectf(table_x1, table_y1, table_x2, table_y2));
  cinder::gl::color(1, 1, 1);
  cinder::gl::drawStrokedRect(Rectf(table_x1, table_y1, table_x2, table_y2), 5);

}

void MyApp::keyDown(KeyEvent event) {
  if (event.getCode() == KeyEvent::KEY_SPACE) {
    b2Vec2 force(0.0f, 100.0f);
    cue_ball_->ApplyForce(force, cue_ball_->GetPosition());
  } else if (event.getCode() == KeyEvent::KEY_LSHIFT) {
    b2Vec2 force(-200.0f, 150.0f);
    cue_ball_->ApplyForce(force, cue_ball_->GetPosition());
  }
}

}  // namespace myapp
