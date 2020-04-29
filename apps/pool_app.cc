// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include "pool_app.h"

#include <vector>
#include <Box2D/Box2D.h>
#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <gflags/gflags.h>
#include <pool/pool_balls.h>
#include <pool/cue.h>

namespace poolapp {

using std::vector;
using cinder::Color;
using cinder::ColorA;
using cinder::Rectf;
using cinder::TextBox;
using cinder::app::KeyEvent;
using cinder::app::MouseEvent;

const int kCueBall = 0;
const int kEightBall = 8;

DECLARE_string(player1);
DECLARE_string(player2);

PoolApp::PoolApp()
  : pool_world_{new b2World({0.0f, 0.0f})},
    pool_balls_{},
    table_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    cue_stick_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    engine_{getWindowCenter(), FLAGS_player1, FLAGS_player2} ,
    state_{GameState::kTurnDone},
    player1_{false},
    player1_score_{0},
    player2_score_{0} {}

void PoolApp::setup() {
  for (auto pos : engine_.GetBallPositions()) {
    pool_balls_.CreateBall(pool_world_, pos.second.x, pos.second.y, pos.first);
  }
  CreateFriction();
}

void PoolApp::update() {
  for (int i = 0; i < 10; ++i) {
    pool_world_->Step( 1 / 30.0f, 1, 1);
  }
  for (auto ball : pool_balls_.GetBalls()) {
    if (ball.first == kCueBall && engine_.Pocketed(ball.second)) {
      ball.second->SetLinearVelocity({0.0f, 0.0f});
    } else if (ball.first == kEightBall && engine_.Pocketed(ball.second)) {
      pool_balls_.RemoveBall(ball.first);
      state_ = GameState::kGameOver;
    } else if (engine_.Pocketed(ball.second)) {
      if (ball.first < kEightBall) {
        engine_.IncreasePlayerScore(FLAGS_player1);
      } else if (ball.first > kEightBall) {
        engine_.IncreasePlayerScore(FLAGS_player2);
      }
      pool_balls_.RemoveBall(ball.first);
    }
  }
  if (!BodyMoving()) {
    state_ = GameState::kSetup;
  }
}

void PoolApp::draw() {
  cinder::gl::clear();
  if (state_ != GameState::kGameOver) {
    DrawPoolTable();
    DrawPoolBalls();
    if (state_ == GameState::kSetup) {
      DrawHelpRay();
    }
    DrawCueStick();
    DrawText();
  }
}

void PoolApp::CreateFriction() {
  for (auto ball : pool_balls_.GetBalls()) {
    b2Vec2 temp_vec(0.0f, 0.0f);
    b2FrictionJointDef friction_joint_def;
    friction_joint_def.localAnchorA = temp_vec;
    friction_joint_def.localAnchorB = temp_vec;
    friction_joint_def.bodyA = ball.second;
    friction_joint_def.bodyB = table_.GetTableBody();
    friction_joint_def.maxForce = 10.0f;
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
  if (length > 250.0f) {
    length = 250.0f;
  } else if (length < pool::kBallRadius + 1.0f) {
    length = pool::kBallRadius + 1.0f;
  }
  length += pool::kCueHalfLength;
  adjust_pos.Normalize();
  adjust_pos *= length;
  ball_pos += adjust_pos;
  cue_stick_.Transform(ball_pos, angle);

  adjust_pos.Normalize();
  adjust_pos *= -4 * pool::kCueHalfLength;
  adjust_pos += ball_pos;
  cue_stick_.SetProjectionRay(adjust_pos, angle);
}

template <typename C>
void PrintText(const string& text, const C& color, const cinder::ivec2& size, const string& font,
               const int font_size, const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(font, static_cast<float>(font_size)))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
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
    cinder::gl::drawSolidCircle(circle_center, pool::kPocketRadius);
    cinder::gl::color(1, 1, 1);
    cinder::gl::drawStrokedCircle(circle_center, pool::kPocketRadius, 3.0f);
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
    cinder::gl::color(1.0f, 1.0f, 1.0f);
    cinder::gl::drawStrokedCircle(center, pool::kBallRadius);
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
//  if (player1_) {
//    cinder::gl::color(0.0f, 0.0f, 1.0f);
//  } else {
//    cinder::gl::color(1.0f, 0.0f, 0.0f);
//  }
//  cinder::gl::drawSolidRect(Rectf(-pool::kCueHalfLength, -4.0f,
//      -pool::kCueHalfLength + (pool::kCueHalfLength / 4), 4.0f));
//  cinder::gl::drawSolidRect(
//      Rectf(pool::kCueHalfLength - (pool::kCueHalfLength / 4),
//          -4.0f, pool::kCueHalfLength, 4.0f));
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
  cinder::gl::drawLine(cinder::vec2((-4 * pool::kCueHalfLength), 0.0f),
      cinder::vec2((4 * pool::kCueHalfLength), 0.0f));
  cinder::gl::popModelMatrix();
}

void PoolApp::DrawText() const {
  cinder::vec2 center = {100, 50};
  const cinder::ivec2 size = {200, 50};

  Color color = Color(0.0f, 0.0f, 1.0f);
  PrintText(FLAGS_player1, color, size,
      "Impact", 40, {center.x + 5, center.y + 5});
  color = Color::white();
  PrintText(FLAGS_player1, color, size, "Impact", 40, center);
  PrintText(std::to_string(engine_.GetPlayerScore(FLAGS_player1)), color, size,
      "Impact", 40, {center.x, center.y + 50});

  center = {getWindowWidth() - 100, 50};
  color = Color(1.0f, 0.0f, 0.0f);
  PrintText(FLAGS_player2, color, size, "Impact", 40, {center.x + 5, center.y + 5});
  color = Color::white();
  PrintText(FLAGS_player2, color, size, "Impact", 40, center);
  PrintText(std::to_string(engine_.GetPlayerScore(FLAGS_player2)), color, size,
            "Impact", 40, {center.x, center.y + 50});
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
    if (strength > 1000.0f) {
      strength = 1000.0f;
    } else if (strength < pool::kBallRadius + 4.0f) {
      strength = pool::kBallRadius + 4.0f;
    }
    strength -= pool::kBallRadius + 3.0f;
    force.Normalize();
    force *= strength;
    pool_balls_.MoveCue(5.0f * force);
    state_ = GameState::kInProgress;
  }
}

}  // namespace myapp
