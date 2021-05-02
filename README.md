 # Roller Game

The Game is About Roller ball that Collect coins and trying to escape from the turrents attacks.

## Grid System
The Grid is width and Length are Rows and colmuns respecitvly, and that will concolude with number of cells where the Roller player set randmoly in one of those cells and automaticlly make sure that there is now turret on this cell if so find another one until it find a spot

## Roller
It rollers with WASD on the keyboard, contain a Health bar and Score point.
if it got hit with a rocket launcher from the turret it deacrease in health untill it dies.
if it collide with a coin it increase the score.

## Coins
Number of Coins got spawned when it reaches to that no more coins get spawned unless a coin was taken so it tells the grid system to spawn another one and check if it reaches the limit or not

## Turret
The turret Rotates with a min and max rotation with a given speed, when roller in the sight of the turret it starts to predict where the roller will go to fire the turret but if turret lost the roller it return back to it's normal rotation.




----------------------------------------------------------------------------
Developed with Unreal Engine 4.26

