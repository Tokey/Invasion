#include "Object.h"
#include "EventKeyboard.h"
#include "EventStep.h"
#include "EventMouse.h"
#include "Reticle.h"
#include <ViewObject.h>

class Hero : public df::Object {

private:
    void kbd(const df::EventKeyboard* p_keyboard_event);
    void mouse(const df::EventMouse* p_mouse_event);
    void step();
    void move(float dy);
    void moveFWB(float dy);
    void nuke();
    void loadHighScore();
    void saveHighScore();

    int move_slowdown;
    int move_countdown;

    int missile_slowdown ;
    int missile_countdown ;

    int laser_countdown;
    int laser_slowdown;

    int cannon_countdown;
    int cannon_slowdown;

    int nuke_count;

    int waveNumber;

private:
    Reticle* p_reticle;

public:
    Hero();
    ~Hero();
    void fireHomingMissile(df::Vector target);
    void fireLaser(df::Vector target);

    void fireCannon(df::Vector target);
    
    int eventHandler(const df::Event* p_e) override;

    df::ViewObject* weapon_vo = nullptr;
    df::ViewObject* wave_vo = nullptr;
    df::ViewObject* hs_vo = nullptr;
    df::ViewObject* pts_vo = nullptr;


    int highscore;
    int laserCount;
    int missileCount;
    int cannonCount;

    float laserRegenDuration;
    float missileRegenDuration;
    float cannonRegenDuration;

    float laserRegenTimer;
    float missileRegenTimer;
    float cannonRegenTimer;

    bool tankStartPosCheck;
    bool weaponLocked;
};