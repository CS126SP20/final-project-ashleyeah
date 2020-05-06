// Copyright (c) 2020 Ashley Yeah. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <gflags/gflags.h>

#include "pool_app.h"

using cinder::app::App;
using cinder::app::RendererGl;
using std::string;

namespace poolapp {

const int kSamples = 8;
const int kWidth = 1800;
const int kHeight = 1100;

// gflags so player's names can be changed
DEFINE_string(player1, "Player 1", "the name of blue player");
DEFINE_string(player2, "Player 2", "the name of red player");

void ParseArgs(vector<string>* args) {
  gflags::SetUsageMessage(
      "Play a game of 8-ball Pool. Pass --helpshort for options.");
  int argc = static_cast<int>(args->size());

  vector<char*> argvs;
  for (string& str : *args) {
    argvs.push_back(&str[0]);
  }

  char** argv = argvs.data();
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

void SetUp(App::Settings* settings) {
  vector<string> args = settings->getCommandLineArgs();
  ParseArgs(&args);

  settings->setWindowSize(kWidth, kHeight);
  settings->setResizable(false);
  settings->setTitle("8-ball Pool");
}

}  // namespace poolapp


// This is a macro that runs the application.
CINDER_APP(poolapp::PoolApp,
           RendererGl(RendererGl::Options().msaa(poolapp::kSamples)),
           poolapp::SetUp)
