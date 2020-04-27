// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include "pool_app.h"

#include <vector>
#include <Box2D/Box2D.h>
#include <cinder/Text.h>
#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <pool/pool_balls.h>
#include <pool/cue.h>

namespace poolapp {

using std::vector;
using cinder::Color;
using cinder::Rectf;
using cinder::app::KeyEvent;
using cinder::app::MouseEvent;

const int kCueBall = 0;
const int kEightBall = 8;
const float kHalfWidth = 600.0f;
const float kHalfHeight = 300.0f;

PoolApp::PoolApp()
  : pool_world_{new b2World({0.0f, 0.0f})},
    pool_balls_{},
    table_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    cue_stick_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    engine_{
  {{getWindowCenter().x - kHalfWidth, getWindowCenter().y - kHalfHeight},
              {getWindowCenter().x, getWindowCenter().y - kHalfHeight},
              {getWindowCenter().x + kHalfWidth, getWindowCenter().y - kHalfHeight},
              {getWindowCenter().x - kHalfWidth, getWindowCenter().y + kHalfHeight},
              {getWindowCenter().x, getWindowCenter().y + kHalfHeight},
              {getWindowCenter().x + kHalfWidth, getWindowCenter().y + kHalfHeight}}} ,
    state_{GameState::kTurnDone},
    player1_{true} {}

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
  CreateFriction();
}

void PoolApp::update() {
  for( int i = 0; i < 9; ++i ) {
    pool_world_->Step( 1 / 30.0f, 1, 1);
  }
  for (auto ball : pool_balls_.GetBalls()) {
    if (ball.first == kCueBall) {
      if (engine_.Pocketed(ball.second)) {
        ball.second->SetLinearVelocity({0.0f, 0.0f});
      } else {
        ball.second->SetLinearDamping(0.0f);
      }
    } else if (engine_.Pocketed(ball.second)) {
      pool_balls_.RemoveBall(ball.first);
    }
  }
  if (!BodyMoving()) {
    state_ = GameState::kSetup;
  }
}

void PoolApp::draw() {
  cinder::gl::clear();
  DrawPoolTable();
  DrawPoolBalls();
  if (state_ == GameState::kSetup) {
    DrawHelpRay();
  }
  DrawCueStick();
}

void PoolApp::CreateFriction() {
  for (auto ball : pool_balls_.GetBalls()) {
    b2Vec2 temp_vec(0.0f, 0.0f);
    b2FrictionJointDef friction_joint_def;
    friction_joint_def.localAnchorA = temp_vec;
    friction_joint_def.localAnchorB = temp_vec;
    friction_joint_def.bodyA = ball.second;
    friction_joint_def.bodyB = table_.GetTableBody();
    friction_joint_def.maxForce = 0.7f;
    friction_joint_def.maxTorque = 0;
    pool_world_->CreateJoint(&friction_joint_def);
  }
}

bool PoolApp::BodyMoving() {
  for (auto ball : pool_balls_.GetBalls()) {
    if (!(ball.second->GetLinearVelocity() == b2Vec2_zero)) {
      return true;
    }
  }
  return false;
}

void PoolApp::TransformCueStick(b2Vec2 mouse_pos) {
  b2Vec2 ball_pos = pool_balls_.GetBall(kCueBall)->GetPosition();
  b2Vec2 adjust_pos = mouse_pos - ball_pos;
  float angle = atan(adjust_pos.y/adjust_pos.x);
  float length = adjust_pos.Length() - 2 * pool::kBallRadius;
  if (length > 175.0f) {
    length = 175.0f;
  } else if (length < pool::kBallRadius + 1.0f) {
    length = pool::kBallRadius + 1.0f;
  }
  length += pool::kCueHalfLength;
  adjust_pos.Normalize();
  adjust_pos *= length;
  ball_pos += adjust_pos;
  cue_stick_.Transform(ball_pos, angle);

  adjust_pos.Normalize();
  adjust_pos *= -3 * pool::kCueHalfLength;
  adjust_pos += ball_pos;
  cue_stick_.SetProjectionRay(adjust_pos, angle);
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
      if (engine_.Pocketed(ball.second)) {
        continue;
      }
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

void PoolApp::DrawCueStick() const {
  cinder::gl::color(0.925f, 0.835f, 0.655f);
  cinder::gl::pushModelMatrix();
  cinder::gl::translate({cue_stick_.GetStick()->GetPosition().x,
                         cue_stick_.GetStick()->GetPosition().y});
  float angle = cue_stick_.GetStick()->GetAngle();
  cinder::gl::rotate(angle);
  cinder::gl::drawSolidRect(Rectf(-pool::kCueHalfLength, -4.0f,
      pool::kCueHalfLength, 4.0f));
  if (player1_) {
    cinder::gl::color(0.0f, 0.0f, 1.0f);
  } else {
    cinder::gl::color(1.0f, 0.0f, 0.0f);
  }
  cinder::gl::drawSolidRect(Rectf(-pool::kCueHalfLength, -4.0f,
      -pool::kCueHalfLength + (pool::kCueHalfLength / 4), 4.0f));
  cinder::gl::drawSolidRect(
      Rectf(pool::kCueHalfLength - (pool::kCueHalfLength / 4),
          -4.0f, pool::kCueHalfLength, 4.0f));
  cinder::gl::color(0.0f, 0.0f, 0.0f);
  cinder::gl::drawStrokedRect(Rectf(-pool::kCueHalfLength, -4.0f,
      pool::kCueHalfLength, 4.0f));
  cinder::gl::popModelMatrix();
}

void PoolApp::DrawHelpRay() const {
  cinder::gl::color(1.0f, 1.0f, 1.0f);
  cinder::gl::pushModelMatrix();
  cinder::gl::translate({cue_stick_.GetRay()->GetPosition().x,
                         cue_stick_.GetRay()->GetPosition().y});
  float angle = cue_stick_.GetRay()->GetAngle();
  cinder::gl::rotate(angle);
  cinder::gl::drawLine(cinder::vec2((-3 * pool::kCueHalfLength), 0.0f),
      cinder::vec2((3 * pool::kCueHalfLength), 0.0f));
  cinder::gl::popModelMatrix();
}

void PoolApp::mouseDown(MouseEvent event) {
  if (event.isLeftDown() && state_ == GameState::kSetup) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    TransformCueStick(mouse_pos);
  }
}

void PoolApp::mouseDrag(MouseEvent event) {
  if (event.isLeftDown() && state_ == GameState::kSetup) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    TransformCueStick(mouse_pos);
  }
}

void PoolApp::mouseUp(MouseEvent event) {
  if (event.isLeft() && state_ == GameState::kSetup) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    cue_stick_.Transform(
        b2Vec2(getWindowCenter().x, getWindowCenter().y - 400), 0.0f);
    cue_stick_.SetProjectionRay(b2Vec2(getWindowCenter().x, -100.0f), 0.0f);
    b2Vec2 force = pool_balls_.GetBall(kCueBall)->GetPosition() - mouse_pos;
    float strength = force.Length() - 2 * pool::kBallRadius;
    if (strength > 200.0f) {
      strength = 200.0f;
    } else if (strength < pool::kBallRadius + 6.0f) {
      strength = pool::kBallRadius + 6.0f;
    }
    strength -= pool::kBallRadius + 5.0f;
    force.Normalize();
    force *= strength;
    pool_balls_.MoveCue(force);
    state_ = GameState::kInProgress;
  }
}

}  // namespace myapp
