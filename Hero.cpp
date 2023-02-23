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
enum Weapon
{
    LaserGun,
    HomingMissileLauncher,
    CannonGun
};

Weapon SelectedWeapon;
Hero::Hero() {
	setSprite("ship");
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

    nuke_count = 3;
    SelectedWeapon = Weapon::LaserGun;
    
    laserCount = 100;
    missileCount = 10;
    cannonCount = 30;

    w_vo = new df::ViewObject; // Weapons
    w_vo->setLocation(df::TOP_CENTER);
    w_vo->setValue(laserCount);
    w_vo->setViewString("Laser");
    w_vo->setColor(df::WHITE);

   
}

Hero::~Hero() {
    // Create GameOver object.
    new GameOver;

    // Mark Reticle for deletion.
    WM.markForDelete(p_reticle);

    // Make a big explosion with particles.
    df::addParticles(df::SPARKS, getPosition(), 2, df::BLUE);
    df::addParticles(df::SPARKS, getPosition(), 2, df::YELLOW);
    df::addParticles(df::SPARKS, getPosition(), 3, df::RED);
    df::addParticles(df::SPARKS, getPosition(), 3, df::RED);
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
        (p_mouse_event->getMouseButton() == df::Mouse::LEFT))
    {
        if(SelectedWeapon == Weapon::HomingMissileLauncher)
            fireHomingMissile(p_mouse_event->getMousePosition());
        else if(SelectedWeapon == Weapon::LaserGun)
            fireLaser(p_mouse_event->getMousePosition());
        else if (SelectedWeapon == Weapon::CannonGun)
            fireCannon(p_mouse_event->getMousePosition());
    }

    if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
        (p_mouse_event->getMouseButton() == df::Mouse::RIGHT))
    {
        if (SelectedWeapon == Weapon::HomingMissileLauncher)
        {
            SelectedWeapon = LaserGun;
            w_vo->setViewString("Laser");
            w_vo->setValue(laserCount);
        }
        else if (SelectedWeapon == Weapon::LaserGun)
        {
            SelectedWeapon = CannonGun;
            w_vo->setViewString("Cannon");
            w_vo->setValue(cannonCount);
        }
        else if (SelectedWeapon == Weapon::CannonGun)
        {
            SelectedWeapon = HomingMissileLauncher;
            w_vo->setViewString("Missile");
            w_vo->setValue(missileCount);
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
        }
        break;

    case df::Keyboard::SPACE:
        if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
            nuke();
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

    if (saucerArray.size() <= 0)
    {
        //Spawn new saucers         ///TODO SET DIFFICULTY
        Tank* s1 = new Tank(1,5);
        Tank* s2 = new Tank(2,7);
        Tank* s3 = new Tank(3,9);
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
    w_vo->setValue(missileCount);

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
    w_vo->setValue(laserCount);

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
    w_vo->setValue(cannonCount);

    // Fire Bullet towards target.
  // Compute normalized vector to position, then scale by speed (1).
    df::Vector v = target - getPosition();
    v.normalize();
    v.scale(1);
    Laser* p = new Laser(getPosition());
    p->setVelocity(v);

    // Play "fire" sound.
    df::Sound* p_sound = RM.getSound("fire");
    if (p_sound)
        p_sound->play();
}

void Hero::nuke()
{
    // Check if nukes left.
    if (!nuke_count)
        return;
    nuke_count--;

    EventNuke nuke;
    WM.onEvent(&nuke);

    // Send "view" event with nukes to interested ViewObjects.
    df::EventView ev("Nukes", -1, true);
    WM.onEvent(&ev);

    // Play "nuke" sound.
    df::Sound* p_sound = RM.getSound("nuke");
    if (p_sound)
        p_sound->play();
}