// Copyright (c) 2020 Ashley. All rights reserved.
#ifndef FINALPROJECT_POOL_TABLE_H_
#define FINALPROJECT_POOL_TABLE_H_

#include <Box2D/Box2D.h>

namespace pool {

const float kPocketRadius = 25.0f;

class Table {
 public:
  Table(b2World* pool_world, float center_x, float center_y);
  b2Body* GetTableBody() const;
 private:
  b2Body* top_edge1_;
  b2Body* top_edge2_;
  b2Body* bottom_edge1_;
  b2Body* bottom_edge2_;
  b2Body* left_edge_;
  b2Body* right_edge_;
  b2Body* table_body_;
};

}  // namespace pool


#endif // FINALPROJECT_POOL_TABLE_H_
