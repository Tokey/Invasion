#include "Hero.h"
#include "WorldManager.h"
#include "GameManager.h"
#include "HomingMissile.h"
#include "EventNuke.h"
#include "EventView.h"
#include "GameOver.h"
#include <utility.h>
#include <Sound.h>
#include <ResourceManager.h>
#include "Laser.h"
#include "Tank.h"
#include "Cannon.h"
#include <fstream>
#include <DisplayManager.h>
enum Weapon
{
    LaserGun,
    HomingMissileLauncher,
    CannonGun,
    NuclearWarhead
};

Weapon SelectedWeapon;
Hero::Hero() {
	setSprite("lasership");
	registerInterest(df::KEYBOARD_EVENT);
    registerInterest(df::MSE_EVENT);
    registerInterest(df::STEP_EVENT);
	setType("Hero");
	df::Vector p(7, 5); // set start pos
	setPosition(p);

    move_slowdown = 0.02;
    move_countdown = move_slowdown;

    missile_slowdown = 35;
    missile_countdown = missile_slowdown;

    laser_slowdown = 5;
    laser_countdown = laser_slowdown;

    cannon_slowdown = 10;
    cannon_countdown = cannon_slowdown;

   // Create reticle for firing bullets.
    p_reticle = new Reticle();
    p_reticle->draw();

    nukeCount = 3;
    SelectedWeapon = Weapon::LaserGun;
    
    laserCount = 100;
    missileCount = 20;
    cannonCount = 50;
    forceFieldDuration = 100;

    waveNumber = 1;

    loadHighScore();

    hs_vo = new df::ViewObject; // Count of nukes.
    hs_vo->setLocation(df::TOP_LEFT);
    hs_vo->setViewString("High Score");
    hs_vo->setValue(highscore);
    hs_vo->setColor(df::YELLOW);

    weapon_vo = new df::ViewObject; // Weapons
    weapon_vo->setLocation(df::TOP_CENTER);
    weapon_vo->setValue(laserCount);
    weapon_vo->setViewString("Laser");
    weapon_vo->setColor(df::WHITE);

    wave_vo = new df::ViewObject; // Wave
    wave_vo->setLocation(df::BOTTOM_CENTER);
    wave_vo->setValue(waveNumber);
    wave_vo->setViewString("Wave");
    wave_vo->setColor(df::GREEN);

    pts_vo = new df::ViewObject; // Points
    pts_vo->setLocation(df::TOP_RIGHT);
    pts_vo->setViewString("Points");
    pts_vo->setValue(0);
    pts_vo->setColor(df::YELLOW);

    ff_vo = new df::ViewObject; // Points
    ff_vo->setLocation(df::BOTTOM_LEFT);
    ff_vo->setViewString("Antimatter Shield");
    ff_vo->setValue(forceFieldDuration);
    ff_vo->setColor(df::YELLOW);

    
    laserRegenDuration = 1;
    missileRegenDuration = 5;
    cannonRegenDuration = 3;
    nukeRegenDuration = 60;

    laserRegenTimer = laserRegenDuration;
    missileRegenTimer = missileRegenDuration;
    cannonRegenTimer = cannonRegenDuration;
    nukeRegenTimer = nukeRegenDuration;

    weaponLocked = true;

    df::Vector v = df::Vector(getPosition().getX(), getPosition().getY() + 5) - getPosition();
    v.normalize();
    v.scale(1);
    forceField = new ForceField(getPosition());
    forceField->setActive(false);

    forceFieldActivated = false;

    nukeFlashTimer = 0;
    nukeFlashDuration = 0.5;
}

Hero::~Hero() {

    saveHighScore();

    // Create GameOver object.
    new GameOver;

    // Mark Reticle for deletion.
    WM.markForDelete(p_reticle);

    // Make a big explosion with particles.
    df::addParticles(df::SPARKS, getPosition(), 2, df::WHITE);
    df::addParticles(df::SPARKS, getPosition(), 2, df::YELLOW);
    df::addParticles(df::SPARKS, getPosition(), 3, df::RED);
    df::addParticles(df::SMOKE, getPosition(), 1, df::WHITE);
}

