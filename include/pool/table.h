// Copyright (c) 2020 Ashley. All rights reserved.
#ifndef FINALPROJECT_POOL_TABLE_H_
#define FINALPROJECT_POOL_TABLE_H_

#include <Box2D/Box2D.h>

namespace pool {

const float kPocketRadius = 28.0f;
const float kHalfTableWidth = 600.0f;
const float kHalfTableHeight = 300.0f;

class Table {

 public:
  Table(b2World* pool_world, float center_x, float center_y);

  // Getter for the body of the table
  b2Body* GetTableBody() const;

  // Creates friction between each ball and the table body
  void CreateFriction(b2World* world, b2Body* ball);

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
