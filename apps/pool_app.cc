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
    table_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    cue_stick_{pool_world_, getWindowCenter().x, getWindowCenter().y},
    engine_{getWindowCenter(), FLAGS_player1, FLAGS_player2} ,
    state_{GameState::kBeginGame},
    blue_scored_{false},
    orange_scored_{false},
    is_first_turn_{true} {}

void PoolApp::setup() {
  pool_world_->SetContactListener(&contact_);
  for (auto pos : engine_.GetBallPositions()) {
    engine_.CreateBall(pool_world_, pos.second.x, pos.second.y, pos.first);
    table_.CreateFriction(pool_world_, engine_.GetBall(pos.first)->GetBody());
  }
}

void PoolApp::update() {

  for (int i = 0; i < 10; ++i) {
    pool_world_->Step( 1 / 30.0f, 1, 1);
  }

  for (auto ball : engine_.GetBalls()) {
    if (state_ == GameState::kInProgress) {
      if (ball.first == kCueBall && engine_.Pocketed(ball.second->GetBody())) {
        ball.second->GetBody()->SetLinearVelocity({0.0f, 0.0f});
        state_ = GameState::kFoul;
      } else if (ball.first == kEightBall
          && engine_.Pocketed(ball.second->GetBody())) {
        engine_.RemoveBall(ball.first);
        state_ = GameState::kGameOver;
      } else if (engine_.Pocketed(ball.second->GetBody())) {
        if (ball.first < kEightBall) {
          engine_.IncreasePlayerScore(FLAGS_player1);
          blue_scored_ = true;
        } else if (ball.first > kEightBall) {
          engine_.IncreasePlayerScore(FLAGS_player2);
          orange_scored_ = true;
        }
        engine_.RemoveBall(ball.first);
      }
    }
  }

  if (!BodyMoving()) {

    if (state_ == GameState::kSetup || state_ == GameState::kFoulSetup) {
      engine_.GetBall(kCueBall)->ResetContact();
    } else if (state_ == GameState::kInProgress) {
      state_ = GameState::kTurnDone;
    } else if (state_ == GameState::kFoul) {
      engine_.SwitchPlayerTurn();
      state_ = GameState::kFoulSetup;
    } else if (state_ == GameState::kTurnDone) {

      int contact = engine_.GetBall(kCueBall)->GetContact();

      string player = FLAGS_player1;
      if (!engine_.PlayerTurn(FLAGS_player1)) {
        player = FLAGS_player2;
      }

      if (is_first_turn_) {
        if (blue_scored_) {
          engine_.SetPlayerTurn(FLAGS_player1);
          is_first_turn_ = false;
        } else if (orange_scored_) {
          engine_.SetPlayerTurn(FLAGS_player2);
          is_first_turn_ = false;
        } else {
          engine_.SwitchPlayerTurn();
        }
        state_ = GameState::kSetup;
      } else if (contact == kCueBall
          || (contact == kEightBall && engine_.GetPlayerScore(player) < 7)
          || (engine_.PlayerTurn(FLAGS_player1) && (contact > kEightBall))
          || (engine_.PlayerTurn(FLAGS_player2) && (contact < kEightBall))) {
        engine_.SwitchPlayerTurn();
        state_ = GameState::kFoulSetup;
      } else if ((engine_.PlayerTurn(FLAGS_player1) && blue_scored_)
          || (engine_.PlayerTurn(FLAGS_player2) && orange_scored_)) {
        state_ = GameState::kSetup;
      } else {
        engine_.SwitchPlayerTurn();
        state_ = GameState::kSetup;
      }
      blue_scored_ = false;
      orange_scored_ = false;
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

bool PoolApp::BodyMoving() {
  for (auto ball : engine_.GetBalls()) {
    if (!(ball.second->GetBody()->GetLinearVelocity() == b2Vec2_zero)) {
      return true;
    }
  }
  return false;
}

float PoolApp::KeepInRange(float center, float lim, float pos) {
  if (pos > center + lim) {
    return center + lim;
  } else if (pos < center - lim) {
    return center - lim;
  } else {
    return pos;
  }
}

void PrintText(const string& text, const Color& color,
    const cinder::ivec2& size, const string& font,
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
  cinder::gl::drawStrokedRect(Rectf(table_x1, table_y1, table_x2, table_y2),5);

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
  for (auto ball : engine_.GetBalls()) {
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
    cinder::gl::drawSolidCircle(center, pool::kBallRadius + 0.5f);
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
  PrintText(FLAGS_player2, color, size, kDefaultFont, 60,
      {center.x + 5, center.y + 5});
  color = Color::white();
  PrintText(FLAGS_player2, color, size, kDefaultFont, 60, center);
  PrintText(std::to_string(engine_.GetPlayerScore(FLAGS_player2)), color, size,
            kDefaultFont, 60, {center.x, center.y + 75});
}

void PoolApp::mouseDown(MouseEvent event) {
  if (event.isLeftDown() && state_ != GameState::kGameOver) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    if (state_ == GameState::kSetup) {
      cue_stick_.SetupCueStick(mouse_pos,
          engine_.GetBall(kCueBall)->GetBody()->GetPosition());
    } else if (state_ == GameState::kBeginGame) {
      state_ = GameState::kSetup;
    } else if (state_ == GameState::kFoulSetup) {
      engine_.GetBall(kCueBall)->GetBody()->GetFixtureList()->SetSensor(false);
      state_ = GameState::kSetup;
    }
  }
}

void PoolApp::mouseDrag(MouseEvent event) {
  if (event.isLeftDown() && state_ == GameState::kSetup) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    cue_stick_.SetupCueStick(mouse_pos,
        engine_.GetBall(kCueBall)->GetBody()->GetPosition());
  }
}

void PoolApp::mouseMove(MouseEvent event) {

  if (state_ == GameState::kBeginGame) {
    auto x_pos = engine_.GetBall(kCueBall)->GetBody()->GetPosition().x;
    auto y_pos = KeepInRange(getWindowCenter().y,pool::kHalfTableHeight
    - pool::kBallRadius, static_cast<float>(event.getY()));

    engine_.GetBall(kCueBall)->GetBody()->SetTransform(b2Vec2(x_pos, y_pos),0);

  } else if (state_ == GameState::kFoulSetup && !BodyMoving()) {
    engine_.GetBall(kCueBall)->GetBody()->GetFixtureList()->SetSensor(true);
    auto x_pos = KeepInRange(getWindowCenter().x,pool::kHalfTableWidth
    - pool::kBallRadius, static_cast<float>(event.getX()));
    auto y_pos = KeepInRange(getWindowCenter().y,pool::kHalfTableHeight
    - pool::kBallRadius, static_cast<float>(event.getY()));

    engine_.GetBall(kCueBall)->GetBody()->SetTransform(b2Vec2(x_pos, y_pos),0);
  }
}

void PoolApp::mouseUp(MouseEvent event) {
  if (event.isLeft()) {
    b2Vec2 mouse_pos(event.getX(), event.getY());
    engine_.HitCueBall(cue_stick_.ReleaseCueStick(
        b2Vec2(getWindowCenter().x, getWindowCenter().y),
        mouse_pos, engine_.GetBall(kCueBall)->GetBody()->GetPosition()));
    state_ = GameState::kInProgress;
  }
}

}  // namespace poolapp
