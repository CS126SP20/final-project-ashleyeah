// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_POOL_APP_H_
#define FINALPROJECT_APPS_POOL_APP_H_

#include <Box2D/Box2D.h>
#include <cinder/app/App.h>
#include <pool/cue.h>
#include <pool/engine.h>
#include <pool/pool_balls.h>
#include <pool/table.h>

namespace poolapp {

using cinder::Color;
using cinder::Rectf;
using cinder::app::MouseEvent;
using std::string;
using std::vector;

enum class GameState {
  kBeginGame,
  kSetup,
  kInProgress,
  kTurnDone,
  kFoul,
  kGameOver,
};

class PoolApp : public cinder::app::App {
 public:
  PoolApp();
  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(cinder::app::MouseEvent) override;
  void mouseDrag(cinder::app::MouseEvent) override;
  void mouseMove(cinder::app::MouseEvent) override;
  void mouseUp(cinder::app::MouseEvent) override;

 private:

  // Creates friction between balls and the table
  void CreateFriction();

  // Determines if the play is in progress by checking if any balls are
  // still moving
  bool BodyMoving();

  // Moves the cue stick based of position of the mouse
  void TransformCueStick(b2Vec2 mouse_pos);

  void DrawPoolTable() const;
  void DrawPoolBalls() const;
  void DrawCueStick() const;
  void DrawHelpRay() const;
  void DrawGuideLine() const;
  void DrawText() const;

 private:
  // Box2D world containing all bodies
  b2World* pool_world_;

  // PoolBalls object that holds information about all the pool balls
  pool::PoolBalls pool_balls_;

  // Table object holds information about the table's body and surface
  pool::Table table_;

  // Cue stick object holds information about the cue stick's body
  pool::Cue cue_stick_;

  // Engine with important functions and information about positioning
  pool::Engine engine_;

  // Current state of the game
  GameState state_;
};

}  // namespace pool

#endif  // FINALPROJECT_APPS_POOL_APP_H_
