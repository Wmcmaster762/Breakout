# Breakout

W Kavanagh & N Merchant. Summer 2024 

## controls

A/D to move the paddle right and left.
P to pause.

## Powerups

big/small paddle (blue)
fast/slow ball (fancy yellow)
fire ball (green)

# Tasklist

## Suggested fixes

* Fix the compiler issues in the code

## Suggested tasks

* Implement mouse input for pad
* Improved VFX (ball trail, paddle hit response, brick particle destruction)
* Better UI (progress bar rather than timer for countdown).
* GameLoop
* Better ball physics (Box2D)
* Leaderboards
* More ball types (e.g., multiball, sticky ball [where you shoot the ball from the paddle every time], tiny ball, big ball, brick-tracking ball)
* Sounds with increasing tone between bounces.
* Implement commentary with calls to an LLM such as LLama

# Time Details and Changelist
<Add information to this section about the time you've taken for this task along with a professional changelist.>
* Fixed circular dependancy bug in PowerUpBase.h and PowerUpFireBall.h by removing #include "PowerUpFireBall.h" from PowerUpBase.h (Time taken: 10 mins)
* Changes the colour of the firball powerup to correctly match the intended green (Time taken: 2 mins)
* Added mouse movement for the paddle (Time taken: 30 mins)
* added ball trail (Time taken: 40 mins)
* added screen shake (Time taken: 1 hour 20 mins)
