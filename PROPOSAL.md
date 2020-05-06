# [Virtual 8-Ball Pool]

Author: [Ashley Yeah]

---

 For the final project, I will be recreating a classic game of pool as a virtual game. Many iterations of this game can now be seen online or even on iMessages, where the game is often times sent around between friends to enjoy. Obviously, in order to recreate the game of pool, I need to know the scoring and rules of the game. However, this gamehttps://github.com/DanielChappuis/reactphysics3d  will also require a significant amount of physics calculations, as well as use of graphics. I don't think I'll be able to calculate the physics myself completely, which is why I will definitely be incorporating and utilizing a physics library into my project. I will be motivated to complete this project because I usually find creating games to be quite interesting and motivating. I also think having a realistic goal in my mind about how I want my project to look and function at the end will really help me towards completion.

 That being said, some of the resources I will need in order to complete this project would definitely be a physics library. There are quite a few options out there so I think it will take some more researching to decide exactly what I will use. For now, I am leaning towards using:
 - Box2D https://github.com/erincatto/Box2D - a 2D physics engine for games. This seems to have everything I need, but there is also a google extention of Box2D which might have some extra capabilities:
 - Liquidfun2D https://github.com/google/liquidfun I think 2D implementations are enough for my purposes, but if I do end up finding a need to do 3D portions of the game, an option could be:
 - ReactPhysics3D https://github.com/DanielChappuis/reactphysics3d - a physics engine library used for 3D simulations and games.
 
 All of these I will most likely still go with the first one because it is probably the most used library with enough functionality for my purposes. Obviously, I will still be using Cinder, which has many useful options for implementing the graphics portion of the game. For the timeline of the project, here is a rough estimate of what I want to get done by the end of each week:
 - Week 1: Library's are completely incorporated and I will have the basics working of maybe how the balls will roll when they hit each other
 - Week 2: Get a basic looking UI functioning, and maybe a 1 player mode done so there's no need for calculating which balls belong to which player and instead just focus on the game basically functioning
 - Week 3: Add in the 2 player functionality and then improve the graphics, maybe make the cue look more realistic or add strips and solids to the ball. If these portions are too hard, I may have to stick with a single player pool and just change or create some new rules so it is still functional and fun to play
 
 I think I will have plenty to do given the graphics and motion portion of the game can get quite difficult. However, if I do have the extra time, I can try to make the graphics more realistic with shadows and make the balls look like they're actually rolling, I can also implement more or less of the complicated rules depending on how hard the basics of creating the game will be. 