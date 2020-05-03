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
const string kDefaultFont = "Product Sans";
const ci::Color kPlayer1Color = {0.145f, 0.231f, 0.574f};
const ci::Color kPlayer2Color = {0.910f, 0.290f, 0.153f};

DECLARE_string(player1);
DECLARE_string(player2);

PoolApp::PoolApp()
  : pool_world_{new b2World({0.0f, 0.0f})},
    pool_balls_{},
    table_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    cue_stick_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    engine_{getWindowCenter(), FLAGS_player1, FLAGS_player2} ,
    state_{GameState::kBeginGame},
    blue_scored_{false},
    orange_scored_{false} {}

void PoolApp::setup() {
  pool_world_->SetContactListener(&contact_);
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
    if (state_ == GameState::kInProgress) {
      if (ball.first == kCueBall && engine_.Pocketed(ball.second->GetBody())) {
        ball.second->GetBody()->SetLinearVelocity({0.0f, 0.0f});
        state_ = GameState::kFoul;
      } else if (ball.first == kEightBall && engine_.Pocketed(ball.second->GetBody())) {
        pool_balls_.RemoveBall(ball.first);
        state_ = GameState::kGameOver;
      } else if (engine_.Pocketed(ball.second->GetBody())) {
        if (ball.first < kEightBall) {
          engine_.IncreasePlayerScore(FLAGS_player1);
          blue_scored_ = true;
        } else if (ball.first > kEightBall) {
          engine_.IncreasePlayerScore(FLAGS_player2);
          orange_scored_ = true;
        }
        pool_balls_.RemoveBall(ball.first);
      }
    }
  }

  if (!BodyMoving()) {
    if (state_ == GameState::kInProgress) {
      state_ = GameState::kTurnDone;
    } else if (state_ == GameState::kFoul) {
      engine_.SwitchPlayerTurn();
      state_ = GameState::kFoulSetup;
    } else if (state_ == GameState::kTurnDone) {

      int contact = pool_balls_.GetBall(kCueBall)->GetContact();

      if (contact == kCueBall || contact == kEightBall
        || (engine_.PlayerTurn(FLAGS_player1) && (contact > kEightBall))
        || (engine_.PlayerTurn(FLAGS_player2) && (contact < kEightBall))) {
        engine_.SwitchPlayerTurn();
        state_ = GameState::kFoulSetup;
      } else if ((engine_.PlayerTurn(FLAGS_player1) && blue_scored_)
      || (engine_.PlayerTurn(FLAGS_player2) && orange_scored_)) {
        blue_scored_ = false;
        orange_scored_ = false;
        state_ = GameState::kSetup;
      } else {
        engine_.SwitchPlayerTurn();
        blue_scored_ = false;
        orange_scored_ = false;
        state_ = GameState::kSetup;
      }
      pool_balls_.GetBall(kCueBall)->ResetContact();
    }
  }
}

void PoolApp::draw() {
  cinder::gl::clear();
  if (state_ != GameState::kGameOver) {
    DrawPoolTable();
    if (state_ == GameState::kBeginGame) {
      DrawGuideLine();
    }
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
    friction_joint_def.bodyA = ball.second->GetBody();
    friction_joint_def.bodyB = table_.GetTableBody();
    friction_joint_def.maxForce = 10.0f;
    friction_joint_def.maxTorque = 0;
    pool_world_->CreateJoint(&friction_joint_def);
  }
}

bool PoolApp::BodyMoving() {
  for (auto ball : pool_balls_.GetBalls()) {
    if (!(ball.second->GetBody()->GetLinearVelocity() == b2Vec2_zero)) {
      return true;
    }
  }
  return false;
}

