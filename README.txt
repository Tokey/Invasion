INVASION

NHA HOANG
nhoang@wpi.edu

SAMIN SHAHRIAR TOKEY
sstokey@wpi.edu


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


Platform: Windows VS 20222
--------------------------------------------------------------------

Directories:

sounds/ - contains the sound files used in Saucer shoot
sprites/ - contains the sprite files used in Saucer shoot
vs-2022/ - contains Visual Studio solution files (Microsoft Windows)

--------------------------------------------------------------------
ALPHA VERSION:

The game is being built on top of saucer-shoot. Codes of that game will be modified extensively to meet the gameplay and visual requirements of Invasion.


Invasion was inspired by Laser Blast Atari 2600. Gameplay of Laser Blast: https://youtu.be/a68hY89Mcpg


Current Features: 
3 Enemies will come and start shooting the player. After all 3 are destroyed, next wave will begin.
Enemies will start shooting faster and will move faster as the wave progresses.
Player currently has 3 weapons, they regenerate ammo with time. Left click to shoot, right click to change ammo.
The game still uses saucer shoot sprites as placeholders. There are some comments and variable names as saucer shoot as well.
They will all be fixed before the final submission. But for alpha it was not altered.

New Classes are:
Tank (built upon saucer and modified extensively)
TankCannon (Weapon of tank)
Laser (Player weapon shoots a fast projectile directly downwards)
Cannon (same - Follows the properties of bullet - Saucer Shoot)
HomingMissile (same - seeks and destroys enemy target. Fires 2 at once)
Hero (It was modified significantly to support our game mechanics)


