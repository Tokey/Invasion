//
// game.cpp
// 

// Engine includes.
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Star.h"
#include "Tank.h"
#include "Hero.h"
#include "Points.h"
#include "EventView.h"
#include "GameStart.h"

void loadResources(void) {
    RM.loadSprite("sprites/saucer-spr.txt", "saucer");
    RM.loadSprite("sprites/ship-spr.txt", "ship");
    RM.loadSprite("sprites/SpaceShip-Laser-spr.txt", "lasership");
    RM.loadSprite("sprites/SpaceShip-Cannon-spr.txt", "cannonship");
    RM.loadSprite("sprites/SpaceShip-Nuclear-spr.txt", "nukeship");
    RM.loadSprite("sprites/SpaceShip-Missile-spr.txt", "missileship");
    RM.loadSprite("sprites/ForceField-spr.txt", "forcefield");
    RM.loadSprite("sprites/antimatterExplosion-spr.txt", "antimatterexplosion");
    RM.loadSprite("sprites/bullet-spr.txt", "bullet");
    RM.loadSprite("sprites/laser-spr.txt", "laser");
    RM.loadSprite("sprites/tankCannon-spr.txt", "tankCannon");
    RM.loadSprite("sprites/cannon-spr.txt", "cannon");
    RM.loadSprite("sprites/explosion-spr.txt", "explosion");
    RM.loadSprite("sprites/gameover-spr.txt", "gameover");
    RM.loadSprite("sprites/gamestart-spr.txt", "gamestart");
    RM.loadSprite("sprites/lockedSaucer-spr.txt", "lockedSaucer");

    RM.loadSound("sounds/fire.wav", "fire");
    RM.loadSound("sounds/weaponChange.wav", "weaponChange");
    RM.loadSound("sounds/tankCannon.wav", "tankCannon");
    RM.loadSound("sounds/antimatterShield.wav", "antimatterShield");
    RM.loadSound("sounds/antimatterExplosion.wav", "antimatterExplosion");
    RM.loadSound("sounds/cannon.wav", "cannon");
    RM.loadSound("sounds/missile.wav", "missile");
    RM.loadSound("sounds/laser.wav", "laser");
    RM.loadSound("sounds/explode.wav", "explode");
    RM.loadSound("sounds/nuke.wav", "nuke");
    RM.loadSound("sounds/game-over.wav", "game over");

    RM.loadMusic("sounds/start-music.wav", "start music");
    
}

void populateWorld() {

    // Spawn some Stars.
    for (int i = 0; i < 16; i++)
        new Star;

    // Spawn GameStart object.
    new GameStart();
}

int main(int argc, char *argv[]) {

  // Start up game manager.
  if (GM.startUp())  {
    LM.writeLog("Error starting game manager!");
    GM.shutDown();
    return 1;
  }

  // Set flush of logfile during development (when done, make false).
  LM.setFlush(true);

  // Show splash screen.
  df::splash();

  loadResources();

  populateWorld();

  GM.run();

  //// Shut everything down.
  //GM.shutDown();
  return 0;
}


