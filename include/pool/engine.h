// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_ENGINE_H_
#define FINALPROJECT_INCLUDE_POOL_ENGINE_H_

#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <vector>

namespace pool {

using std::vector;

class Engine {
 public:
  void SetPocketLocations(vector<cinder::vec2> pocket_pos);
  bool Pocketed(b2Body* ball);

 private:
  vector<cinder::vec2> pocket_pos_;
};

}

#endif  // FINALPROJECT_INCLUDE_POOL_ENGINE_H_