int Hero::eventHandler(const df::Event* p_e) {
    if (p_e->getType() == df::KEYBOARD_EVENT) {
        const df::EventKeyboard* p_keyboard_event =
            dynamic_cast <const df::EventKeyboard*> (p_e);
        kbd(p_keyboard_event);
        return 1;
    }

    if (p_e->getType() == df::MSE_EVENT) {
        const df::EventMouse* p_mouse_event =
            dynamic_cast <const df::EventMouse*> (p_e);
        mouse(p_mouse_event);
        return 1;
    }

    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    
    }

    return 0;
}

// Move up or down.
void Hero::move(float dy) {

    // See if time to move.
    if (move_countdown > 0)
        return;
    move_countdown = move_slowdown;

    // If stays on window, allow move.
    df::Vector new_pos(getPosition().getX(), getPosition().getY() + dy);
    if ((new_pos.getY() > 3) && // Maybe space for HUD?
        (new_pos.getY() < WM.getBoundary().getVertical() - 1))
        WM.moveObject(this, new_pos);
}

// Move up or down.
void Hero::moveFWB(float dx) {

    // See if time to move.
    if (move_countdown > 0)
        return;
    move_countdown = move_slowdown;

    // If stays on window, allow move.
    df::Vector new_pos(getPosition().getX() + dx, getPosition().getY() );
    if ((new_pos.getX() > 3) && 
        (new_pos.getX() < WM.getBoundary().getHorizontal() - 1))
        WM.moveObject(this, new_pos);
}

// Take appropriate action according to mouse action.
void Hero::mouse(const df::EventMouse *p_mouse_event) {

  // Pressed button?
    if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
        (p_mouse_event->getMouseButton() == df::Mouse::LEFT) && !weaponLocked && !forceFieldActivated)
    {
        if (SelectedWeapon == Weapon::HomingMissileLauncher)
            fireHomingMissile(p_mouse_event->getMousePosition());
        else if (SelectedWeapon == Weapon::LaserGun)
            fireLaser(p_mouse_event->getMousePosition());
        else if (SelectedWeapon == Weapon::CannonGun)
            fireCannon(p_mouse_event->getMousePosition());
        else if (SelectedWeapon == Weapon::NuclearWarhead)
            nuke();
    }

    if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
        (p_mouse_event->getMouseButton() == df::Mouse::RIGHT))
    {
        if (SelectedWeapon == Weapon::HomingMissileLauncher)
        {
            SelectedWeapon = LaserGun;
            weapon_vo->setViewString("Laser");
            weapon_vo->setValue(laserCount);
            setSprite("lasership");
        }
        else if (SelectedWeapon == Weapon::LaserGun)
        {
            SelectedWeapon = CannonGun;
            weapon_vo->setViewString("Cannon");
            weapon_vo->setValue(cannonCount);
            setSprite("cannonship");
        }
        else if (SelectedWeapon == Weapon::CannonGun)
        {
            SelectedWeapon = NuclearWarhead;
            weapon_vo->setViewString("Antimatter Bomb");
            weapon_vo->setValue(nukeCount);
            setSprite("nukeship");
            
        }
        else if (SelectedWeapon == Weapon::NuclearWarhead)
        {
            SelectedWeapon = HomingMissileLauncher;
            weapon_vo->setViewString("Missile");
            weapon_vo->setValue(missileCount);
            setSprite("missileship");
        }

        // Play "Weapon Change" sound.
        df::Sound* p_sound = RM.getSound("weaponChange");
        if (p_sound)
            p_sound->play();
    }
    
  /*if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
      (p_mouse_event->getMouseButton() == df::Mouse::LEFT))
      fireLaser(p_mouse_event->getMousePosition());*/
}

