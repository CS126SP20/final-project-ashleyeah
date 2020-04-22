// Copyright (c) 2020 Ashley. All rights reserved.
#ifndef FINALPROJECT_POOL_TABLE_H_
#define FINALPROJECT_POOL_TABLE_H_

#include <Box2D/Box2D.h>

namespace pool {

class Table {
 public:
  void CreateTable(b2World* pool_world, float pos_x, float pos_y);
  b2Body* GetTableBody() const;
 private:
  b2Body* top_edge_;
  b2Body* bottom_edge_;
  b2Body* left_edge_;
  b2Body* right_edge_;
  b2Body* table_body_;
};

}  // namespace pool


#endif // FINALPROJECT_POOL_TABLE_H_
