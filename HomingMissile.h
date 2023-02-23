
#include "Object.h"
#include "EventCollision.h"
class HomingMissile : public df::Object {

private:
    void out();
    void step();
    void hit(const df::EventCollision* p_collision_event);

public:
    HomingMissile(df::Vector hero_pos);
    int eventHandler(const df::Event* p_e) override;
    bool targetLocked;
    Object *lockedObject;

    std::vector<Object*> saucerArray;
};