// Take appropriate action according to key pressed.
void Hero::kbd(const df::EventKeyboard* p_keyboard_event) {

    switch (p_keyboard_event->getKey()) {

    case df::Keyboard::Q:    // quit
        if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
            WM.markForDelete(this);
        break;

    //case df::Keyboard::W:    // up
    //    if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
    //        move(-.35);
    //    break;
    //case df::Keyboard::S:    // down
    //    if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
    //        move(+.35);
    //    break;

    case df::Keyboard::A:    // left
        if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)

        {
            if (SelectedWeapon == Weapon::LaserGun)
                moveFWB(-2);
            else if (SelectedWeapon == Weapon::CannonGun)
                moveFWB(-1.3);
            else if (SelectedWeapon == Weapon::HomingMissileLauncher)
                moveFWB(-.7);
            else if (SelectedWeapon == Weapon::NuclearWarhead)
                moveFWB(-.1);
        }

        break;

    case df::Keyboard::D:    // right
        if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
        {
            if (SelectedWeapon == Weapon::LaserGun)
                moveFWB(2);
            else if (SelectedWeapon == Weapon::CannonGun)
                moveFWB(1.3);
            else if (SelectedWeapon == Weapon::HomingMissileLauncher)
                moveFWB(.7);
            else if (SelectedWeapon == Weapon::NuclearWarhead)
                moveFWB(.1);
        }
        break;

    case df::Keyboard::SPACE:
        if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
            activateForceField();
        else if (p_keyboard_event->getKeyboardAction() == df::KEY_RELEASED)
            deactivateForceField();
        break;
    
    }

}

void Hero::step() {

    /// IS THIS DELTA TIME? OR UPDATE?

    // Move countdown.
    move_countdown--;
    if (move_countdown < 0)
        move_countdown = 0;

    // NOTE - in step()
    // Fire countdown.
    missile_countdown--;
    if (missile_countdown < 0)
        missile_countdown = 0;

    laser_countdown--;
    if (laser_countdown < 0)
        laser_countdown = 0;

    cannon_countdown--;
    if (cannon_countdown < 0)
        cannon_countdown = 0;


    laserRegenTimer -= .033;
    missileRegenTimer -= .033;
    cannonRegenTimer -= .033;
    nukeRegenTimer -= .033;

    if (forceFieldActivated && forceFieldDuration > 0)
        forceFieldDuration -= .63;
    else {
        forceFieldDuration += .063;
    }

    ff_vo->setValue(forceFieldDuration);

    if (laserRegenTimer <= 0)
    {
        laserRegenTimer = laserRegenDuration;
        laserCount++;
        if (SelectedWeapon == Weapon::LaserGun)
            weapon_vo->setValue(laserCount);
    }
    if (missileRegenTimer <= 0)
    {
        missileRegenTimer = missileRegenDuration;
        missileCount++;
        missileCount++;
        if (SelectedWeapon == Weapon::HomingMissileLauncher)
            weapon_vo->setValue(missileCount);
    }
    if (cannonRegenTimer <= 0)
    {
        cannonRegenTimer = cannonRegenDuration;
        cannonCount++;
        if (SelectedWeapon == Weapon::CannonGun)
            weapon_vo->setValue(cannonCount);
    }
    if (nukeRegenTimer <= 0)
    {
        nukeRegenTimer = nukeRegenDuration;
        nukeCount++;
        if (SelectedWeapon == Weapon::NuclearWarhead)
            weapon_vo->setValue(nukeCount);
    }


    std::vector<Object*> saucerArray;
    // Getting list of all saucers
    for (int i = 0; i < WM.getAllObjects().getCount(); i++)
    {
        Object* p_o = WM.getAllObjects()[i];
        if (p_o->getType() == "Saucer")
        {
            saucerArray.push_back(p_o);
        }
    }

    tankStartPosCheck = false;
    if (saucerArray.size() <= 0)
    {
        waveNumber++;
        //Spawn new saucers         
        ///TODO SET DIFFICULTY
        Tank* s1 = new Tank(1,rand() % 3 + 4 + waveNumber);
        Tank* s2 = new Tank(2,rand() % 3 + 4 + waveNumber);
        Tank* s3 = new Tank(3,rand() % 3 + 4 + waveNumber);

        wave_vo->setValue(waveNumber);
    }
    // Lock player weapons until saucers are ready
    else {
        for (int i = 0; i < saucerArray.size(); i++) {
            Tank* t = dynamic_cast <Tank*> (saucerArray[i]);
            if (!t->isInStartPos)
            {
                tankStartPosCheck = true;
                weaponLocked = true;
            }
        }
        if (!tankStartPosCheck)
            weaponLocked = false;
    }
    
    if (pts_vo->getValue() > highscore)
    {
        highscore = pts_vo->getValue();
        hs_vo->setValue(highscore);
    }

    if (nukeFlashTimer >= 0)
    {
        DM.setBackgroundColor(df::WHITE);
        nukeFlashTimer -= .053;
    }
    else {
        DM.setBackgroundColor(df::BLACK);
    }

}

