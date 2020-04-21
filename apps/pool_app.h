// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_POOL_APP_H_
#define FINALPROJECT_APPS_POOL_APP_H_

#include <Box2D/Box2D.h>
#include <cinder/app/App.h>
#include <pool/pool_balls.h>

namespace poolapp {

using cinder::Color;
using cinder::Rectf;
using cinder::app::MouseEvent;
using std::vector;

class PoolApp : public cinder::app::App {
 public:
  PoolApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  void mouseDown(cinder::app::MouseEvent) override;

 private:
  void CreateTableBody();
  void DrawPoolTable() const;
  void DrawPoolBalls() const;

 private:
  b2World* pool_world_;
  pool::PoolBalls pool_balls_;
};

}  // namespace pool

#endif  // FINALPROJECT_APPS_POOL_APP_H_
