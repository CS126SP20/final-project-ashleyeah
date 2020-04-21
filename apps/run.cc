// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include "pool_app.h"

using cinder::app::App;
using cinder::app::RendererGl;


namespace poolapp {

const int kSamples = 8;
const int kWidth = 1700;
const int kHeight = 1000;

void SetUp(App::Settings* settings) {
  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("My CS 126 Application");
}

}  // namespace poolapp


// This is a macro that runs the application.
CINDER_APP(poolapp::PoolApp,
           RendererGl(RendererGl::Options().msaa(poolapp::kSamples)),
           poolapp::SetUp)
