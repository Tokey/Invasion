#include "Object.h"
#include "EventKeyboard.h"
#include "EventStep.h"
#include "EventMouse.h"
#include "Reticle.h"

class Hero : public df::Object {

private:
    void kbd(const df::EventKeyboard* p_keyboard_event);
    void mouse(const df::EventMouse* p_mouse_event);
    void step();
    void move(float dy);
    void moveFWB(float dy);
    void nuke();

    int move_slowdown;
    int move_countdown;

    int fire_slowdown ;
    int fire_countdown ;

    int laser_countdown;
    int laser_slowdown;

    int nuke_count;

private:
    Reticle* p_reticle;

public:
    Hero();
    ~Hero();
    void fireHomingMissile(df::Vector target);
    void fireLaser(df::Vector target);
    
    int eventHandler(const df::Event* p_e) override;

};