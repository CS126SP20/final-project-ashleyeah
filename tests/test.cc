// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <Box2D/Box2D.h>

#include <pool/engine.h>
#include <pool/pool_balls.h>

// Engine Tests
TEST_CASE("Determine Ball in Pocket", "[Engine]") {
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

TEST_CASE("Get Player Info", "[Engine]") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  engine.IncreasePlayerScore("player1");
  REQUIRE(engine.GetPlayerScore("player1") == 1);
  REQUIRE(engine.GetPlayerScore("player2") == 0);
}

TEST_CASE("Get Player Turn", "[Engine]") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  SECTION("Player 1 Turn") {
    REQUIRE(engine.PlayerTurn("player1"));
  }
  SECTION("Player 2 Turn") {
    engine.SwitchPlayerTurn();
    REQUIRE(engine.PlayerTurn("player2"));
  }
}

