# Development

---

**Week 1**
- **4/18/20** Made basic draw method for drawing pool table 
    - ~~Need to look into how to use Box2D correctly (how to create world/bodies)~~
    
- **4/19/20** Implement basic friction and ball bouncing
    - Applied basics of Box2D features in order to create a ball/table (body, shape, fixture, joint)
    - Test inplementations by applying arbitrary force which causes ball to move in order to test boundaries and friction
    - Force applied on ball doesn't seem to be enough, may need to adjust velocity force
    
- **4/20/20** Refined ball movement and added multiple balls
    - Refined movement of ball by increasing amount of steps made per update, which makes movement of balls look moe realistic
    - Added more balls, simulating a smaller scenario of pool to test collisions between balls
    
- **4/21/20** Restructured and modularized code
    - Restructured classes in order to make building the game as efficient as possible
    - Created separate ball and table class and moved creation of each body to these classes to minimize repeated code and streamline main app class
    