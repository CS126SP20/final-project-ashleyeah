[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)

# 8-Ball Pool
A simple implementation of a virtual game of pool

**Author**: Ashley Yeah - [ayeah2@illinois.edu](mailto:ayeah2@illinois.edu)

![Begin Game](https://raw.githubusercontent.com/CS126SP20/final-project-ashleyeah/master/assets/images/8-ball%20begin%20game.png?token=ANDXBLJJQTNN7CE4BYTRJZC6XSISQ)
![Playing](https://raw.githubusercontent.com/CS126SP20/final-project-ashleyeah/master/assets/images/8-ball%20playing.png?token=ANDXBLPBBT5E3O26R7UDHNS6XSI4S)

# Dependencies

**Windows or Mac**
* [CMake](https://cmake.org/)
* [Cinder (v0.9.2)](https://libcinder.org/download)
* [Box2D (cinder block)](https://github.com/y3i12/CinderBox2D)
* [gflags](https://github.com/gflags/gflags)

  **For windows, download [Visual Studio 2015](https://my.visualstudio.com/Downloads?q=visual%20studio%202015&wt.mc_id=o~msft~vscom~older-downloads) in order to use cinder*
  
# Quick Start

1) Download Cinder v0.9.2
2) Open the ~/Cinder folder in an IDE and run it in order to build Cinder
3) For windows, download Visual Studio 2015 and for the project, set the toolchain to be Visual Studio 2015
4) Download [repository](https://github.com/CS126SP20/final-project-ashleyeah.git) into a new folder within the ~/Cinder folder
5) Box2D cinder block comes with cinder

# Features

Upon downloading and building the game, simply run the game. Then press the start button.

![Start Screen](https://raw.githubusercontent.com/CS126SP20/final-project-ashleyeah/master/assets/images/8-ball%20start.png?token=ANDXBLLKLPCKV4NCZHXISGC6XSIUY)

Then simply using your mouse, left click to place the ball and continue pressing down on the cursor to drag the cue stick around the ball. Let go when ready to shoot. Be aware not to commit a foul.

![Foul](https://raw.githubusercontent.com/CS126SP20/final-project-ashleyeah/master/assets/images/8-ball%20foul.png?token=ANDXBLNGGS5OEYDOS67QTA26XSI72)
  
  Fouls consist of:
  - Pocketing the white cue ball
  - Not making contact with the ball of your own color first
  - Not making contact with any balls
  
    **You do not have to make contact with a ball of your color before the first ball is pocketed and you must make contact with the 8-ball if you have already made all of your other balls*
  
The goal is to pocket all of the balls of your own color. Once you do, then you must pocket the 8-ball to win the game. If you pocket the 8-ball before before you pocket all of your other balls, you will lose. If you pocket one of your balls, you will be given another turn to go until you miss again.

![Game Over](https://raw.githubusercontent.com/CS126SP20/final-project-ashleyeah/master/assets/images/8-ball%20game%20over.png?token=ANDXBLMUK2G6ZA4YWAPEEFC6XSJCC)

# Tests

Testing was implemented using [catch2](https://github.com/catchorg/Catch2). You can find them [here](https://github.com/CS126SP20/final-project-ashleyeah/blob/master/tests/test.cc).
