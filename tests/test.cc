// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>
#include <Box2D/Box2D.h>

#include <pool/engine.h>
#include <pool/pool_balls.h>

TEST_CASE("Engine Determine Ball in Pocket") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  b2World world({0.0f, 0.0f});
  b2BodyDef body_def;
  body_def.position.Set(313.0f, 260.0f);
  b2Body* ball = world.CreateBody(&body_def);
  b2CircleShape ballShape;
  ballShape.m_p.Set(0, 0);
  ballShape.m_radius = pool::kBallRadius;
  b2FixtureDef fixture_def;
  fixture_def.shape = &ballShape;
  ball->CreateFixture(&fixture_def);

  SECTION("Ball in Pocket") {
    REQUIRE(engine.Pocketed(ball));
  }

  SECTION("Ball not Pocketed") {
    ball->SetTransform({1513.0f, 276.0f}, 0.0f);
    REQUIRE(!engine.Pocketed(ball));
  }
}

