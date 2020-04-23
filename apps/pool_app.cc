// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include "pool_app.h"

#include <vector>
#include <Box2D/Box2D.h>
#include <cinder/Text.h>
#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <pool/pool_balls.h>

namespace poolapp {

using std::vector;
using cinder::Color;
using cinder::Rectf;
using cinder::app::KeyEvent;
using cinder::app::MouseEvent;

const int kCueBall = 0;
const int kEightBall = 8;

PoolApp::PoolApp() {
  b2Vec2 gravity;
  gravity.Set(0.0f, 0.0f);
  pool_world_ = new b2World(gravity);
}

void PoolApp::setup() {
  pool_balls_.CreateBall(pool_world_, getWindowCenter().x - 200,
      getWindowCenter().y, kCueBall);

  for (int i = 1; i < 4; ++i) {
    float pos_x = getWindowCenter().x + 200;
    float pos_y = (static_cast<float>(getWindowCenter().y
        - (1.5 * pool::kBallRadius)))
        + (static_cast<float>(i - 1) * pool::kBallRadius);
    pool_balls_.CreateBall(pool_world_, pos_x, pos_y, i);
  }

  table_.CreateTable(pool_world_, getWindowCenter().x, getWindowCenter().y);
  vector<cinder::vec2> pocket_pos =
      { {getWindowCenter().x - 600, getWindowCenter().y - 300},
        {getWindowCenter().x, getWindowCenter().y - 300},
        {getWindowCenter().x + 600, getWindowCenter().y - 300},
        {getWindowCenter().x - 600, getWindowCenter().y + 300},
        {getWindowCenter().x, getWindowCenter().y + 300},
        {getWindowCenter().x + 600, getWindowCenter().y + 300}};
  engine_.SetPocketLocations(pocket_pos);
  CreateFriction();
}

void PoolApp::update() {
  for( int i = 0; i < 8; ++i ) {
    pool_world_->Step( 1 / 30.0f, 1, 1 );
  }
  for (auto ball : pool_balls_.GetBalls()) {
    if (ball.first == kCueBall) {
      if (engine_.Pocketed(ball.second)) {
        ball.second->SetLinearDamping(2.0f);
      } else {
        ball.second->SetLinearDamping(0.0f);
      }
    } else if (engine_.Pocketed(ball.second)) {
      pool_balls_.RemoveBall(ball.first);
    }
  }
}

void PoolApp::draw() {
  cinder::gl::clear();
  DrawPoolTable();
  DrawPoolBalls();
}

void PoolApp::CreateFriction() {
  b2Vec2 temp_vec(0.0f, 0.0f);
  b2FrictionJointDef friction_joint_def;
  friction_joint_def.localAnchorA = temp_vec;
  friction_joint_def.localAnchorB = temp_vec;
  friction_joint_def.bodyA = pool_balls_.GetBall(kCueBall);
  friction_joint_def.bodyB = table_.GetTableBody();
  friction_joint_def.maxForce = 0.7f;
  friction_joint_def.maxTorque = 0;

  pool_world_->CreateJoint(&friction_joint_def);

  for (int i = 1; i < 4; ++i) {
    friction_joint_def.bodyA = pool_balls_.GetBall(i);
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
  cinder::gl::drawSolidRoundedRect(Rectf(table_x1 - 42.0f, table_y1 - 42.0f,
      table_x2 + 42.0f, table_y2 + 42.0f), 20.0f);
  cinder::gl::color(1, 1, 1);
  cinder::gl::drawStrokedRoundedRect(Rectf(table_x1 - 42.0f, table_y1 - 42.0f,
      table_x2 + 42.0f, table_y2 + 42.0f), 20.0f);

  cinder::gl::color(0.039f, 0.424f, 0.012f);
  cinder::gl::drawSolidRect(Rectf(table_x1, table_y1, table_x2, table_y2));
  cinder::gl::color(1, 1, 1);
  cinder::gl::drawStrokedRect(Rectf(table_x1, table_y1, table_x2, table_y2), 5);

  cinder::vec2 circle_center;
  for (int i = 0; i < 6; ++i) {
    if (i == 0) {
      circle_center = {table_x1, table_y1};
    } else if (i == 1) {
      circle_center = {getWindowCenter().x, table_y1 - 5};
    } else if (i == 2) {
      circle_center = {table_x2, table_y1};
    } else if (i == 3) {
      circle_center = {table_x1, table_y2};
    } else if (i == 4) {
      circle_center = {getWindowCenter().x, table_y2 + 5};
    } else {
      circle_center = {table_x2, table_y2};
    }
    cinder::gl::color(0, 0, 0);
    cinder::gl::drawSolidCircle(circle_center, 25.0f);
    cinder::gl::color(1, 1, 1);
    cinder::gl::drawStrokedCircle(circle_center, 25.0f, 3.0f);
  }
}

void PoolApp::DrawPoolBalls() const {
  for (auto ball : pool_balls_.GetBalls()) {
    float x = ball.second->GetPosition().x;
    float y = ball.second->GetPosition().y;
    cinder::vec2 center = {x, y};

    if (ball.first == kCueBall) {
      cinder::gl::color(1.0f, 1.0f, 1.0f);
    } else if (ball.first == kEightBall) {
      cinder::gl::color(0.0f, 0.0f, 0.0f);
    } else if (ball.first < kEightBall) {
      cinder::gl::color(0.0f, 0.0f, 1.0f);
    } else {
      cinder::gl::color(1.0f, 0.0f, 0.0f);
    }

    cinder::gl::drawSolidCircle(center, pool::kBallRadius);
  }
}

void PoolApp::mouseDown(MouseEvent event) {
  if (event.isLeftDown()) {
    float force_x = static_cast<float>(event.getX()) -
        pool_balls_.GetBall(kCueBall)->GetPosition().x;
    float force_y = static_cast<float>(event.getY()) -
        pool_balls_.GetBall(kCueBall)->GetPosition().y;
    pool_balls_.MoveCue(force_x, force_y);
  }
}

}  // namespace myapp