void Hero::fireHomingMissile(df::Vector target) {
    if (missileCount <= 0) // TODO ADD NO BULLET SOUND HERE
        return;

    if (missile_countdown > 0)
        return;
    missile_countdown = missile_slowdown;

    // Fire Bullet towards target.
    // Compute normalized vector to position, then scale by speed (1).
    //df::Vector v = target - getPosition();
    //v.normalize();
    //v.scale(1);
    
    missileCount -= 2;
    weapon_vo->setValue(missileCount);

    new HomingMissile(df::Vector(getPosition().getX()+1, getPosition().getY()+2));
    new HomingMissile(df::Vector(getPosition().getX()-1, getPosition().getY()+2));
    //p->setVelocity(v);

    // Play "fire" sound.
    df::Sound* p_sound = RM.getSound("fire");
    if (p_sound)
        p_sound->play();
}

void Hero::fireLaser(df::Vector target) {

    if (laserCount <= 0) // TODO ADD NO BULLET SOUND HERE
        return;

    if (laser_countdown > 0)
        return;
    laser_countdown = laser_slowdown;

    laserCount--;
    weapon_vo->setValue(laserCount);

    // Fire Bullet towards target.
  // Compute normalized vector to position, then scale by speed (1).
    df::Vector v = df::Vector(getPosition().getX(), getPosition().getY()+3) - getPosition();
    v.normalize();
    v.scale(1);
    Laser* p = new Laser(getPosition());
    p->setVelocity(v);
    
    // Play "fire" sound.
    df::Sound* p_sound = RM.getSound("fire");
    if (p_sound)
        p_sound->play();
}

void Hero::fireCannon(df::Vector target) {

    if (cannonCount <= 0) // TODO ADD NO BULLET SOUND HERE
        return;

    if (cannon_countdown > 0)
        return;
    cannon_countdown = cannon_slowdown;

    cannonCount--;
    weapon_vo->setValue(cannonCount);

    // Fire Bullet towards target.
  // Compute normalized vector to position, then scale by speed (1).
    df::Vector v = target - getPosition();
    v.normalize();
    v.scale(1);
    Cannon * p = new Cannon(getPosition());
    p->setVelocity(v);

    // Play "fire" sound.
    df::Sound* p_sound = RM.getSound("fire");
    if (p_sound)
        p_sound->play();
}

void Hero::nuke()
{
    // Check if nukes left.
    if (!nukeCount)
        return;
    nukeCount--;

    weapon_vo->setValue(nukeCount);


    EventNuke nuke;
    WM.onEvent(&nuke);

    // Play "nuke" sound.
    df::Sound* p_sound = RM.getSound("nuke");
    if (p_sound)
        p_sound->play();

    nukeFlashTimer = nukeFlashDuration;
}

void Hero::loadHighScore()
{
    FILE* filePointer;

    filePointer = fopen("HighScore.txt", "r");
    if (filePointer == nullptr)
    {
        filePointer = fopen("HighScore.txt", "w");
        highscore = 0;
        fprintf(filePointer, "%d", highscore);
        fclose(filePointer);
    }
    else 
    {
        fscanf(filePointer, "%d", &highscore);
    }
}

void Hero::saveHighScore()
{
    FILE* filePointer;
    filePointer = fopen("HighScore.txt", "w");
    fprintf(filePointer, "%d", highscore);
    fclose(filePointer);
}

void Hero::activateForceField()
{
    if (forceFieldDuration < .3)
    {
        forceField->setActive(false);
        return;
    }
    forceField->setActive(true);
    forceField->setPosition(getPosition()+df::Vector(0,4));
    forceFieldActivated = true;
}

void Hero::deactivateForceField()
{
    forceField->setActive(false);
    forceFieldActivated = false;
}
