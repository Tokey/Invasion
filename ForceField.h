#include "Object.h"
#include "EventCollision.h"
class ForceField : public df::Object {

private:
    void out();
    void hit(const df::EventCollision* p_collision_event);

public:
    ForceField(df::Vector hero_pos);
    int eventHandler(const df::Event* p_e) override;
};