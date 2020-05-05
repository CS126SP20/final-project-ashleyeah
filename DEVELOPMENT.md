# Development

---

**WEEK 1**
- **4/18/20** Made basic draw method for drawing pool table 
   - [x] Need to look into how to use Box2D correctly (how to create world/bodies)
    
- **4/19/20** Implement basic friction and ball bouncing
    - Applied basics of Box2D features in order to create a ball/table (body, shape, fixture, joint)
    - Test implementations by applying arbitrary force which causes ball to move in order to test boundaries and friction
    - [x] Force applied on ball doesn't seem to be enough, may need to adjust velocity force
    
- **4/20/20** Refined ball movement and added multiple balls
    - Refined movement of ball by increasing amount of steps made per update, which makes movement of balls look moe realistic
    - Added more balls, simulating a smaller scenario of pool to test collisions between balls
    
- **4/21/20** Restructured and modularized code
    - Restructured classes in order to make building the game as efficient as possible
    - Created separate ball and table class and moved creation of each body to these classes to minimize repeated code and streamline main app class
    
- **4/22/20** Created pockets on table for ball to go into
    - Changed table creation in order to account for openings where the pockets are
    - Added engine class to deal with scoring and calculating when a ball is pocketed
    
**WEEK 2**
- **4/25/20** Added functionality of cue stick
    - Added cue stick as body and figured out how to position it around the cue ball
    - Cue stick can be used to adjust angle and power of the hitting the cue ball
    - Continued to restructure certain parts of code in order to shorten or reduce repetitive code 
    
- **4/26/20** Cue stick aiming now aided with a projection line
    - Cue stick now has a line projecting out of its front tip which acts as a way for the player to line up their shot
    - Fixed issues with the power of the shot and made it more accurate towards to the graphical representations
    
- **4/27/20** Positioned and added all 16 balls to the table
    - Added all the pool balls, with correct colors and positioning at the beginning
    - Tweaked with the force, friction, and density of the bodies in order to make the bodies balls bounce as accurately as possible
    
- **4/28/20** Started implementing scoring system
    - Added gflags library in order for players to decide their names in the command-line arguments
    - Engine takes care of the players (name, score) and keeps track of progress
    - Scoring system currently only consists of only a point to player who's corresponding ball is pocketed
    
- **4/29/20** Minor changes to game function and graphics
    - Now allows player to change the position of the ball accordingly at the beginning of the game for the break as well as when the other player commits a foul
    - Ball can only be moved along a line at the beginning, while after a foul, the other player can place the cue ball wherever
    
 **WEEK 3**
 - **5/1/20 - 5/2/20** Spent time working on contact listener in order to detect fouls
    - Rules in pool mean that I need to detect what ball the cue ball hits first or if it hits any ball at all in order to detect fouls
        -  **In pool, fouls are committed either when the cue ball goes into a pocket or when the cue ball does not make contact with any ball, or it makes contact first with ball that is not the current player's ball*
    - Added class that inherited b2ContactListener in order to keep track of contacts
    - Had to add a ball class in order implement a ball object that would keep track of its own contacts
    