// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <Box2D/Box2D.h>
#include <pool/engine.h>
#include <pool/ball.h>
#include <pool/cue.h>
#include <pool/table.h>

#include <map>

// Engine Class Tests
TEST_CASE("Ball Related Functions", "[Engine]") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  b2World world({0.0f, 0.0f});
  engine.CreateBall(&world, 5.0f, 10.0f, 4);
  engine.CreateBall(&world, 800.0f, 300.0f, 13);
  SECTION("Get All Balls") {
    REQUIRE(engine.GetBalls().size() == 2);
    REQUIRE(engine.GetBalls().at(4)->GetBody()->GetPosition() == b2Vec2(5.0f, 10.0f));
    REQUIRE(engine.GetBalls().at(13)->GetBody()->GetPosition() == b2Vec2(800.0f, 300.0f));
  }
  SECTION("Get Specific Ball") {
    REQUIRE(engine.GetBall(4)->GetBallType() == 4);
    REQUIRE(engine.GetBall(4)->GetBody()->GetPosition() == b2Vec2(5.0f, 10.0f));
    REQUIRE(engine.GetBall(13)->GetBallType() == 13);
    REQUIRE(engine.GetBall(13)->GetBody()->GetPosition() == b2Vec2(800.0f, 300.0f));
    REQUIRE_THROWS(engine.GetBall(5));
  }
  SECTION("Remove Ball") {
    engine.RemoveBall(4);
    REQUIRE(engine.GetBalls().size() == 1);
    REQUIRE_THROWS(engine.GetBall(4));
  }
}

TEST_CASE("Get Ball Start Positions", "[Engine]") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  std::map<int, b2Vec2> positions = engine.GetBallPositions();
  REQUIRE(positions.at(2) == b2Vec2(1170.0f, 535.0f));
  REQUIRE(positions.at(15) == b2Vec2(1260.0f, 520.0f));
}

TEST_CASE("Hit Cue Ball", "[Engine]") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  b2World world({0.0f, 0.0f});
  engine.CreateBall(&world, 5.0f, 10.0f, 0);
  engine.HitCueBall(b2Vec2(10.0f, 0.0f));
  REQUIRE(engine.GetBall(0)->GetBody()->GetLinearVelocity() == b2Vec2(10.0f, 0.0f));
}

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

TEST_CASE("Score Related Functions", "[Engine]") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  SECTION("Get Player Score") {
    REQUIRE(engine.GetPlayerScore("player1") == 0);
    REQUIRE(engine.GetPlayerScore("player2") == 0);
  }
  SECTION("Increase Player Score") {
    engine.IncreasePlayerScore("player1");
    REQUIRE(engine.GetPlayerScore("player1") == 1);
  }
}

TEST_CASE("Player Turn Related Functions", "[Engine]") {
  pool::Engine engine({900.0f, 550.0f}, "player1", "player2");
  SECTION("Check If Current Player Turn") {
    REQUIRE(engine.PlayerTurn("player1"));
  }
  SECTION("Switch Player Turn") {
    engine.SwitchPlayerTurn();
    REQUIRE(engine.PlayerTurn("player2"));
  }
  SECTION("Set Player Turn") {
    engine.SetPlayerTurn("player1");
    REQUIRE(engine.PlayerTurn("player1"));
  }
}

// Ball Class tests
TEST_CASE("Create and Get Attributes of Ball", "[Ball]") {
  b2World world({0.0f, 0.0f});
  b2BodyDef body_def;
  b2Body* body = world.CreateBody(&body_def);
  pool::Ball ball(0, body);
  SECTION("Get Ball Type") {
    REQUIRE(ball.GetBallType() == 0);
  }
  SECTION("Get Ball Body") {
    REQUIRE(ball.GetBody()->GetPosition() == b2Vec2(0.0f, 0.0f));
  }
  SECTION("Get Ball First Contact") {
    REQUIRE(ball.GetContact() == 0);
  }
}

TEST_CASE("Ball Contact Handling", "[Ball]") {
  b2World world({0.0f, 0.0f});
  b2BodyDef body_def;
  b2Body* body = world.CreateBody(&body_def);
  b2Body* body2 = world.CreateBody(&body_def);
  pool::Ball ball(0, body);
  pool::Ball other(2, body2);
  SECTION("Determine First Contact") {
    ball.HandleContact(&other);
    REQUIRE(ball.GetContact() == 2);
  }
  SECTION("Reset First Contact") {
    ball.ResetContact();
    REQUIRE(ball.GetContact() == 0);
  }
}

// Cue Stick Class Tests
TEST_CASE("Create and Access Cue", "[Cue]") {
  b2World world({0.0f, 0.0f});
  pool::Cue cue(&world, 900.0f, 550.0f);
  SECTION("Get Stick") {
    REQUIRE(cue.GetStick()->GetPosition() == b2Vec2(900.0f, 150.0f));
  }
  SECTION("Get Guideline Ray") {
    REQUIRE(cue.GetRay()->GetPosition() == b2Vec2_zero);
  }
}

TEST_CASE("Using Cue", "[Cue]") {
  b2World world({0.0f, 0.0f});
  pool::Cue cue(&world, 900.0f, 550.0f);
  SECTION("Setup Cue Stick Around Cue Ball") {
    cue.SetupCueStick(b2Vec2(400.0f, 550.0f), b2Vec2(600.0f, 550.0f));
    REQUIRE(cue.GetStick()->GetPosition() == b2Vec2(255.0f, 550.0f));
  }
  SECTION("Release Cue with Force") {
    REQUIRE(cue.ReleaseCueStick(b2Vec2(900.0f, 550.0f), b2Vec2(400.0f, 550.0f), b2Vec2(600.0f, 550.0f)) == b2Vec2(608.0f, 0.0f));
    REQUIRE(cue.GetStick()->GetPosition() == b2Vec2(900.0f, 150.0f));
  }
}

// Table Class Test
TEST_CASE("Creation of Table", "[Table]") {
  b2World world({0.0f, 0.0f});
  pool::Table table(&world, 900.0f, 550.0f);
  SECTION("Get Table Body") {
    REQUIRE(table.GetTableBody()->GetPosition() == b2Vec2(900.0f, 550.0f));
  }
  SECTION("Create Friction Between Table/Ball") {
    b2BodyDef body_def;
    b2Body* ball = world.CreateBody(&body_def);
    table.CreateFriction(&world, ball);
    REQUIRE(table.GetTableBody()->GetJointList()->joint->GetType() == b2JointType::e_frictionJoint);
    REQUIRE(table.GetTableBody()->GetJointList()->joint->GetBodyA() == ball);
  }
}