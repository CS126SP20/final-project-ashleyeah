// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_CONTACT_H_
#define FINALPROJECT_INCLUDE_POOL_CONTACT_H_

#include <Box2D/Box2D.h>

namespace pool {

class Contact : public b2ContactListener {
  // Override contact listener's method
  void BeginContact(b2Contact* contact) override;
};

} // namespae pool

#endif  // FINALPROJECT_INCLUDE_POOL_CONTACT_H_
