// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#ifndef FINALPROJECT_INCLUDE_POOL_ENGINE_H_
#define FINALPROJECT_INCLUDE_POOL_ENGINE_H_

#include <cinder/app/App.h>
#include <Box2D/Box2D.h>
#include <pool/ball.h>

#include <vector>
#include <string>
#include <map>

namespace pool {

using std::vector;
using std::string;
using std::map;

const float kHalfWidth = 600.0f;
const float kHalfHeight = 300.0f;
const float kBallRadius = 15.0f;

class Engine {

 public:
  Engine(const cinder::vec2& center, const string& player1_name, const string& player2_name);

  // Creates ball and adds it to the map
  void CreateBall(b2World* pool_world, float pos_x, float pos_y, int ball_type);

  // Returns map containing where each ball should be at the beginning
  map<int, b2Vec2> GetBallPositions();

  // Set the velocity of the cue ball when it is "hit"
  void HitCueBall(b2Vec2 force);

  // Get the ball object with the right key
  Ball* GetBall(int key) const;

  // Gets all of the balls as a map
  map<int, Ball*> GetBalls() const;

  // Removes the ball from the engine
  void RemoveBall(int key);

  // Checks if ball was pocketed
  bool Pocketed(b2Body* ball) const;

  // Increases player score by one point
  void IncreasePlayerScore(const string& name);

  // Gets player's current score
  int GetPlayerScore(const string& name) const;

  // Gets which player's turn it is
  string GetPlayerTurn() const;

  // Checks if it is a specified player's turn currently
  bool IsPlayerTurn(const string& name) const;

  // Sets the current turn to the given player
  void SetPlayerTurn(const string& name);

  // Switches turn to the other player
  void SwitchPlayerTurn();

 private:
  // Map that stores all the balls
  map<int, Ball*> pool_balls_;

  // Vector contains where the center of each pocket is
  vector<cinder::vec2> pocket_pos_;

  // Map stores the starting position of each ball
  map<int, b2Vec2> ball_pos_;

  // Name of each player
  string player1_name_;
  string player2_name_;

  // Contain's player's score with their name as the key
  map<string, int> players_;

  // Keeps track who's turn it is
  bool is_player1_turn_;

};

} // namespace pool

#endif  // FINALPROJECT_INCLUDE_POOL_ENGINE_H_
