#include "Object.h"
#include "EventCollision.h"
class Laser : public df::Object {

private:
    void out();
    void hit(const df::EventCollision* p_collision_event);

public:
    Laser(df::Vector hero_pos);
    int eventHandler(const df::Event* p_e) override;
};

