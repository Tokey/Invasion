#pragma once
#include "Object.h"
#include "EventCollision.h"
class TankCannon : public df::Object {

private:
    void out();
    void hit(const df::EventCollision* p_collision_event);

public:
    TankCannon(df::Vector hero_pos);
    int eventHandler(const df::Event* p_e) override;
};

