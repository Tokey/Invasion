INVASION

SAMIN SHAHRIAR TOKEY
sstokey@wpi.edu

NHA HOANG
nhoang@wpi.edu

Code, Audio, Some Sprite and Modifications - Tokey
Sprites - Nha

Github - https://github.com/Tokey/Invasion
Music - https://youtu.be/ICwnlNitmUw

Build On top of SAUCER SHOOT using Dragonfly Engine
Mark Claypool (WPI)
2016-2022

Saucer Shoot game from Dragonfly tutorial.

Tutorial available online:

 http://dragonfly.wpi.edu/tutorial/index.html

And in book:

 Mark Claypool. Dragonfly - Program a Game Engine from Scratch,
 Interactive Media and Game Development, Worcester Polytechnic
 Institute, 2014. Online at: http://dragonfly.wpi.edu/book/

--------------------------------------------------------------------

To build:

0) Setup development environment for Windows.
See http://dragonfly.wpi.edu/engine/index.html#setup for details.

1) Build, as appropriate for the platform (e.g., F7 on Windows from Visual Studio).

2) Run game, as appropriate for the platform (e.g., F5 on Windows from Visual Studio)

3) You can copy dragonfly and SFML libraries in the root folder and run from VS.


--------------------------------------------------------------------
How to play:

A/D to move left and right
Left mouse to shoot
Right mouse to change weapon
Space to activate forceshield
p to play from menu
q to quit

Platform: Windows VS 2022
--------------------------------------------------------------------

Directories:

sounds/ - contains the sound files used in Saucer shoot
sprites/ - contains the sprite files used in Saucer shoot
vs-2022/ - contains Visual Studio solution files (Microsoft Windows)

--------------------------------------------------------------------
FINAL VERSION:

The game is being built on top of saucer-shoot. Codes of that game were be modified extensively to meet the gameplay and visual requirements of Invasion.


Invasion was inspired by Laser Blast Atari 2600. Gameplay of Laser Blast: https://youtu.be/a68hY89Mcpg


Current Features: 
3 Enemies will come and start shooting the player. After all 3 are destroyed, next wave will begin.
Enemies will start shooting faster and will move faster as the wave progresses. Speed of their weapon will increase as well.
Player has 4 weapons, they regenerate ammo with time. Left click to shoot, right click to change ammo.
Highscore will be saved and displayed on the main menu and during gameplay. 
You will earn more points in the higher waves. For killing each enemy in the first wave, you get 5. There is some randomness to the enemy. The more aggressive the enemy is, greater your point for killing it.
You are not expected to survive. The gameplay is infinite and difficulty will increase dramatically. Getting 1000 score means you are super good. 

New Classes are:
Tank (built upon saucer and modified extensively)
TankCannon (Weapon of tank)
Laser (Player weapon shoots a fast projectile directly downwards)
Cannon (same - Follows the properties of bullet - Saucer Shoot)
HomingMissile (same - seeks and destroys enemy target. Fires 2 at once)
ForceField (Shield for the player)
Hero (It was modified significantly to support our game mechanics, also works as wave manager)