void PoolApp::TransformCueStick(b2Vec2 mouse_pos) {
  b2Vec2 ball_pos = pool_balls_.GetBall(kCueBall)->GetBody()->GetPosition();
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

void PrintText(const string& text, const Color& color, const cinder::ivec2& size, const string& font,
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
  const cinder::vec2 locp = {loc.x - static_cast<float>(box_size.x) / 2,
                             loc.y - static_cast<float>(box_size.y) / 2};
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

  vector<cinder::vec2> pocket_center = {{table_x1, table_y1},
                                        {getWindowCenter().x, table_y1 - 5},
                                        {table_x2, table_y1},
                                        {table_x1, table_y2},
                                        {getWindowCenter().x, table_y2 + 5},
                                        {table_x2, table_y2}};

  for (const auto& pos : pocket_center) {
    cinder::gl::color(0, 0, 0);
    cinder::gl::drawSolidCircle(pos, pool::kPocketRadius);
    cinder::gl::color(1, 1, 1);
    cinder::gl::drawStrokedCircle(pos, pool::kPocketRadius, 3.0f);
  }
}

void PoolApp::DrawPoolBalls() const {
  for (auto ball : pool_balls_.GetBalls()) {
    float x = ball.second->GetBody()->GetPosition().x;
    float y = ball.second->GetBody()->GetPosition().y;
    cinder::vec2 center = {x, y};

    if (ball.first == kCueBall) {
      if (engine_.Pocketed(ball.second->GetBody())) {
        continue;
      }
      cinder::gl::color(1.0f, 1.0f, 1.0f);
    } else if (ball.first == kEightBall) {
      cinder::gl::color(0.0f, 0.0f, 0.0f);
    } else if (ball.first < kEightBall) {
      cinder::gl::color(kPlayer1Color);
    } else {
      cinder::gl::color(kPlayer2Color);
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

  if (engine_.PlayerTurn(FLAGS_player1)) {
    cinder::gl::color(kPlayer1Color);
  } else {
    cinder::gl::color(kPlayer2Color);
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
  cinder::gl::drawLine(cinder::vec2((-4 * pool::kCueHalfLength), 0.0f),
      cinder::vec2((4 * pool::kCueHalfLength), 0.0f));
  cinder::gl::popModelMatrix();
}

void PoolApp::DrawGuideLine() const {
  cinder::gl::color(1.0f, 1.0f, 1.0f);
  cinder::gl::drawLine({getWindowCenter().x - 300, getWindowCenter().y - 300},
      {getWindowCenter().x - 300, getWindowCenter().y + 300});
}

void PoolApp::DrawText() const {
  cinder::vec2 center = {125, 75};
  const cinder::ivec2 size = {200, 50};

  Color color = kPlayer1Color;
  PrintText(FLAGS_player1, color, size,
      kDefaultFont, 60, {center.x + 5, center.y + 5});
  color = Color::white();
  PrintText(FLAGS_player1, color, size, kDefaultFont, 60, center);
  PrintText(std::to_string(engine_.GetPlayerScore(FLAGS_player1)), color, size,
            kDefaultFont, 60, {center.x, center.y + 75});

  center = {getWindowWidth() - 125, 75};
  color = kPlayer2Color;
  PrintText(FLAGS_player2, color, size, kDefaultFont, 60, {center.x + 5, center.y + 5});
  color = Color::white();
  PrintText(FLAGS_player2, color, size, kDefaultFont, 60, center);
  PrintText(std::to_string(engine_.GetPlayerScore(FLAGS_player2)), color, size,
            kDefaultFont, 60, {center.x, center.y + 75});
}

void PoolApp::mouseDown(MouseEvent event) {
  if (event.isLeftDown()) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    if (state_ == GameState::kSetup) {
      TransformCueStick(mouse_pos);
    } else if (state_ == GameState::kBeginGame) {
      state_ = GameState::kSetup;
    } else if (state_ == GameState::kFoulSetup) {
      pool_balls_.GetBall(kCueBall)->GetBody()->GetFixtureList()->SetSensor(false);
      state_ = GameState::kSetup;
    }
  }
}

void PoolApp::mouseDrag(MouseEvent event) {
  if (event.isLeftDown() && state_ == GameState::kSetup) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    TransformCueStick(mouse_pos);
  }
}

void PoolApp::mouseMove(MouseEvent event) {
  if (state_ == GameState::kBeginGame) {

    auto x_pos = pool_balls_.GetBall(kCueBall)->GetBody()->GetPosition().x;
    auto y_pos = static_cast<float>(event.getY());

    if (y_pos > (getWindowCenter().y + pool::kHalfTableHeight)
      - pool::kBallRadius) {
      y_pos = (getWindowCenter().y + pool::kHalfTableHeight)
          - pool::kBallRadius;
    } else if (y_pos < (getWindowCenter().y - pool::kHalfTableHeight)
      + pool::kBallRadius) {
      y_pos = (getWindowCenter().y - pool::kHalfTableHeight)
          + pool::kBallRadius;
    }

    pool_balls_.GetBall(kCueBall)->GetBody()->SetTransform(b2Vec2(x_pos, y_pos), 0);

  } else if (state_ == GameState::kFoulSetup && !BodyMoving()) {

    pool_balls_.GetBall(kCueBall)->GetBody()->GetFixtureList()->SetSensor(true);

    auto x_pos = static_cast<float>(event.getX());
    auto y_pos = static_cast<float>(event.getY());

    if (x_pos > (getWindowCenter().x + pool::kHalfTableWidth)
      - pool::kBallRadius) {
      x_pos = (getWindowCenter().x + pool::kHalfTableWidth)
          - pool::kBallRadius;
    } else if (x_pos < (getWindowCenter().x - pool::kHalfTableWidth)
      + pool::kBallRadius) {
      x_pos = (getWindowCenter().x - pool::kHalfTableWidth)
          + pool::kBallRadius;
    }

    if (y_pos > (getWindowCenter().y + pool::kHalfTableHeight)
      - pool::kBallRadius) {
      y_pos = (getWindowCenter().y + pool::kHalfTableHeight)
          - pool::kBallRadius;
    } else if (y_pos < (getWindowCenter().y - pool::kHalfTableHeight)
      + pool::kBallRadius) {
      y_pos = (getWindowCenter().y - pool::kHalfTableHeight)
          + pool::kBallRadius;
    }

    pool_balls_.GetBall(kCueBall)->GetBody()->SetTransform(b2Vec2(x_pos, y_pos), 0);
  }
}

void PoolApp::mouseUp(MouseEvent event) {
  if (event.isLeft() && state_ == GameState::kSetup) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    cue_stick_.Transform(
        b2Vec2(getWindowCenter().x, getWindowCenter().y - 400), 0.0f);
    cue_stick_.SetProjectionRay(b2Vec2(getWindowCenter().x, -100.0f), 0.0f);
    b2Vec2 force = pool_balls_.GetBall(kCueBall)->GetBody()->GetPosition() - mouse_pos;
    float strength = force.Length() - 2 * pool::kBallRadius;
    if (strength > 1000.0f) {
      strength = 1000.0f;
    } else if (strength < pool::kBallRadius + 4.0f) {
      strength = pool::kBallRadius + 4.0f;
    }
    strength -= pool::kBallRadius + 3.0f;
    force.Normalize();
    force *= strength;
    pool_balls_.MoveCue(2.0f * force);
    state_ = GameState::kInProgress;
  }
}

}  // namespace myapp
